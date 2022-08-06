#include "t_tts.h"

/*@@@@@@@@@@@@@@@@@@@@@*/
TTS::TTS(QObject* parent):QObject(parent){
    _netManager = new QNetworkAccessManager(this);
    connect(_netManager,&QNetworkAccessManager::finished,this,&TTS::finishedSlot);
}
void TTS::finishedSlot(QNetworkReply* reply){
    if(reply->error()==QNetworkReply::NoError){
        QByteArray bytes = reply->readAll();
        QString replyUrl = reply->url().toString();
        if(replyUrl==_baiduTokenUrl){
            _receiveToken(bytes);
        }else if(replyUrl==_baiduTtsUrl){
            if(reply->rawHeader("Content-Type")=="audio/wav"){
                 _receiveVoice(bytes);
            }
        }else{}
    }else{
        note->writeLine(Note::M_TTS,"tts出错",QString("URL:%1").arg(reply->url().toString()));
    }
}
void TTS::_receiveToken(QByteArray bytes){
    QJsonDocument tokenDoc = QJsonDocument::fromJson(bytes);
    QJsonObject tokenObj = tokenDoc.object();
    tokenObj.insert("getTime",QDateTime::currentDateTime().toSecsSinceEpoch());
    config->setValue("baiduToken",tokenObj);
}
void TTS::_receiveVoice(QByteArray bytes){
    QString filePath = BAIDUVOICE_PATH+"/"+_filename;
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    file.write(bytes);
    file.close();
    note->writeLine(Note::M_TTS,"文字转语音",QString("%1下载完成").arg(_filename));
    emit createSoundSuccess(_filename);
}
void TTS::_createToken(){
    _baiduTokenUrl= QString("https://openapi.baidu.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2").arg(_apikey).arg(_secretkey);
    QNetworkRequest request;
    request.setUrl(QUrl(_baiduTokenUrl));
    request.setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
    _netManager->get(request);
}
void TTS::_checkTokenIsAvilable(){
    QJsonObject baiduToken = config->getValue("baiduToken").toObject();
    quint32 expiresIn = baiduToken.value("expires_in").toVariant().toLongLong();
    quint64 lastTime = baiduToken.value("getTime").toVariant().toLongLong();
    quint64 nowTime = QDateTime::currentDateTime().toSecsSinceEpoch();
    if(expiresIn+lastTime-10000<=nowTime){
        _createToken();
    }else{}
}
void TTS::saveVoiceInfo(HMI::VOICE_INFO voiceInfo){
    QJsonObject voicePersonObj;
    voicePersonObj.insert("spd",voiceInfo.spd);
    voicePersonObj.insert("pit",voiceInfo.pit);
    voicePersonObj.insert("vol",voiceInfo.vol);
    voicePersonObj.insert("per",voiceInfo.per);
    config->setValue("voicePerson",voicePersonObj);
}
void TTS::setVoiceInfo(HMI::VOICE_INFO voiceInfo){
    _voiceInfo=voiceInfo;
}
void TTS::createSpeechFromText(QString text,QString filename){
    _checkTokenIsAvilable();//生成的时候检查一下是否token是否过期，过期自动更新
    _filename = filename;//保存的文件名
    QByteArray postArray;
    postArray.append(QString("tex=%1").arg(text.toUtf8().data()));
    postArray.append(QString("&tok=%1").arg(config->getValue("baiduToken").toObject().value("access_token").toString()));
    postArray.append(QString("&cuid=%1").arg(macAddress));
    postArray.append("&ctp=1");
    postArray.append("&lan=zh");
    postArray.append(QString("&spd=%1").arg(_voiceInfo.spd));
    postArray.append(QString("&pit=%1").arg(_voiceInfo.pit));
    postArray.append(QString("&vol=%1").arg(_voiceInfo.vol));
    postArray.append(QString("&per=%1").arg(_voiceInfo.per));
    postArray.append("&aue=6");//6为wav格式
    QNetworkRequest request;
    request.setUrl(QUrl(_baiduTtsUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postArray.size());
    _netManager->post(request,postArray);
    note->writeLine(Note::M_TTS,"文字转语音",QString("%1-%2").arg(filename).arg(text));
}
