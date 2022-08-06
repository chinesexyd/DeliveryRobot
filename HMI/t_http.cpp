#include "t_http.h"

HTTP::HTTP(QObject *parent) : QObject(parent){
//    QString baseUrl="http://"+(isTestMode?_baseUrl.test+":8080/mqService":_baseUrl.real);
    QString baseUrl="http://"+getHost();
//    note->writeLine(Note::M_HMI,"HTTP地址",baseUrl);
    _url.initCommunityConfigUrl =baseUrl+_exactUrl.initCommunityConfig;
    _url.advertListUrl          =baseUrl+_exactUrl.advertList;
    _url.carActionUrl           =baseUrl+_exactUrl.carAction;
    _url.startingUpUrl          =baseUrl+_exactUrl.startingUp;
    _url.uploadCarSysInfoUrl    =baseUrl+_exactUrl.uploadCarSysInfo;
    _url.sendSmsCodeUrl         =baseUrl+_exactUrl.sendSmsCode;
    _url.resetOrderUrl          =baseUrl+_exactUrl.resetOrder;
    _url.trackOrderUrl          =baseUrl+_exactUrl.trackOrder;
    _url.devWarningReportUrl    =baseUrl+_exactUrl.devWarningReport;
    _url.uploadLogUrl           =baseUrl+_exactUrl.uploadLog;
    _url.lockElevatorUrl        =baseUrl+_exactUrl.lockElevator;
    _url.unlockElevatorUrl      =baseUrl+_exactUrl.unlockElevator;
    _url.sendVoiceCodeUrl       =baseUrl+_exactUrl.sendVoiceCode;
    _url.checkSoftwareUrl       =baseUrl+_exactUrl.checkSoftware;
    _netManager = new QNetworkAccessManager(this);
    connect(_netManager,&QNetworkAccessManager::finished,this,&HTTP::finishedSlot);
    connect(timer,SIGNAL(timer100TimeoutSignal()),this,SLOT(timer100TimeoutSlot()));
}
HTTP::~HTTP(){}
QString HTTP::getHost(){
    QFile testFile(QDir::homePath()+"/DeliveryRobot/DeliveryRobot/Config/test");
    QFile previewFile(QDir::homePath()+"/DeliveryRobot/DeliveryRobot/Config/preview");
    if(testFile.exists()&&!previewFile.exists()){
        return _baseUrl.test;
    }else if(previewFile.exists()){
        return _baseUrl.preview;
    }else{
        return _baseUrl.real;
    }
}
void HTTP::isConnect(){
    QString host=getHost();
    QString pinip= "ping "+host+ " -c 1";
    QByteArray ba = pinip.toLatin1();
    int ret=system(pinip.toLatin1().data());
    if(ret!=0){
        emit pingHttpIsSuccessSignal(false);
    }else{
        emit pingHttpIsSuccessSignal(true);
    }
}
void HTTP::initCommunityConfig(){
    QJsonObject obj;
    obj.insert("cuy_id",commnuityInfo.cuyID);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.initCommunityConfigUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"初始化小区配置",bytes);
}
void HTTP::advertList(){
    QJsonObject obj;
    obj.insert("cuy_id",commnuityInfo.cuyID);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.advertListUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"小区广告列表",bytes);
}
void HTTP::carAction(HTTP::ACTION_POINT pointAction,HTTP::WORK_STATE workState){
    QJsonObject obj;
    obj.insert("car_id",commnuityInfo.carID);
    obj.insert("point_action",pointAction);
    obj.insert("work_state",workState);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.carActionUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"无人车调度执行",bytes);
}
void HTTP::startingUp(){
    QJsonObject obj;
    obj.insert("car_mac",macAddress);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.startingUpUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"开机接口",bytes);
}
void HTTP::uploadCarSysInfo(HTTP::PARAM_UploadCarSysInfo uploadCarSysInfo){ 
    QJsonObject obj;
    obj.insert("camera_model",uploadCarSysInfo.cameraModel);
    obj.insert("car_id",uploadCarSysInfo.carID);
    obj.insert("car_ip",uploadCarSysInfo.carIP);
    obj.insert("drive_model",uploadCarSysInfo.driveModel);
    obj.insert("emergency_model",uploadCarSysInfo.emergencyModel);
    obj.insert("imei",uploadCarSysInfo.imei);
    obj.insert("imu_model",uploadCarSysInfo.imuModel);
    obj.insert("laser_model",uploadCarSysInfo.larserModel);
    obj.insert("lower_version",uploadCarSysInfo.lowerVersion);
    obj.insert("operator_name",uploadCarSysInfo.operatorName);
    obj.insert("os_version",uploadCarSysInfo.osVersion);
    obj.insert("router_ip",uploadCarSysInfo.routerIP);
    obj.insert("router_mac",uploadCarSysInfo.routerMac);
    obj.insert("router_model",uploadCarSysInfo.routerModel);
    obj.insert("router_version",uploadCarSysInfo.routerVersion);
    obj.insert("sonar_model",uploadCarSysInfo.sonarModel);
    obj.insert("uccid_number",uploadCarSysInfo.uccidNumber);
    obj.insert("ui_version",uploadCarSysInfo.uiVersion);
    obj.insert("uu_id",uploadCarSysInfo.uuID);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.uploadCarSysInfoUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"上传小车系统信息",bytes);
}
void HTTP::sendSmsCode(QString code,QString mobile){
    _checkInfo["sendSmsCode"].isRequest=true;
    _checkInfo["sendSmsCode"].timeCount=0;
    _httpRequest.sendSmsCode_code=code;
    _httpRequest.sendSmsCode_mobile=mobile;

    QJsonObject obj;
    obj.insert("code",code);
    obj.insert("mobile",mobile);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.sendSmsCodeUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"发送短信验证码",bytes);
}
void HTTP::devWarningReport(HTTP::PARAM_DevWarningReport devWarningReport){
    QJsonObject obj;
    obj.insert("building_id",devWarningReport.buildingID);
    obj.insert("cuy_id",devWarningReport.cuyID);
    obj.insert("device_id",devWarningReport.deviceID);
    obj.insert("device_type",devWarningReport.deviceType);
    obj.insert("elevator_id",devWarningReport.elevatorID);
    obj.insert("error_code",devWarningReport.errorCode);
    obj.insert("error_remarks",devWarningReport.errorRemarks);
    obj.insert("floor_id",devWarningReport.floorID);
    obj.insert("logistics_id",devWarningReport.logisticsID);
    obj.insert("point_a",devWarningReport.point.angle);
    obj.insert("point_x",devWarningReport.point.x);
    obj.insert("point_y",devWarningReport.point.y);
    obj.insert("unit_id",devWarningReport.unitID);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.devWarningReportUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"设备警报上报",bytes);
}
void HTTP::uploadLog(qint64 timestamp,QString pictureName){
    QString dateStr = QDateTime::fromMSecsSinceEpoch(timestamp).toString("yyyy-MM-dd");
    if(_multiPart==nullptr){
        _multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    }else{
        delete _multiPart;
        _multiPart=new QHttpMultiPart(QHttpMultiPart::FormDataType);
    }

    QHttpPart part01;
    part01.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data;name=\"deviceId\";"));
    part01.setBody(QString::number(commnuityInfo.carID).toLatin1());
    _multiPart->append(part01);

    QHttpPart part02;
    part02.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data;name=\"deviceType\";"));
    part02.setBody(QString::number(1).toLatin1());//1代表小车
    _multiPart->append(part02);

    QHttpPart part03;
    part03.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data;name=\"timestamp\";"));
    part03.setBody(QString::number(timestamp).toLatin1());
    _multiPart->append(part03);

    QDir rosLogDir(ROS_LOG_PATH);
    QFile pictureFile(IMAGE_PATH+"/"+pictureName);
    QFile noteFile(QString("%1/%2.txt").arg(NOTE_PATH).arg(dateStr));

    QString zipPath = NOTE_PATH+QString("/%1.zip").arg(dateStr);
    QZipWriter writer(zipPath);
    if(rosLogDir.exists()){
        QString rosLogDate = QDateTime::fromMSecsSinceEpoch(timestamp).toString("yyyy-M-d");
        foreach(QString filename,rosLogDir.entryList(QDir::NoDotAndDotDot|QDir::Files)){
            if(filename.contains(rosLogDate)){
                QString filepath=ROS_LOG_PATH+"/"+filename;
                QFile file(filepath);
                file.open(QIODevice::ReadOnly);
                QByteArray rosBytes=file.readAll();
                file.close();
                writer.addFile(filename,rosBytes);
            }
        }
    }else{
        note->writeLine(Note::M_ROS_ERROR,"获取ROS日志",QString("获取ROS日志失败，找不到%1文件夹").arg(ROS_LOG_PATH));
    }
    //压缩拍照
    if(pictureFile.exists()){
        pictureFile.open(QIODevice::ReadOnly);
        QByteArray bytes=pictureFile.readAll();
        pictureFile.close();
        writer.addFile(pictureName,bytes);
    }else{
        note->writeLine(Note::M_ROS_ERROR,"相机拍照","拍照200ms后找不到图片，或许相机损坏。");
    }
    //压缩HMI的
    if(noteFile.exists()){
        noteFile.open(QIODevice::ReadOnly);
        QByteArray bytes = noteFile.readAll();
        noteFile.close();
        writer.addFile(QString("%1.txt").arg(dateStr),bytes);
    }
    writer.close();
    //索取压缩后数据流
    QFile zipFile(zipPath);
    zipFile.open(QIODevice::ReadOnly);
    QByteArray byte = zipFile.readAll();
    zipFile.close();
    QHttpPart part04;
    part04.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"file\";filename=\"%1\";").arg(QFileInfo(noteFile).fileName().replace("txt","zip")));
    part04.setBody(byte);
    _multiPart->append(part04);
    zipFile.remove();

    QNetworkRequest request;
    request.setUrl(QUrl(_url.uploadLogUrl));
    request.setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
    _multiPart->setBoundary(macAddress.toLatin1());
    request.setHeader(QNetworkRequest::ContentTypeHeader,QString("multipart/form-data;boundary=%1;").arg(macAddress));
    _netManager->post(request,_multiPart);
}
void HTTP::lockElevator(qint32 startFloor,qint32 targetFloor,qint64 unitID){
    _checkInfo["lockElevator"].isRequest=true;
    _checkInfo["lockElevator"].timeCount=0;
    _httpRequest.lockElevator_startFloor=startFloor;
    _httpRequest.lockElevator_targetFloor=targetFloor;
    _httpRequest.lockElevator_unitID=unitID;

    QJsonObject obj;
    obj.insert("start_floor",startFloor);
    obj.insert("target_floor",targetFloor);
    obj.insert("unit_id",unitID);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.lockElevatorUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"锁定电梯",bytes);
}
void HTTP::unlockElevator(QString elevatorMac){
    _checkInfo["unlockElevator"].isRequest=true;
    _checkInfo["unlockElevator"].timeCount=0;
    _httpRequest.unlockElevator_elevatorMac=elevatorMac;

    QJsonObject obj;
    obj.insert("elevator_mac",elevatorMac);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.unlockElevatorUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"解锁电梯",bytes);
}
void HTTP::sendVoiceCode(QString code,QString mobile){
    _checkInfo["sendVoiceCode"].isRequest=true;
    _checkInfo["sendVoiceCode"].timeCount=0;
    _httpRequest.sendVoiceCode_code=code;
    _httpRequest.sendVoiceCode_mobile=mobile;

    QJsonObject obj;
    obj.insert("code",code);
    obj.insert("mobile",mobile);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.sendVoiceCodeUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"发送语音验证码",bytes);
}
void HTTP::trackOrder(qint64 logisticsID,qint64 orderID,HTTP::ORDER_STATE orderState,LOGISTICS_STATE logisticsState){
    QJsonObject obj;
    obj.insert("logistics_id",logisticsID);
    obj.insert("order_id",orderID);
    obj.insert("order_state",orderState);
    obj.insert("logistics_state",logisticsState);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.trackOrderUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"更新订单状态",bytes);
}
void HTTP::resetOrder(qint32 carID,qint64 logisticsID,qint64 orderID){
    QJsonObject obj;
    obj.insert("car_id",carID);
    obj.insert("logistics_id",logisticsID);
    obj.insert("order_id",orderID);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    post(_url.resetOrderUrl,bytes);
    note->writeLine(Note::M_TO_HTTP,"重置订单状态",bytes);
}
void HTTP::checkSoftware(HTTP::SOFT_TYPE type){
    QJsonObject obj;
    QNetworkRequest request;
    request.setUrl(QUrl(_url.checkSoftwareUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    switch (type) {
    case HTTP::ST_HMI:
        request.setRawHeader("productCode",_softTag.hmi.toUtf8());
        obj.insert("productCode",_softTag.hmi);;
        break;
    case HTTP::ST_ROS:
        request.setRawHeader("productCode",_softTag.ros.toUtf8());
        obj.insert("productCode",_softTag.ros);
        break;
    case HTTP::ST_BAN:
        request.setRawHeader("productCode",_softTag.ban.toUtf8());
        obj.insert("productCode",_softTag.ban);
        break;
    default:
        break;
    }
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray bytes=doc.toJson(QJsonDocument::Compact);
    request.setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
    _netManager->post(request,bytes);
    note->writeLine(Note::M_TO_HTTP,"软件检查",bytes);
}
void HTTP::finishedSlot(QNetworkReply* reply){
    QNetworkReply::NetworkError error=reply->error();
    if(error==QNetworkReply::NoError){
        emit pingHttpIsSuccessSignal(true);
        QString replyUrl = reply->url().toString();
        if(replyUrl!=_url.checkSoftwareUrl){
            QByteArray bytes = reply->readAll();
            QJsonObject jsonObj=QJsonDocument::fromJson(bytes).object();
            if(replyUrl==_url.initCommunityConfigUrl){
                note->writeLine(Note::M_FROM_HTTP,"初始化小区配置",bytes);
                _initCommunityConfig(jsonObj);
            }else if(replyUrl==_url.advertListUrl){
                note->writeLine(Note::M_FROM_HTTP,"小区广告列表",bytes);
                _advertList(jsonObj);
            }else if(replyUrl==_url.carActionUrl){
                note->writeLine(Note::M_FROM_HTTP,"无人车调度执行",bytes);
                _carAction(jsonObj);
            }else if(replyUrl==_url.startingUpUrl){
                note->writeLine(Note::M_FROM_HTTP,"开机接口",bytes);
                _startingUp(jsonObj);
            }else if(replyUrl==_url.uploadCarSysInfoUrl){
                note->writeLine(Note::M_FROM_HTTP,"上传小车系统信息",bytes);
                _uploadCarSysInfo(jsonObj);
            }else if(replyUrl==_url.sendSmsCodeUrl){
                note->writeLine(Note::M_FROM_HTTP,"发送短信验证码",bytes);
                _sendSmsCode(jsonObj);
            }else if(replyUrl==_url.resetOrderUrl){
                note->writeLine(Note::M_FROM_HTTP,"重置订单状态",bytes);
                _resetOrder(jsonObj);
            }else if(replyUrl==_url.trackOrderUrl){
                note->writeLine(Note::M_FROM_HTTP,"更新订单状态",bytes);
                _trackOrder(jsonObj);
            }else if(replyUrl==_url.devWarningReportUrl){
                note->writeLine(Note::M_FROM_HTTP,"设备报警上报",bytes);
                _devWarningReport(jsonObj);
            }else if(replyUrl==_url.uploadLogUrl){
                note->writeLine(Note::M_FROM_HTTP,"日志上传",bytes);
                _uploadLog(jsonObj);
            }else if(replyUrl==_url.lockElevatorUrl){
                note->writeLine(Note::M_FROM_HTTP,"锁定电梯",bytes);
                _lockElevator(jsonObj);
            }else if(replyUrl==_url.unlockElevatorUrl){
                note->writeLine(Note::M_FROM_HTTP,"解锁电梯",bytes);
                _unlockElevator(jsonObj);
            }else if(replyUrl==_url.sendVoiceCodeUrl){
                note->writeLine(Note::M_FROM_HTTP,"发送短信验证码",bytes);
                _sendVoiceCode(jsonObj);
            }else{}
        }else if(replyUrl==_url.checkSoftwareUrl){
            _checkSoftware(reply);
        }
    }else{
        QString replyUrl = reply->url().toString();
        if(replyUrl==_url.unlockElevatorUrl){
            unlockElevator(_httpRequest.unlockElevator_elevatorMac);
        }else if(replyUrl==_url.lockElevatorUrl){
            lockElevator(_httpRequest.lockElevator_startFloor,_httpRequest.lockElevator_targetFloor,_httpRequest.lockElevator_unitID);
        }else if(replyUrl==_url.sendSmsCodeUrl){
            sendSmsCode(_httpRequest.sendSmsCode_code,_httpRequest.sendSmsCode_mobile);
        }else if(replyUrl==_url.sendVoiceCodeUrl){
            sendVoiceCode(_httpRequest.sendVoiceCode_code,_httpRequest.sendVoiceCode_mobile);
        }else{}
    }
    reply->abort();
    reply->deleteLater();
}
void HTTP::timer100TimeoutSlot(){
    QStringList keys=_checkInfo.keys();
    for(int i=0;i<keys.length();i++){
        QString key=keys[i];
        if(_checkInfo[key].isRequest){
            _checkInfo[key].timeCount++;
            if(_checkInfo[key].timeCount>_checkInfo[key].overtime){
                if(key=="sendSmsCode"){
                    sendSmsCode(_httpRequest.sendSmsCode_code,_httpRequest.sendSmsCode_mobile);
                }else if(key=="sendVoiceCode"){
                    sendVoiceCode(_httpRequest.sendSmsCode_code,_httpRequest.sendSmsCode_mobile);
                }else if(key=="lockElevator"){
                    lockElevator(_httpRequest.lockElevator_startFloor,_httpRequest.lockElevator_targetFloor,_httpRequest.lockElevator_unitID);
                }else if(key=="unlockElevator"){
                    unlockElevator(_httpRequest.unlockElevator_elevatorMac);
                }else{}
            }
        }
    }
}
void HTTP::_initCommunityConfig(QJsonObject obj){
    int code=obj.value("code").toInt();
    if(code==0){
        QJsonObject dataObj=obj.value("data").toObject();
        QJsonObject cuyInfo=dataObj.value("cuyInfo").toObject();
        initCommunityConfigInfo.cuyInfo.cuyID=cuyInfo.value("cuy_id").toInt();
        initCommunityConfigInfo.cuyInfo.cuyName=cuyInfo.value("cuy_name").toString();
        initCommunityConfigInfo.cuyInfo.cuyAddress=cuyInfo.value("cuy_address").toString();
        initCommunityConfigInfo.cuyInfo.provinceID=cuyInfo.value("province_id").toVariant().toLongLong();
        initCommunityConfigInfo.cuyInfo.cityID=cuyInfo.value("city_id").toVariant().toLongLong();
        initCommunityConfigInfo.cuyInfo.countyID=cuyInfo.value("county_id").toVariant().toLongLong();
        initCommunityConfigInfo.cuyInfo.districtName=cuyInfo.value("district_name").toString();
        initCommunityConfigInfo.cuyInfo.countyNumber=cuyInfo.value("county_number").toString();
        initCommunityConfigInfo.cuyInfo.latitude=cuyInfo.value("latitude").toDouble();
        initCommunityConfigInfo.cuyInfo.logitude=cuyInfo.value("logitude").toDouble();
        initCommunityConfigInfo.cuyInfo.facadeUrl=cuyInfo.value("facade_url").toString();
        initCommunityConfigInfo.cuyInfo.businessState=cuyInfo.value("business_state").toInt();
        initCommunityConfigInfo.cuyInfo.businessTime=cuyInfo.value("business_time").toString();
        initCommunityConfigInfo.cuyInfo.personName=cuyInfo.value("person_name").toString();
        initCommunityConfigInfo.cuyInfo.phoneNumber=cuyInfo.value("phone_number").toString();
        initCommunityConfigInfo.cuyInfo.operatorID=cuyInfo.value("operator_id").toInt();
        initCommunityConfigInfo.cuyInfo.mapFile=cuyInfo.value("map_file").toString();
        initCommunityConfigInfo.cuyInfo.locationFloor=cuyInfo.value("location_floor").toInt();
        initCommunityConfigInfo.cuyInfo.mapPath=cuyInfo.value("map_path").toString();
        QJsonObject initPoint=dataObj.value("initPoint").toObject();
        initCommunityConfigInfo.initPoint.pointID=initPoint.value("point_id").toInt();
        initCommunityConfigInfo.initPoint.cuyID=initPoint.value("cuy_id").toInt();
        initCommunityConfigInfo.initPoint.pointName=initPoint.value("point_name").toString();
        initCommunityConfigInfo.initPoint.pointType=initPoint.value("point_type").toInt();
        initCommunityConfigInfo.initPoint.xyaPoint.x=initPoint.value("point_x").toDouble();
        initCommunityConfigInfo.initPoint.xyaPoint.y=initPoint.value("point_y").toDouble();
        initCommunityConfigInfo.initPoint.xyaPoint.angle=initPoint.value("point_a").toDouble();
        QJsonObject buildingList=dataObj.value("buildingList").toObject();
        foreach (QString building, buildingList.keys()) {
            QJsonObject buildingObj=buildingList.value(building).toObject();
            QMap<QString,XYA_POINT> map;
            foreach(QString unid,buildingObj.keys()){
                QJsonObject unidObj=buildingObj.value(unid).toObject();
                XYA_POINT xyaPoint;
                xyaPoint.x=unidObj.value("x").toDouble();
                xyaPoint.x=unidObj.value("y").toDouble();
                xyaPoint.x=unidObj.value("a").toDouble();
                map.insert(unid,xyaPoint);
            }
            initCommunityConfigInfo.buildingList.insert(building,map);
        }
        emit initCommunityConfigIsSuccessSignal(true);
        QJsonDocument doc;
        cuyInfo.insert("password",commnuityInfo.carPassword);
        cuyInfo.insert("carName",commnuityInfo.carName);
        cuyInfo.insert("macAddress",macAddress);
        doc.setObject(cuyInfo);
        QByteArray toByte = doc.toJson(QJsonDocument::Indented);
        QFile file(ROBOTINFO_PATH);
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        file.write(toByte);
        file.close();
    }else{
        emit initCommunityConfigIsSuccessSignal(false);
    }
}
void HTTP::_advertList(QJsonObject obj){
    int code=obj.value("code").toInt();
    if(code==0){
        //需要保存到配置文件
        QJsonArray data=obj.value("data").toArray();
        config->setValue("advertWordList",data);
        QStringList advertList;
        for(int i=0;i<data.size();i++){
            advertList.append(data[i].toString());
        }
        emit advertListIsSucessSignal(true);
        emit advertListSignal(advertList);
    }else{
        emit advertListIsSucessSignal(false);
    }
}
void HTTP::_carAction(QJsonObject obj){
    int code=obj.value("code").toInt();
    if(code==0){
        QJsonObject data=obj.value("data").toObject();
        if(!data.isEmpty()){
            POINT_INFO pointInfo;
            pointInfo.pointID=data.value("point_id").toInt();
            pointInfo.cuyID=data.value("cuy_id").toInt();
            pointInfo.pointName=data.value("point_name").toString();
            pointInfo.pointType=data.value("point_type").toInt();
            pointInfo.xyaPoint.x=data.value("point_x").toDouble();
            pointInfo.xyaPoint.y=data.value("point_y").toDouble();
            pointInfo.xyaPoint.angle=data.value("point_a").toDouble();
            emit carActionSignal(pointInfo);
        }
        emit carActionIsSucessSignal(true);
    }else{
        emit carActionIsSucessSignal(false);
    }
}
void HTTP::_startingUp(QJsonObject obj){
    int code=obj.value("code").toInt();
    if(code==HTTP::CV_NORMAL){
        QJsonObject dataObj=obj.value("data").toObject();
        commnuityInfo.carID=dataObj.value("car_id").toInt();
        commnuityInfo.carNumber=dataObj.value("car_number").toString();
        commnuityInfo.carMac=dataObj.value("car_mac").toString();
        commnuityInfo.carName=dataObj.value("car_name").toString();
        commnuityInfo.carType=dataObj.value("car_type").toInt();
        commnuityInfo.carState=dataObj.value("car_state").toInt();
        commnuityInfo.carPassword=dataObj.value("car_password").toString();
        config->setValue("password",dataObj.value("car_password").toString());
        commnuityInfo.cuyID=dataObj.value("cuy_id").toVariant().toLongLong();
        commnuityInfo.deviceState=dataObj.value("device_state").toInt();
        commnuityInfo.isSucess=true;
        emit startingUpSignal(HTTP::CV_NORMAL);
    }else if(code==HTTP::CV_DISABLE){
        commnuityInfo.isSucess=false;
        emit startingUpSignal(HTTP::CV_DISABLE);
    }else if(code==HTTP::CV_UNAUTHORIZED){
        commnuityInfo.isSucess=false;
        emit startingUpSignal(HTTP::CV_UNAUTHORIZED);
    }else if(code==HTTP::CV_UNBIND){
        commnuityInfo.isSucess=false;
        emit startingUpSignal(HTTP::CV_UNBIND);
    }else{
        commnuityInfo.isSucess=false;
    }
}
void HTTP::_uploadCarSysInfo(QJsonObject obj){
    int code=obj.value("code").toInt();
    if(code==0){
        emit uploadCarSysInfoIsSuccessSignal(true);
    }else{
        emit uploadCarSysInfoIsSuccessSignal(false);
    }
}
void HTTP::_sendSmsCode(QJsonObject obj){
    _checkInfo["sendSmsCode"].isRequest=false;
    _checkInfo["sendSmsCode"].timeCount=0;

    int code=obj.value("code").toInt();
    if(code==0){
        emit sendSmsCodeIsSuccessSignal(true);
    }else{
        emit sendSmsCodeIsSuccessSignal(false);
    }
}
void HTTP::_devWarningReport(QJsonObject obj){
    int code=obj.value("code").toInt();
    if(code==0){
        emit devWarningReportIsSuccessSignal(true);
    }else{
        emit devWarningReportIsSuccessSignal(false);
    }
}
void HTTP::_uploadLog(QJsonObject obj){
    int code=obj.value("code").toInt();
    if(code==0){
        uploadLogIsSuccessSignal(true);
    }else{
        uploadLogIsSuccessSignal(false);
    }
}
void HTTP::_lockElevator(QJsonObject obj){
    _checkInfo["lockElevator"].isRequest=false;
    _checkInfo["lockElevator"].timeCount=0;

    emit lockElevatorSignal(obj);
}
void HTTP::_unlockElevator(QJsonObject obj){
    _checkInfo["unlockElevator"].isRequest=false;
    _checkInfo["unlockElevator"].timeCount=0;

    int code=obj.value("code").toInt();
    if(code==0){
        emit unlockElevatorIsSucessSignal(true);
    }else{
        emit unlockElevatorIsSucessSignal(false);
    }
}
void HTTP::_sendVoiceCode(QJsonObject obj){
    _checkInfo["sendVoiceCode"].isRequest=false;
    _checkInfo["sendVoiceCode"].timeCount=0;

    int code=obj.value("code").toInt();
    if(code==0){
        emit sendVoiceCodeIsSuccessSignal(true);
    }else{
        emit sendVoiceCodeIsSuccessSignal(false);
    }
}
void HTTP::_trackOrder(QJsonObject obj){
    int code=obj.value("code").toInt();
    if(code==0){
        emit trackOrderIsSuccessSignal(true);
    }else{
        emit trackOrderIsSuccessSignal(false);
    }
}
void HTTP::_resetOrder(QJsonObject obj){
    int code=obj.value("code").toInt();
    emit resetOrderIsSuccessSignal(code);
}
void HTTP::_checkSoftware(QNetworkReply* reply){
    QByteArray bytes = reply->readAll();
    QJsonObject jsonObj=QJsonDocument::fromJson(bytes).object();
    int code=jsonObj.value("code").toInt();
    QString productCode=reply->request().rawHeader("productCode");
    if(code==0){
        QJsonObject data=jsonObj.value("data").toObject();
        SoftwareFrame::CLOUD cloud;
        cloud.versionName=data.value("versionName").toString();
        cloud.versionCode=data.value("versionCode").toInt();
        cloud.url=data.value("url").toString();
        cloud.remark=data.value("remark").toString();
        cloud.enforced=data.value("enforced").toInt();
        QString url = data.value("url").toString();
        QStringList urlList = url.split("/");
        QString zipName = urlList.last();
        if(productCode==_softTag.hmi){
            cloud.savePath=UPPERSOFT_PATH+"/"+zipName;;
            note->writeLine(Note::M_FROM_HTTP,"上位机",bytes);
            emit checkSoftwareSignal(HTTP::ST_HMI,cloud);
        }else if(productCode==_softTag.ros){
            cloud.savePath = MIDDLESOFT_PATH+"/"+zipName;
            note->writeLine(Note::M_FROM_HTTP,"中位机",bytes);
            emit checkSoftwareSignal(HTTP::ST_ROS,cloud);
        }else if(productCode==_softTag.ban){
            cloud.savePath = LOWERSOFT_PATH+"/"+zipName;
            note->writeLine(Note::M_FROM_HTTP,"下位机",bytes);
            emit checkSoftwareSignal(HTTP::ST_BAN,cloud);
        }
    }else{}
}
void HTTP::post(QString url,QByteArray bytes){
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
    _netManager->post(request,bytes);
}
