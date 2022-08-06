#include "t_obs.h"

OBS::OBS(QObject* parent):QThread(parent){
    _netManager=new QNetworkAccessManager(this);
    connect(_netManager,&QNetworkAccessManager::finished,this,&OBS::finishedSlot);
    connect(this,SIGNAL(sendData(QByteArray)),this,SLOT(receiveData(QByteArray)));
}
void OBS::finishedSlot(QNetworkReply* reply){
    if(reply->error()==QNetworkReply::NoError){
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>()==200){
            note->writeLine(Note::M_HMI,"上传地图差异","上传成功");
            emit putSuccessSignal(true);
        }else{
            note->writeLine(Note::M_HMI,"上传地图差异",QString("上传失败:%1").arg(reply->readAll().data()));
            emit putSuccessSignal(false);
        }
    }else{
        note->writeLine(Note::M_HMI,"上传地图差异",QString("上传失败:%1").arg(reply->readAll().data()));
        emit putSuccessSignal(false);
    }
}
void OBS::put(quint64 orderId,QString customerAddress,QString status){
    _put.orderId=orderId;
    _put.customerAddress=customerAddress;
    _put.status=status;
    start();
}
void OBS::run(){
    QString oldFilePath,newFilePath;
    oldFilePath=newFilePath=QDir::homePath()+"/jld_profile/maps/"+_mapPath+".pgm";
    QFile oldFile(oldFilePath);
    newFilePath.insert(newFilePath.indexOf("."),"_new");
    QFile newFile(newFilePath); 
    if(oldFile.exists()&&newFile.exists()){
        note->writeLine(Note::M_HMI,"上传地图差异","已找到新旧文件");
        ///获取差异数据
        oldFile.open(QIODevice::ReadOnly);
        QByteArray oldBytes=oldFile.readAll();
        oldFile.close();
        newFile.open(QIODevice::ReadOnly);
        QByteArray newBytes=newFile.readAll();
        newFile.close();
        QJsonObject obj;
        QJsonArray array;
        if(oldBytes.size()==newBytes.size()){
            for(int i=0;i<oldBytes.size();i++){
                if(oldBytes[i]!=newBytes[i]){
                    array.append(QJsonArray({i,newBytes.at(i)}));
                }
            }
            note->writeLine(Note::M_HMI,"上传地图差异","已获取比对数据");
            QString areaId=config->getValue("mapInfo").toObject().value("community_id").toString();
            ///构造json文件
            obj.insert("IndexValue",array);
            obj.insert("AreaId",areaId);
            obj.insert("BaseVersion",QFileInfo(oldFile).lastModified().toMSecsSinceEpoch());
            obj.insert("UpdateTime",QDateTime::currentMSecsSinceEpoch());
            obj.insert("OrderId",QJsonValue::fromVariant(QVariant(_put.orderId)));
            obj.insert("CustomerAddress",_put.customerAddress);
            obj.insert("MacAddress",macAddress);
            obj.insert("Direction",_put.status);
            ///获取json数据
            QJsonDocument doc;
            doc.setObject(obj);
            QByteArray data=doc.toJson(QJsonDocument::Compact);
            emit sendData(data);
        }else{
            note->writeLine(Note::M_HMI,"上传地图差异","新旧文件长度不匹配");
        }
    }else{
        note->writeLine(Note::M_HMI,"上传地图差异",QString("找不到新旧文件，oldPath：%1").arg(oldFilePath));
    }
}
void OBS::receiveData(QByteArray data){
    ///构造签名
    //1.构造请求字符串 HTTP-Verb Content-MD5 Content-Type Date CanonicalizedHeaders+CanonicalizedResource
    SIGNATRUE signature;
    signature.httpVerb="PUT";
    signature.date=formatDate();
    signature.contentType="application/json";
    QString areaId=config->getValue("mapInfo").toObject().value("community_id").toString();
    QString objName=areaId+"--"+QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch())+"--"+_put.customerAddress.replace("#","")+".json";
    signature.canonicalizedResource="/ros-map/"+objName;
    QByteArray authorization=createAuthorization(signature);
    ///put请求
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::UserAgentHeader,"curl/7.29.0");
    request.setRawHeader("Authorization",authorization);//请求消息中可带的签名信息
    request.setRawHeader("Content-Length",QString::number(data.length()).toUtf8());//消息体长度
    request.setRawHeader("Content-Type","application/json");//信息体类型
    request.setRawHeader("Date",signature.date.toUtf8());//日期
    request.setRawHeader("Host","ros-map.obs.cn-east-3.myhuaweicloud.com");//域名
    request.setRawHeader("Accept","*/*");
    request.setUrl(QUrl(_accountAccessDomainName+"/"+objName));
    _netManager->put(request,data);
    note->writeLine(Note::M_HMI,"上传地图差异","上传地图差异");
}
QByteArray OBS::hmacSha1(QByteArray key, QByteArray baseString){
    int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
    if (key.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }
    QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char"6"
    QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char"/"
    for (int i = 0; i < key.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
        outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
    }
    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(baseString);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
    QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
    return hashed;
}
QByteArray OBS::createAuthorization(SIGNATRUE signature){
    ///1.构造请求字符串(StringToSign)。
    QString stringToSign=QString("%1\n%2\n%3\n%4\n%5")
            .arg(signature.httpVerb)
            .arg(signature.contentMD5)
            .arg(signature.contentType)
            .arg(signature.date)
            .arg(signature.canonicalizedHeaders+signature.canonicalizedResource);
    ///2.对第一步的结果进行UTF-8编码。
    QByteArray stringToSignUtf8=stringToSign.toUtf8();
    ///3.使用SK对第二步的结果进行HMAC-SHA1签名计算。
    QByteArray _signature=hmacSha1(_secretAccessKey.toUtf8(),stringToSignUtf8);
    ///4.对第三步的结果进行Base64编码，得到签名。
    QByteArray __signature=_signature.toBase64();
    QByteArray authorization=QString("OBS %1").arg(_accessKeyId).toUtf8()+":"+__signature;
    return authorization;
}
QString OBS::formatDate(){
    QString format="ddd, dd MMM yyyy HH:mm:ss";
    QLocale loc=QLocale(QLocale::English,QLocale::UnitedStates);
    QDateTime t=QDateTime::currentDateTime();
    QString date=loc.toString(t.toUTC(),format)+" GMT";
    return date;
}
