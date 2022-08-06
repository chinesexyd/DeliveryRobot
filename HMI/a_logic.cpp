#include "a_logic.h"

/*@@@@@@@@@@@@@@@@@@@@*/
HTTP* http=new HTTP();
MQServer* mq=new MQServer();
Camera* camera=new Camera();
TTS* tts=new TTS();
Voice* voice=new Voice();
OBS* obs=new OBS();
/*@@@@@@@@@@@检查网络连接状态@@@@@@@@@@@*/
CheckNetWorkStateThread::CheckNetWorkStateThread(QObject* parent):QThread(parent){
    connect(timer,SIGNAL(timer1000TimeoutSignal()),this,SLOT(timerTimeoutSlot()));
}
void CheckNetWorkStateThread::run(){
    ros->isConnect();
    http->isConnect();
}
void CheckNetWorkStateThread::timerTimeoutSlot(){
    _timerCount++;
    if(_timerCount==60){
        _timerCount=0;
        start();
    }
}
/*@@@@@@@@@@@@@@@@@@@@*/
Logic::Logic():QObject(){
    _receiveGoodsTimer=new QTimer(this);
    connect(_receiveGoodsTimer,SIGNAL(timeout()),this,SLOT(receiveGoodsTimerSlot()));
    _checkNetWorkStateThread=new CheckNetWorkStateThread(this);
}
//连接信号
void Logic::connectSignals(){
    connect(ros,SIGNAL(loadMapSignal(ROS::RETURN_STATE)),this,SLOT(loadMapSlot(ROS::RETURN_STATE)));
    connect(ros,SIGNAL(initialPoseSignal(ROS::RETURN_STATE)),this,SLOT(initialPoseSlot(ROS::RETURN_STATE)));
    connect(ros,SIGNAL(moveGoalSignal(ROS::RETURN_STATE,QString)),this,SLOT(moveGoalSlot(ROS::RETURN_STATE,QString)));
    connect(ros,SIGNAL(moveGoalFileSignal(ROS::RETURN_STATE)),this,SLOT(moveGoalFileSlot(ROS::RETURN_STATE)));
    connect(ros,SIGNAL(rideElevatorSignal(ROS::RETURN_ELEVATOR_STATE)),this,SLOT(rideElevatorSlot(ROS::RETURN_ELEVATOR_STATE)));
    connect(ros,SIGNAL(mapFileChangedSignal(QString)),this,SLOT(mapFileChangedSlot(QString)));
    connect(ros,SIGNAL(receiveRosErrorSignal(quint32,QString)),this,SLOT(receiveRosErrorSlot(quint32,QString)));
    connect(ros,SIGNAL(updateMapSignal(ROS::ROS::UPDATE_MAP_ENUM)),this,SLOT(updateMapSlot(ROS::ROS::UPDATE_MAP_ENUM)));
    connect(ros,SIGNAL(receiveUDPDataSignal()),this,SLOT(receiveUDPDataSlot()));
    connect(mq,SIGNAL(ackRobotStateSignal()),this,SLOT(relayRobotStateSlot()));
    connect(mq,SIGNAL(receiveOrderInfoSignal(QJsonObject)),this,SLOT(dealOrderInfoSlot(QJsonObject)));
    connect(mq,SIGNAL(ackNoteSignal(quint64)),this,SLOT(relayNoteSlot(quint64)));
    connect(mq,SIGNAL(receivePickupCodeSignal(QJsonObject)),this,SLOT(receivePickupCodeSlot(QJsonObject)));
    connect(http,SIGNAL(initCommunityConfigIsSuccessSignal(bool)),this,SLOT(initCommunityConfigIsSuccessSlot(bool)));
    connect(http,SIGNAL(carActionSignal(HTTP::POINT_INFO)),this,SLOT(carActionSlot(HTTP::POINT_INFO)));
    connect(http,SIGNAL(lockElevatorSignal(QJsonObject)),this,SLOT(lockElevatorSlot(QJsonObject)));
    connect(http,SIGNAL(unlockElevatorIsSucessSignal(bool)),this,SLOT(unlockElevatorIsSucessSlot(bool)));
    connect(http,SIGNAL(trackOrderIsSuccessSignal(bool)),this,SLOT(trackOrderIsSuccessSlot(bool)));
    connect(http,SIGNAL(resetOrderIsSuccessSignal(qint32)),this,SLOT(resetOrderIsSuccessSlot(qint32)));
}
//创建文件夹
void Logic::makeDirs(){
    QDir baiduVoiceDir(BAIDUVOICE_PATH);
    if(!baiduVoiceDir.exists()){
        baiduVoiceDir.mkpath(BAIDUVOICE_PATH);
    }
    QDir noteDir(NOTE_PATH);
    if(!noteDir.exists()){
        noteDir.mkpath(NOTE_PATH);
    }
    QDir imageDir(IMAGE_PATH);
    if(!imageDir.exists()){
        imageDir.mkpath(IMAGE_PATH);
    }
    QDir vedioDir(VIDEO_PATH);
    if(!vedioDir.exists()){
        vedioDir.mkpath(VIDEO_PATH);
    }
    QDir upperSoftDir(UPPERSOFT_PATH);
    if(!upperSoftDir.exists()){
        upperSoftDir.mkpath(UPPERSOFT_PATH);
    }
    QDir middleSoftDir(MIDDLESOFT_PATH);
    if(!middleSoftDir.exists()){
        middleSoftDir.mkpath(MIDDLESOFT_PATH);
    }
    QDir lowerSoftDir(LOWERSOFT_PATH);
    if(!lowerSoftDir.exists()){
        lowerSoftDir.mkpath(LOWERSOFT_PATH);
    }
}
//初次启动的准备工作
void Logic::firstStart(){
    note->writeLine(Note::M_HMI,"开机","开机准备工作");
    note->removeFrontFile();
    connectSignals();                                   //连接信号
    http->isConnect();                                  //是否能连上平台
    mq->openServer();                                   //开启MQ服务
    ros->tryConnect();                                  //连接ROS
    makeDirs();                                         //创建文件夹
    workState=HTTP::WS_UNINIT;
}
//初始化
void Logic::initRobot(){
    if(!logicParams.isInit){
        http->initCommunityConfig();
        logicParams.isInit=true;
    }else{
        emit hmi->sendLogicTextSignal("不可重复初始化");
    }
}
//开始送货
void Logic::startSendGoods(){
    emit hmi->setPageSignal(HMI::BLINK);
    orderState=HTTP::OS_DELIVER_GOODS;
    workState=HTTP::WS_SENDING_GOODS;
    http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,orderState,HTTP::LS_ORDER_START);
    camera->startRecorder(QString("%1-%2-%3").arg(orderInfo.addressInfo.buildingNum).arg(orderInfo.addressInfo.unitNum).arg(orderInfo.addressInfo.floorNumber));
    emit hmi->sendLogicTextSignal("靠右行驶（送货中）");
    ros->closeDoor();
    ros->gotoBuilding("go",orderInfo.addressInfo.goPathFlag);
    http->carAction(HTTP::AP_WAIT,workState);
    voice->playRoundSound(S_HIGH,"startSendGoods.wav",1);
    startUpdateMap();//开始更新地图
}
//检查验证码输入正确与否
void Logic::checkMsgCode(QString code){
    if(code==orderInfo.pickupCode){
        logicParams.codeTrueTimerCount=logicParams.timerCount;
        logicParams.isCodeTrue=true;
        note->writeLine(Note::M_HMI,"用户输入验证码",QString("验证码正确，取货验证码为：%1，用户输入为：%2").arg(orderInfo.pickupCode,code));
        emit hmi->sendLogicTextSignal("验证码输入正确");
        ros->openDoor();//打开车门
        voice->playRoundSound(S_HIGH,"codeTrue.wav",1);
        emit hmi->setPageSignal(HMI::USER_RECEIVE);//切换接收货品确认页面
        orderState=HTTP::OS_FINISHED_TAKING_GOODS;
        http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,orderState,HTTP::LS_RETURN);
    }else{
        logicParams.isCodeTrue=false;
        note->writeLine(Note::M_HMI_ERROR,"用户输入验证码",QString("验证码不正确，取货验证码为：%1，用户输入为：%2").arg(orderInfo.pickupCode,code));
        voice->playRoundSound(S_HIGH,"codeFalse.wav",1);
        emit hmi->sendLogicTextSignal("验证码输入错误");
    }
}
//确认收货
void Logic::confirmGoods(){
    if(workState==HTTP::WS_SENDING_GOODS){
        ros->closeDoor();
        emit hmi->sendLogicTextSignal("确认收货,返航");
        _receiveGoodsTimer->stop();
        workState=HTTP::WS_RETURN;//小车切换为返航状态
        emit hmi->setPageSignal(HMI::BLINK);//切换为眨眼界面
        http->lockElevator(orderInfo.addressInfo.floorNumber,http->initCommunityConfigInfo.cuyInfo.locationFloor,orderInfo.addressInfo.unitID);//申请返航电梯
        ros->gotoPoint(logicParams.EWEP_BACK,logicParams.pointTag.CEW);//去电梯避让点
        voice->playRoundSound(S_HIGH,"receivedGoods.wav",1);
    }else{}
}
//再次送货
void Logic::againSendGoods(){
    emit hmi->setPageSignal(HMI::BLINK);
    orderState=HTTP::OS_DELIVER_GOODS;//订单发货中
    workState=HTTP::WS_SENDING_GOODS;//工作状态切换为送货中状态
    http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,orderState,HTTP::LS_ORDER_START);
    camera->startRecorder(QString("%1-%2-%3").arg(orderInfo.addressInfo.buildingNum).arg(orderInfo.addressInfo.unitNum).arg(orderInfo.addressInfo.floorNumber));
    emit hmi->sendLogicTextSignal("靠右行驶（送货中）");
    ros->closeDoor();//关闭车门
    ros->gotoBuilding("go",orderInfo.addressInfo.goPathFlag);
    http->carAction(HTTP::AP_WAIT,workState);
    voice->playRoundSound(S_HIGH,"startSendGoods.wav",1);
    startUpdateMap();//开始更新地图
}
//取消订单
void Logic::cancelOrder(){
    http->resetOrder(http->commnuityInfo.carID,orderInfo.logisticsID,orderInfo.orderID);
}
//手动完成
void Logic::resetOrderState(){
    workState=HTTP::WS_UNUSED;
    hmi->sendLogicTextSignal("等待新订单（滞留已重置）");
    http->carAction(HTTP::AP_WAIT,workState);
    http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,orderState,HTTP::LS_FINISH);
    hmi->setPageSignal(HMI::BLINK);
}
//退出软件
void Logic::exitApp(){
    if(logicParams.isLockElevator){
        if(workState==HTTP::WS_SENDING_GOODS){
            http->unlockElevator(logicParams.EMacGo);//释放电梯
        }else if(workState==HTTP::WS_RETURN){
            http->unlockElevator(logicParams.EMacBack);//释放电梯
        }
    }
    emit hmi->exitSignal();
}
//加载地图的反馈
void Logic::loadMapSlot(ROS::RETURN_STATE state){
    switch (state) {
    case ROS::RS_SUCEESS:
        ros->initPoint(http->initCommunityConfigInfo.initPoint.xyaPoint);
        emit hmi->sendLogicTextSignal("初始化地图信息，初始化点");
        break;
    case ROS::RS_FAIL:
        emit hmi->sendLogicTextSignal("初始化地图失败");
        break;
    default:
        emit hmi->sendLogicTextSignal("正在初始化地图");
        break;
    }
}
//初始化点应答
void Logic::initialPoseSlot(ROS::RETURN_STATE state){
    switch (state) {
    case ROS::RS_SUCEESS:
        emit hmi->sendLogicTextSignal("初始化点成功，获取等待点");
        http->carAction(HTTP::AP_INIT,workState);
        emit hmi->setPageSignal(HMI::BLINK);
        break;
    case ROS::RS_FAIL:
        emit hmi->sendLogicTextSignal("初始化点失败");
        break;
    default:
        emit hmi->sendLogicTextSignal("正在初始化点");
        break;
    }
}
//前往目标点应答
void Logic::moveGoalSlot(ROS::RETURN_STATE state,QString tag){
    switch (state){
    case ROS::RS_SUCEESS:
        // 开机前往避让点成功 等待订单
        if(tag ==logicParams.pointTag.initWait||tag==logicParams.pointTag.returnWait){
            //未初始化
            if(workState==HTTP::WS_UNINIT){
                workState=HTTP::WS_UNUSED; //切换为闲置状态/ /由于MQ会频繁索要小车状态，所以这里不用对前往避让点等待订单进行处理
                emit hmi->sendLogicTextSignal("等待新订单（初始化完成）");
                voice->playRoundSound(S_MIDDLE,"init.wav",1);
                http->carAction(HTTP::AP_WAIT,workState);
            //滞留返回
            }else if(workState==HTTP::WS_RETURN&&orderState==HTTP::OS_DETENTION){
                emit hmi->setPageSignal(HMI::MANAGER_LOGIN);//管理员输入密码页面
                emit hmi->sendLogicTextSignal("等待运维人员处理订单");
                http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,orderState,HTTP::LS_RETURNING);
                voice->playRoundSound(S_HIGH,"retentionGoods.wav",1);
                stopUpdateMap();//结束更新地图
            //已完成返回
            }else if(workState==HTTP::WS_RETURN&&orderState==HTTP::OS_FINISHED_TAKING_GOODS){
                workState=HTTP::WS_UNUSED;//切换为闲置状态
                http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,orderState,HTTP::LS_FINISH);
                emit hmi->sendLogicTextSignal("等待新订单（已完成旧订单）");
                camera->stopRecorder();//结束录像
                stopUpdateMap();//结束更新地图
            }else{}
        //抵达地下室电梯避让点
        }else if(tag == logicParams.pointTag.SEW){
            logicParams.isAWEPGo=true;//送货中到达电梯避让点，不用处理，电梯会一直申请
            emit hmi->sendLogicTextSignal("抵达电梯避让点（送货）");
        //抵达用户面前
        }else if(tag == logicParams.pointTag.custormer){
        //返航到达电梯避让点
        }else if(tag == logicParams.pointTag.CEW){
            emit hmi->sendLogicTextSignal("抵达电梯避让点（返航）");
            logicParams.isAWEPBack=true;//返航到达电梯避让点，不用处理，电梯会一直申请
        }else{}
        break;
    case ROS::RS_RUNNING:
        if(tag==logicParams.pointTag.SEW){
            emit hmi->sendLogicTextSignal("前往电梯避让点中（送货）");
            logicParams.isAWEPGo=false;//未到达地下室电梯避让点
        }else if(tag==logicParams.pointTag.CEW){
            emit hmi->sendLogicTextSignal("前往电梯避让点中（返航）");
            logicParams.isAWEPBack=false;//未到达用户层电梯避让点
        }else{}
        break;
    default:
        break;
    }
}
//靠右行驶应答
void Logic::moveGoalFileSlot(ROS::RETURN_STATE state){
    switch (state) {
    case ROS::RS_SUCEESS:
        //如果为送货中
        if(workState==HTTP::WS_SENDING_GOODS){
            //申请电梯的同时去避让点
            http->lockElevator(http->initCommunityConfigInfo.cuyInfo.locationFloor,orderInfo.addressInfo.floorNumber,orderInfo.addressInfo.unitID);//申请电梯
            ros->gotoPoint(http->initCommunityConfigInfo.buildingList.value(orderInfo.addressInfo.buildingNum).value(orderInfo.addressInfo.unitNum),logicParams.pointTag.SEW);//前往避让点
            emit hmi->sendLogicTextSignal("申请电梯，前往电梯避让点（送货中）");
        //如果为返航中或退货返航中
        }else if(workState==HTTP::WS_RETURN){
            emit hmi->sendLogicTextSignal("前往避让点（返航）");
            ros->gotoPoint(logicParams.savePoint,logicParams.pointTag.returnWait);
        }else{}
        break;
    case ROS::RS_FAIL:
        if(workState==HTTP::WS_SENDING_GOODS){
            emit hmi->sendLogicTextSignal("靠右行驶失败（送货）");
        }else if(workState==HTTP::WS_RETURN){
            emit hmi->sendLogicTextSignal("靠右行驶失败（返航）");
        }
        break;
    default:
        if(workState==HTTP::WS_SENDING_GOODS){
            emit hmi->sendLogicTextSignal("靠右行驶中（送货）");
        }else if(workState==HTTP::WS_RETURN){
            emit hmi->sendLogicTextSignal("靠右行驶中（返航）");
        }
        break;
    }
}
//ros告知出电梯
void Logic::rideElevatorSlot(ROS::RETURN_ELEVATOR_STATE state){
    switch (state) {
    case ROS::ELE_SUCEESS://乘坐电梯成功了
        logicParams.elevatorID=0;
        //抵达用户层
        if(workState==HTTP::WS_SENDING_GOODS){
            stopUpdateMap();//结束更新地图
            emit hmi->sendLogicTextSignal("发送验证码短信，释放电梯，前往用户门口");
            http->sendSmsCode(orderInfo.pickupCode,orderInfo.addressInfo.consigneePhone);//发送验证码短信
            http->unlockElevator(logicParams.EMacGo);//释放电梯
            ros->gotoPoint(orderInfo.addressInfo.point,logicParams.pointTag.custormer);//前往用户坐标
            emit hmi->setPageSignal(HMI::USER_LOGIN);//切换为用户输入验证码的界面
            _receiveGoodsTimer->start(1000);
            logicParams.timerCount=0;//计时清为0
            //出电梯即等待取货
            emit hmi->sendLogicTextSignal("等待取货");
            orderState=HTTP::OS_WAITING_FOR_TAKE_GOODS;//订单状态到用户家门口,等待取货
            http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,orderState,HTTP::LS_ORDER_SENDING);
        //抵达了地下室
        }else if(workState==HTTP::WS_RETURN){
            http->unlockElevator(logicParams.EMacBack);//释放电梯
            ros->gotoBuilding("back",orderInfo.addressInfo.goPathFlag);//多点靠右导航返回
            logicParams.isSavePoint=true;//保存点
            http->carAction(HTTP::AP_WAY,workState); //避让点来源于服务器，暂存避让点
            emit hmi->sendLogicTextSignal("释放电梯，靠右行驶（返航）");
            startUpdateMap();//开始更新地图
        }else{}
        break;
    case ROS::ELE_FAIL://乘坐电梯失败了
        emit hmi->sendLogicTextSignal("乘坐电梯失败");
        break;
    case ROS::ELE_DAODIANTIKOU://到电梯口了
        emit hmi->sendLogicTextSignal("抵达电梯口");
        if(workState==HTTP::WS_SENDING_GOODS){
            orderState=HTTP::OS_ELEVATOR_ENTRANCE;
            http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,orderState,HTTP::LS_ORDER_SENDING);
        }else{}
        break;
    case ROS::ELE_DENGDAIDIANTIDAODA_IN://
        emit hmi->sendLogicTextSignal("等待电梯到达（进）");
        break;
    case ROS::ELE_SAOMIAODIANTIMEN_IN:
        emit hmi->sendLogicTextSignal("扫描电梯（进）");
        break;
    case ROS::ELE_JINRUDIANTI:
        emit hmi->sendLogicTextSignal("进入电梯");
        voice->playRoundSound(S_HIGH,"elevatorGiveWayIn.wav",3);
        if(workState==HTTP::WS_SENDING_GOODS){   
            orderState=HTTP::OS_IN_ELEVATOR;
            http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,orderState,HTTP::LS_ORDER_SENDING);
        }else{}
        break;
    case ROS::ELE_QIEHUANLOUCENG:
        emit hmi->sendLogicTextSignal("切换楼层");
        if(config->getValue("isUsingElevatorAdvert").toBool()){
            voice->playRoundAdvert(3);//三遍广告语
        }
        break;
    case ROS::ELE_DENGDAIDIANTIDAODA_OUT:
        emit hmi->sendLogicTextSignal("等待电梯到达（出）");
        break;
    case ROS::ELE_SAOMIAODIANTIMEN_OUT:
        emit hmi->sendLogicTextSignal("扫描电梯（出）");
        voice->playRoundSound(S_HIGH,config->numberMap[logic->orderInfo.addressInfo.floorNumber]+".wav",1);
        break;
    case ROS::ELE_CHUDIANTI:
        emit hmi->sendLogicTextSignal("出电梯");
        voice->playRoundSound(S_HIGH,"elevatorGiveWayOut.wav",3);
        break;
    case ROS::ELE_KONGIANBUZU:
        emit hmi->sendLogicTextSignal("空间不足，等待下一班电梯");
        break;
    default:
        if(workState==HTTP::WS_SENDING_GOODS){
            stopUpdateMap();//结束更新地图
        }else if(workState==HTTP::WS_RETURN){
            startUpdateMap();//开始更新地图
        }
        break;
    }
}
//地图路径发生改变
void Logic::mapFileChangedSlot(QString){
    if(logicParams.currentFloorID==0){
        logicParams.currentFloorID=orderInfo.addressInfo.floorID;
    }else if(logicParams.currentFloorID==orderInfo.addressInfo.floorID){
        logicParams.currentFloorID=0;
    }else{}
}
//ros报错
void Logic::receiveRosErrorSlot(quint32 errorCode,QString errorRemarks){
    if(errorCode==2004||errorCode==2005){
        voice->playRoundSound(S_HIGH,"dontMove.wav",1);
    }
    if(errorCode!=1){
        HTTP::PARAM_DevWarningReport report;
        report.errorCode=QString::number(errorCode);
        report.errorRemarks=errorRemarks;
        report.cuyID=http->commnuityInfo.cuyID;
        report.point=ros->robotStateValue.point;
        report.deviceID=http->commnuityInfo.carID;
        report.deviceType=1;//设备类型：1无人车、2收银机、3梯控
        if(workState!=HTTP::WS_UNINIT||workState!=HTTP::WS_UNUSED){
            report.unitID=orderInfo.addressInfo.unitID;
            report.floorID=orderInfo.addressInfo.floorID;
            report.buildingID=orderInfo.addressInfo.buildingID;
            report.logisticsID=orderInfo.logisticsID;
        }
        if(orderState==HTTP::OS_IN_ELEVATOR){
            report.elevatorID=logicParams.elevatorID;
        }
        http->devWarningReport(report);
    }
}
//更新地图
void Logic::updateMapSlot(ROS::ROS::UPDATE_MAP_ENUM type){
    switch (type) {
    case ROS::UPDATE_MAP_START:
        emit hmi->sendLogicTextSignal("开启地图更新");
        break;
    case ROS::UPDATE_MAP_SAVE:
        emit hmi->sendLogicTextSignal("保存地图更新");
        if(workState==HTTP::WS_SENDING_GOODS){
            if(logicParams.canUpdateMap){
                Sleep(3000);
                obs->put(orderInfo.orderID,orderInfo.addressInfo.goPathFlag,"GO");//对比上传图片区别
            }else{
                note->writeLine(Note::M_HMI,"上传地图更新","失败，或未开启或中途出现错误。");
            }
        }else{
            if(logicParams.canUpdateMap){
                Sleep(3000);
                obs->put(orderInfo.orderID,orderInfo.addressInfo.goPathFlag,"BACK");//对比上传图片区别
            }else{
                note->writeLine(Note::M_HMI,"上传地图更新","失败，或未开启或中途出现错误。");
            }
        }
        break;
    case ROS::UPDATE_MAP_RELOAD:
        emit hmi->sendLogicTextSignal("重载地图更新");
        break;
    case ROS::UPDATE_MAP_UPDATE:
        emit hmi->sendLogicTextSignal("更新地图更新");
        break;
    case ROS::UPDATE_MAP_STOP:
        emit hmi->sendLogicTextSignal("结束地图更新");
        break;
    default:
        break;
    }
}
//回应MQ索要小车数据
void Logic::relayRobotStateSlot(){
    QJsonDocument robotDoc;
    QJsonObject robotObj;
    robotObj.insert("car_id",http->commnuityInfo.carID);
    robotObj.insert("car_mac",macAddress);
    robotObj.insert("is_charge",ros->robotStateValue.charge?1:0);
    robotObj.insert("mileage",ros->robotStateValue.mils);
    robotObj.insert("electric",ros->robotStateValue.mah);
    robotObj.insert("voltage",ros->robotStateValue.vol);
    robotObj.insert("current",ros->robotStateValue.cur);
    robotObj.insert("dbm",0);                                               //信号强度
    robotObj.insert("laser_state",1);                                       //激光是否异常 0故障 1正常
    robotObj.insert("imu_state",ros->robotStateValue.imu?0:1);
    robotObj.insert("drive_state",1);//驱动是否异常
//    robotObj.insert("sonar1_state",ros->robotStateValue.sonar?0:1);
//    robotObj.insert("sonar2_state",ros->robotStateValue.sonar?0:1);
//    robotObj.insert("sonar3_state",ros->robotStateValue.sonar?0:1);
//    robotObj.insert("sonar4_state",ros->robotStateValue.sonar?0:1);
    robotObj.insert("sonar1_state",1);
    robotObj.insert("sonar2_state",1);
    robotObj.insert("sonar3_state",1);
    robotObj.insert("sonar4_state",1);
    robotObj.insert("camera_state",1);//摄像机
    robotObj.insert("emergency_state",ros->robotStateValue.emergency?1:0);
    robotObj.insert("collide_state",ros->robotStateValue.collision?0:1);
    robotObj.insert("pushrod_state",1);
    robotObj.insert("servo_state",1);
    if(workState==HTTP::WS_UNINIT||workState==HTTP::WS_UNUSED){
        robotObj.insert("building_id",0);
        robotObj.insert("unit_id",0);
    }else{
        robotObj.insert("building_id",orderInfo.addressInfo.buildingID);
        robotObj.insert("unit_id",orderInfo.addressInfo.unitID);
    }
    robotObj.insert("elevator_id",logicParams.elevatorID);
    robotObj.insert("floor_id",logicParams.currentFloorID);
    robotObj.insert("location_x",ros->robotStateValue.point.x);
    robotObj.insert("location_y",ros->robotStateValue.point.y);
    robotObj.insert("location_a",ros->robotStateValue.point.angle);
    robotObj.insert("run_speed",0);
    robotObj.insert("rotate_speed",0);
    robotObj.insert("work_state",workState);
    robotDoc.setObject(robotObj);
    QByteArray bytes=robotDoc.toJson(QJsonDocument::Compact);
    mq->sendMessage(bytes);
}
//顾客通过手机端打开车门
void Logic::receivePickupCodeSlot(QJsonObject obj){
    QString carMac=obj.value("car_mac").toString();
    QString pickupCode=obj.value("pickup_code").toString();
    qint64 orderID=obj.value("order_id").toVariant().toLongLong();
    qint64 userID=obj.value("user_id").toVariant().toLongLong();
    if(orderState==HTTP::OS_WAITING_FOR_TAKE_GOODS){
        if(carMac==macAddress&&pickupCode==orderInfo.pickupCode&&orderID==orderInfo.orderID&&userID==orderInfo.userID){
            logicParams.codeTrueTimerCount=logicParams.timerCount;
            logicParams.isCodeTrue=true;
            note->writeLine(Note::M_HMI,"验证取货","用户通过手机端打开仓门");
            emit hmi->sendLogicTextSignal("通过手机端打开仓门");
            ros->openDoor();//打开车门
            voice->playRoundSound(S_HIGH,"codeTrue.wav",1);
            emit hmi->setPageSignal(HMI::USER_RECEIVE);//切换接收货品确认页面
            orderState=HTTP::OS_FINISHED_TAKING_GOODS;
            http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,orderState,HTTP::LS_RETURN);
        }else{
            note->writeLine(Note::M_HMI,"验证取货","核验信息不对");
        }
    }else{
        note->writeLine(Note::M_HMI,"验证取货","状态不对");
    }
}
//接收到订单
void Logic::dealOrderInfoSlot(QJsonObject obj){
    if(workState==HTTP::WS_UNUSED){
        logicParams.currentFloorID=0; //地下室
        logicParams.isCodeTrue=false; //重置验证码不正确
        orderInfo.logisticsID=obj.value("logistics_id").toVariant().toLongLong();
        orderInfo.logisticsNumber=obj.value("logistics_number").toString();
        orderInfo.orderID=obj.value("order_id").toVariant().toLongLong();
        orderInfo.userID=obj.value("user_id").toVariant().toLongLong();
        orderInfo.orderAmount=obj.value("order_amount").toVariant().toLongLong()/100.0;
        orderInfo.concessionary=obj.value("concessionary").toVariant().toLongLong()/100.0;
        orderInfo.pickupCode=obj.value("pickup_code").toString();
        orderInfo.createTime=QDateTime::fromMSecsSinceEpoch(obj.value("create_time").toVariant().toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
        orderInfo.vehicleMac=obj.value("vehicleMac").toString();
        QJsonArray goodsList=obj.value("goodsList").toArray();
        orderInfo.goodsList.clear();
        for(int i=0;i<goodsList.size();i++){
            QJsonObject goodsObj=goodsList[i].toObject();
            HMI::GOODS_INFO goodsInfo;
            goodsInfo.orderGoodsID=goodsObj.value("ord_goo_id").toVariant().toLongLong();
            goodsInfo.goodsID=goodsObj.value("goods_id").toVariant().toLongLong();
            goodsInfo.speID=goodsObj.value("spe_id").toVariant().toLongLong();
            goodsInfo.goodsNumber=goodsObj.value("goods_number").toString();
            goodsInfo.goodsName=goodsObj.value("goods_name").toString();
            goodsInfo.speInfo=goodsObj.value("spe_info").toString();
            goodsInfo.listPicture=goodsObj.value("list_picture").toString();
            goodsInfo.sellingPrice=goodsObj.value("selling_price").toDouble()/100.0;
            goodsInfo.buyNumber=goodsObj.value("buy_number").toInt();
            goodsInfo.unitWeight=goodsObj.value("unit_weight").toInt();
            int type=goodsObj.value("pack_type").toInt();
            if(type==HMI::PACK_SANBAOZHUANG){
                goodsInfo.packType=HMI::PACK_SANBAOZHUANG;
            }else if(type==HMI::PACK_YUBAOZHUANG){
                goodsInfo.packType=HMI::PACK_YUBAOZHUANG;
            }else{
                goodsInfo.packType=HMI::PACK_JINGBAOZHUANG;
            }
            orderInfo.goodsList.append(goodsInfo);
        }
        QJsonObject addressObj=obj.value("addressInfo").toObject();
        orderInfo.addressInfo.consigneeName=addressObj.value("consignee_name").toString();
        orderInfo.addressInfo.consigneePhone=addressObj.value("consignee_phone").toString();
        orderInfo.addressInfo.cuyID=addressObj.value("cuy_id").toVariant().toLongLong();
        orderInfo.addressInfo.cuyName=addressObj.value("cuy_name").toString();
        orderInfo.addressInfo.buildingID=addressObj.value("building_id").toVariant().toLongLong();
        orderInfo.addressInfo.buildingNumber=addressObj.value("building_number").toString();
        orderInfo.addressInfo.buildingNum=addressObj.value("building_number").toString().replace("楼号","");
        orderInfo.addressInfo.unitID=addressObj.value("unit_id").toVariant().toLongLong();
        orderInfo.addressInfo.unitNumber=addressObj.value("unit_number").toString();
        orderInfo.addressInfo.unitNum=addressObj.value("unit_number").toString().replace("单元","");
        orderInfo.addressInfo.floorID=addressObj.value("floor_id").toVariant().toLongLong();
        orderInfo.addressInfo.floorNumber=addressObj.value("floor_number").toString().toInt();
        orderInfo.addressInfo.roomID=addressObj.value("room_id").toVariant().toLongLong();
        orderInfo.addressInfo.roomNumber=addressObj.value("room_num").toString();
        orderInfo.addressInfo.goPathFlag=addressObj.value("go_pathFlag").toString();
        orderInfo.addressInfo.backPathFlag=addressObj.value("back_pathFlag").toString();
        orderInfo.addressInfo.point.x=addressObj.value("x").toDouble();
        orderInfo.addressInfo.point.y=addressObj.value("y").toDouble();
        orderInfo.addressInfo.point.angle=addressObj.value("a").toDouble();
        ros->openDoor(); //打开车门
        emit hmi->setPageSignal(HMI::SHOW_ORDER);//显示订单页面
        workState=HTTP::WS_RECEIVED_GOODS; //工作状态切换为已接单状态
        http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,HTTP::OS_ORDER_TAKING,HTTP::LS_ORDER_TAKING);
        emit hmi->sendLogicTextSignal("已收取新订单");
    }
}
//上传日志
void Logic::relayNoteSlot(quint64 timeStemp){
    QString dateStr=QDateTime::fromMSecsSinceEpoch(timeStemp).toString("yyyy-MM-dd");
    QString notepath=QString("%1/%2.txt").arg(NOTE_PATH,dateStr);
    QFile notefile(notepath);
    if(notefile.exists()){
        QString pictureName=camera->takePicture();
        Sleep(200);//等待图片保存
        http->uploadLog(timeStemp,pictureName);
    }else{
        note->writeLine(Note::M_HMI_ERROR,"索要日志",QString("%1.txt 日志不存在！").arg(dateStr));
    }
}
void Logic::initCommunityConfigIsSuccessSlot(bool isTrue){
    if(isTrue){
        ros->loadMap(http->initCommunityConfigInfo.cuyInfo.mapPath);
    }else{}
}
//http告知避让点位置
void Logic::carActionSlot(HTTP::POINT_INFO info){
    if(logicParams.isSavePoint){
        logicParams.savePoint=info.xyaPoint;
        logicParams.isSavePoint=false;
    }else{
        emit hmi->sendLogicTextSignal("已获取闲置点，正在前往");
        ros->gotoPoint(info.xyaPoint,logicParams.pointTag.initWait);
    }
}
//电梯上锁
void Logic::lockElevatorSlot(QJsonObject elevatorObj){
    bool isSuccess= (elevatorObj.value("code").toInt()==0?true:false);
    //获取电梯信息成功
    if(isSuccess){
        logicParams.applyElevatorCount=0;
        logicParams.isLockElevator=true;
        emit hmi->sendLogicTextSignal("已获取电梯信息");
        //如果为送货中
        if(workState==HTTP::WS_SENDING_GOODS){
            if(!logicParams.isAWEPGo){ros->cancelGotoPoint();};//如果没到避让点,取消前往避让点
            QJsonObject dataObj=elevatorObj.value("data").toObject();
            QString startFloor=QString("%1").arg(http->initCommunityConfigInfo.cuyInfo.locationFloor);
            QJsonObject startFloorObj=dataObj.value(startFloor).toObject();
            logicParams.EMacGo=startFloorObj.value("elevator_mac").toString();
            logicParams.elevatorID=startFloorObj.value("elevator_id").toInt();
            QString endFloor=QString("%1").arg(orderInfo.addressInfo.floorNumber);
            QJsonObject endFloorObj=dataObj.value(endFloor).toObject();
            logicParams.EMacBack=endFloorObj.value("elevator_mac").toString();
            QJsonObject preparePointObj=endFloorObj.value("presetPoint").toObject();
            logicParams.EWEP_BACK.x=preparePointObj.value("x").toDouble();
            logicParams.EWEP_BACK.y=preparePointObj.value("y").toDouble();
            logicParams.EWEP_BACK.angle=preparePointObj.value("a").toDouble();
            ros->rideElevator(http->initCommunityConfigInfo.cuyInfo.locationFloor,orderInfo.addressInfo.floorNumber,dataObj); //告诉ROS电梯信息
            //如果为返航中或退货返航中
        }else if(workState==HTTP::WS_RETURN){
            if(!logicParams.isAWEPBack){ros->cancelGotoPoint();};//如果没到避让点,取消前往避让点
            QJsonObject dataObj=elevatorObj.value("data").toObject();
            ros->rideElevator(orderInfo.addressInfo.floorNumber,http->initCommunityConfigInfo.cuyInfo.locationFloor,dataObj); //告诉ROS电梯信息
        }else{}
        //获取电梯信息失败
    }else{
        logicParams.applyElevatorCount++;
        if(logicParams.applyElevatorCount==1){
            receiveRosErrorSlot(ERR_UnableToApplyForElevator,"申请电梯失败！");//只上报第一次申请电梯失败信息
        }
        emit hmi->sendLogicTextSignal("获取电梯信息失败，正在重新获取");
        Sleep(3000);//延时3秒再重新申请
        //如果为送货中
        if(workState==HTTP::WS_SENDING_GOODS){
            http->lockElevator(http->initCommunityConfigInfo.cuyInfo.locationFloor,orderInfo.addressInfo.floorNumber,orderInfo.addressInfo.unitID);//继续申请电梯
            //如果为返航中
        }else if(workState==HTTP::WS_RETURN){
            http->lockElevator(orderInfo.addressInfo.floorNumber,http->initCommunityConfigInfo.cuyInfo.locationFloor,orderInfo.addressInfo.unitID);//继续申请返航电梯
        }else{}
    }
}
//电梯解锁
void Logic::unlockElevatorIsSucessSlot(bool isTrue){
    if(isTrue){
        logicParams.isLockElevator=false;
        emit hmi->sendLogicTextSignal("电梯释放成功");
    }else{
        emit hmi->sendLogicTextSignal("电梯释放失败");
    }
}
//订单上报
void Logic::trackOrderIsSuccessSlot(bool isTrue){
    if(!logicParams.isLockButton){
        if(isTrue){
            //滞留退款
            if(orderState==HTTP::OS_DETENTION_REFUND){
                emit hmi->sendLogicTextSignal("等待新订单");
                workState=HTTP::WS_UNUSED;
                camera->stopRecorder();//结束录像
                emit hmi->setPageSignal(HMI::BLINK);
            //滞留拒退款
            }else if(orderState==HTTP::OS_DETENTION_REFUSAL_REFUND){
                emit hmi->sendLogicTextSignal("等待新订单");   
                workState=HTTP::WS_UNUSED;
                camera->stopRecorder();//结束录像
                emit hmi->setPageSignal(HMI::BLINK);
            //发货中
            }else if(orderState==HTTP::OS_DELIVER_GOODS){
            //取完货
            }else if(orderState==HTTP::OS_FINISHED_TAKING_GOODS){
            //滞留
            }else if(orderState==HTTP::OS_DETENTION){
            }else{}
            logicParams.isLockButton=true;
            Sleep(3000);
            logicParams.isLockButton=false;
        }else{
            if(orderState==HTTP::OS_DETENTION_REFUND){
                emit hmi->sendLogicTextSignal("滞留退款失败");
            }else if(orderState==HTTP::OS_DETENTION_REFUSAL_REFUND){
                emit hmi->sendLogicTextSignal("滞留拒退款失败");
            }else if(orderState==HTTP::OS_DELIVER_GOODS){
                emit hmi->sendLogicTextSignal("上报发货失败");
            }else if(orderState==HTTP::OS_FINISHED_TAKING_GOODS){
                emit hmi->sendLogicTextSignal("上报确认收获失败");
                _receiveGoodsTimer->stop();
            }else if(orderState==HTTP::OS_DETENTION){
                emit hmi->sendLogicTextSignal("上报滞留失败");
                _receiveGoodsTimer->stop();
            }else{}
        }
    }else{
        if(orderState==HTTP::OS_DETENTION_REFUND){
            emit hmi->sendLogicTextSignal("滞留退款已成功触发，当前多余点击滞留退款");
        }else if(orderState==HTTP::OS_DETENTION_REFUSAL_REFUND){
            emit hmi->sendLogicTextSignal("滞留拒退款已成功触发，当前多余点击滞留拒退款");
        }else if(orderState==HTTP::OS_DELIVER_GOODS){
            emit hmi->sendLogicTextSignal("发货已成功触发，当前多余点击发货");
        }else if(orderState==HTTP::OS_FINISHED_TAKING_GOODS){
            emit hmi->sendLogicTextSignal("确认收货已成功触发，当前多余点击确认收货");
        }else if(orderState==HTTP::OS_DETENTION){
            emit hmi->sendLogicTextSignal("已上报滞留信息，当前多余上报滞留信息");
        }else{}
    }
}
//重置订单
void Logic::resetOrderIsSuccessSlot(qint32 code){
    if(code==0||code==1026){
        workState=HTTP::WS_UNUSED;
        http->carAction(HTTP::AP_WAIT,workState);
        emit hmi->setPageSignal(HMI::BLINK);
        emit hmi->sendLogicTextSignal("等待新订单");
    }else{
        emit hmi->sendLogicTextSignal("放弃订单失败");
    }
}
//计时
void Logic::receiveGoodsTimerSlot(){
    if(logicParams.isCodeTrue){
        if(logicParams.timerCount<(logicParams.codeTrueTimerCount+120)){
            emit hmi->sendLogicTextSignal(QString("取货倒计时：%1").arg(GetTimeFromSeconds(logicParams.codeTrueTimerCount+120-logicParams.timerCount)));
        }else if(logicParams.timerCount==(logicParams.codeTrueTimerCount+120)){
            ros->closeDoor();//关闭车门
            emit hmi->sendLogicTextSignal("自动确认收货,返航");
            _receiveGoodsTimer->stop();
            workState=HTTP::WS_RETURN;//小车切换为返航状态
            emit hmi->setPageSignal(HMI::BLINK);//切换为眨眼界面
            http->lockElevator(orderInfo.addressInfo.floorNumber,http->initCommunityConfigInfo.cuyInfo.locationFloor,orderInfo.addressInfo.unitID);//申请返航电梯
            ros->gotoPoint(logicParams.EWEP_BACK,logicParams.pointTag.CEW);//去电梯避让点
            voice->playRoundSound(S_HIGH,"receivedGoods.wav",1);
        }else{}
    }else{
        if(logicParams.timerCount==120){
            emit hmi->sendLogicTextSignal("拨打电话");
            http->sendVoiceCode(orderInfo.pickupCode,orderInfo.addressInfo.consigneePhone); //拨打电话
        }else if(logicParams.timerCount==420){
            emit hmi->sendLogicTextSignal("已等待7分钟，滞留返航");
            emit hmi->setPageSignal(HMI::BLINK);
            orderState=HTTP::OS_DETENTION;//订单状态切换为滞留
            http->trackOrder(orderInfo.logisticsID,orderInfo.orderID,orderState,HTTP::LS_RETURN);
            _receiveGoodsTimer->stop();
            emit hmi->sendLogicTextSignal("滞留，返航");
            workState=HTTP::WS_RETURN;//小车切换为返航
            http->lockElevator(orderInfo.addressInfo.floorNumber,http->initCommunityConfigInfo.cuyInfo.locationFloor,orderInfo.addressInfo.unitID);//申请返航电梯
            ros->gotoPoint(logicParams.EWEP_BACK,logicParams.pointTag.CEW);//去电梯避让点
        }else if(logicParams.timerCount<120){
            emit hmi->sendLogicTextSignal(QString("拨打电话倒计时：%1").arg(GetTimeFromSeconds(120-logicParams.timerCount)));
        }else if(logicParams.timerCount>120&&logicParams.timerCount<420){
            emit hmi->sendLogicTextSignal(QString("滞留返航倒计时：%1").arg(GetTimeFromSeconds(420-logicParams.timerCount)));
        }else{}
    }
   logicParams.timerCount+=1;
}
void Logic::startUpdateMap(){
    emit hmi->setAutoUpdateMapBoxEnabledSignal(false);
    bool isUsingAutoUpdateMap=config->getValue("isUsingAutoUpdateMap").toBool();
    if(isUsingAutoUpdateMap){
        logicParams.canUpdateMap=true;
        ros->startUpdateMap();
    }else{
        logicParams.canUpdateMap=false;
    }
}
void Logic::stopUpdateMap(){
    emit hmi->setAutoUpdateMapBoxEnabledSignal(true);
    bool isUsingAutoUpdateMap=config->getValue("isUsingAutoUpdateMap").toBool();
    if(isUsingAutoUpdateMap){
        ros->saveUpdateMap();
        ros->stopUpdateMap();
    }else{}
}
void Logic::receiveUDPDataSlot(){
    if(http->commnuityInfo.isSucess&&!logicParams.isSendCarSysInfo){
            HTTP::PARAM_UploadCarSysInfo carSysInfo;
        //    carSysInfo.cameraModel=               //摄像头型号
            carSysInfo.carID=http->commnuityInfo.carID;//无人车ID
        //    carSysInfo.carIP=         //无人车IP
        //    carSysInfo.driveModel=    //驱动轮型号
        //    carSysInfo.emergencyModel=    //急停型号
            carSysInfo.imei=ros->robotStateValue.IMEI;//imei串号
        //    carSysInfo.imuModel=  //IMU型号
        //    carSysInfo.larserModel=   //激光型号
            carSysInfo.lowerVersion=ros->robotStateValue.mcuHwVersion;//下位机版本
            carSysInfo.operatorName=ros->robotStateValue.SERVER;//运营商名称
        //    carSysInfo.osVersion=//系统版本
        //    carSysInfo.routerIP=//路由器IP
        //    carSysInfo.routerMac=//路由器MAC
        //    carSysInfo.routerModel=//路由器型号
        //    carSysInfo.routerVersion=//路由器版本
        //    carSysInfo.sonarModel=//声纳型号
            carSysInfo.uccidNumber=ros->robotStateValue.UCCID;//uccid卡号
            carSysInfo.uiVersion=VERSON;//界面版本
        //    carSysInfo.uuID=//下位机UUID
            http->uploadCarSysInfo(carSysInfo);
            logicParams.isSendCarSysInfo=true;//已经上传了
    }else{}
}
/*@@@@@@@@@@@@@@@@@@@@*/
Logic* logic=new Logic();
