#include "t_ros.h"

/*@@@@@@@ROS交互器@@@@@@@*/
ROSTCPChater::ROSTCPChater(QObject *parent): QObject(parent){
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(connectedSlot()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnectedSlot()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
}
void ROSTCPChater::sockedSend(QByteArray data){
    unsigned int len=data.length();
    unsigned int i = 0;
    unsigned char CheckSum = 0;
    unsigned int index=0;
    static unsigned short int frameseq=0;
    //
    v_sendbuff[index++] = 0xA5;
    v_sendbuff[index++] = (uint8_t)((UGVLINK_FRAME_LEN + len) >> 8);
    v_sendbuff[index++] = (uint8_t)(UGVLINK_FRAME_LEN + len);
    //
    v_sendbuff[index++] = (uint8_t)(UGV_ADDR_TOPBOARD >> 8);
    v_sendbuff[index++] = (uint8_t)UGV_ADDR_TOPBOARD;
    //
    v_sendbuff[index++] = (uint8_t)(UGV_ADDR_MIDDLEBOARD  >> 8);
    v_sendbuff[index++] = (uint8_t)UGV_ADDR_MIDDLEBOARD;

    v_sendbuff[index++] = (uint8_t)(frameseq >> 8);
    v_sendbuff[index++] = (uint8_t)(frameseq);
    //
    frameseq++;
    v_sendbuff[index++] = UGV_FRAME_JSON_PACKET;

    for(i = 0; i < len; i++){
        v_sendbuff[index++] = (unsigned char)data.data()[i];

    }
    //
    for(i = 0; i < index; i++) {
        CheckSum += v_sendbuff[i];
    }
    v_sendbuff[index++] = CheckSum;

    QByteArray out;
    for(i=0;i<index;i++) {
        out.append(v_sendbuff[i]);
    }
    socket->write(out);
    socket->flush();
}
void ROSTCPChater::sockedReceive(unsigned char data){
    unsigned int len;
    unsigned char crc1,crc2;

//  printf("%2x ",data);
    switch(v_sta){
        case 0:{
                if(data==0xA5){
                    v_sta++;
                    v_index=0;
                    v_revbuff[v_index++]=data;
                }
            }
            break;
        case 1:
            v_revbuff[v_index++]=data;
            if(v_index>11){
                len=(v_revbuff[1]<< 8)+v_revbuff[2];
                if(len>=BUFF_MAX_LEN-UGVLINK_FRAME_LEN){
                    v_sta=0;
                    printf("bag len more than 200 err!!!\r\n");
                }
                else if(len==v_index){
                    unsigned int i;
                    crc1=0;
                    crc2=v_revbuff[len-1];
                    for(i=0;i<len-1;i++){
                        crc1+=v_revbuff[i];
                    }
                    if(crc1==crc2){
                        switch (v_revbuff[9]){
                            //得到数据包中的字符串
                            case UGV_FRAME_JSON_PACKET:{
                                std::string msg=std::string((char *)(v_revbuff+10),len-11);
                                QByteArray btarray(msg.c_str() , msg.size());
                                emit receiveMessageSignal(btarray);
                              }
                          break;
                            default:
                                printf("unknow cmd from android %d\r\n ",v_revbuff[9]);
                                //hex_publish(rdata,len-11,revbuff[9]);
                              break;
                          }
                    }else {
                        printf("crc check err \r\n");
                    }
                    v_sta=0;
                }
            }
            break;
        default:
            v_sta=0;
            v_index=0;
            break;
    }
}
void ROSTCPChater::tryConnect(){
    socket->connectToHost(v_host,v_port);
    //
    if(!socket->waitForConnected(2000)){
        v_isConnectSuccess = false;
        emit connectStateSignal(false);
    }
    else{
        v_isConnectSuccess = true;
        emit connectStateSignal(true);
    }
}
bool ROSTCPChater::isConnect(){
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(v_host,v_port);
    if(tcpClient.waitForConnected(1000)){
        if(!v_isConnectSuccess){
            socket->connectToHost(v_host,v_port);
        }
        tcpClient.disconnectFromHost();
        tcpClient.deleteLater();
        return true;
    }else{
        tcpClient.disconnectFromHost();
        tcpClient.deleteLater();
        return false;
    }
}
void ROSTCPChater::tryConnect(QString host, unsigned short int port){
    socket->connectToHost(host,port);
    //
    if(!socket->waitForConnected(2000)){
        v_isConnectSuccess = false;
        emit connectStateSignal(false);
    }
    else{
        v_isConnectSuccess = true;
        emit connectStateSignal(true);
        v_host = host;
        v_port = port;
    }
}
void ROSTCPChater::tryDisConnect(){
    socket->disconnectFromHost();
}
void ROSTCPChater::connectedSlot(){
    v_isConnectSuccess = true;
}
void ROSTCPChater::disconnectedSlot(){
    v_isConnectSuccess = false;
    emit disconnectStateSignal(true);
}
void ROSTCPChater::readyReadSlot(){
    QByteArray buffer = socket->readAll();
    for(int i=0;i<buffer.length();i++){
        sockedReceive((unsigned char)buffer[i]);
    }
}
/*@@@@@@@ROSUDP@@@@@@@@@@*/
ROSUDPChater::ROSUDPChater(QObject* parent):QObject(parent){
    _socket=new QUdpSocket(this);
    connect(_socket,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
}
void ROSUDPChater::tryConnect(){
    _socket->bind(_port,QUdpSocket::ShareAddress);
}
void ROSUDPChater::readyReadSlot(){
    //判断udpSocket是否有数据正在等待读取
    while(_socket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(_socket->pendingDatagramSize());//调整大小
        _socket->readDatagram(datagram.data(), datagram.size());//读取第一个datagram的长度
        emit receiveMessageSignal(datagram);
    }
}
/*@@@@@@@ROS收发信息打包解包@@@@@@@*/
ROS::ROS(QObject* parent):QObject(parent){
    rosTcpChater = new ROSTCPChater(this);
    rosUdpChater=new ROSUDPChater(this);
    operatorNameMap.insert("CHINA MOBILE","中国移动");
    operatorNameMap.insert("CHINA UNICOM","中国联通");
    operatorNameMap.insert("CHN-CT","中国电信");
    connect(rosTcpChater,SIGNAL(receiveMessageSignal(QByteArray)),this,SLOT(receiveTcpMessageSlot(QByteArray)));
    connect(rosTcpChater,SIGNAL(connectStateSignal(bool)),this,SLOT(connectStateSlot(bool)));
    connect(rosTcpChater,SIGNAL(disconnectStateSignal(bool)),this,SLOT(disconnectStateSlot(bool)));
    connect(rosUdpChater,SIGNAL(receiveMessageSignal(QByteArray)),this,SLOT(receiveUdpMessageSlot(QByteArray)));
}
void ROS::tryConnect(){
    rosTcpChater->tryConnect();
    rosUdpChater->tryConnect();
}
void ROS::connectStateSlot(bool state){
    emit connectRosIsSuccessSignal(state);
    if(state){
        note->writeLine(Note::M_HMI,"连接ROS","成功");
    }else{
        note->writeLine(Note::M_HMI,"连接ROS","失败");
    }
}
void ROS::disconnectStateSlot(bool state){
    emit disconnectRosIsSuccessSignal(state);
    if(state){
        note->writeLine(Note::M_HMI,"断开ROS","成功");
    }else{
        note->writeLine(Note::M_HMI,"断开ROS","失败");
    }
}
void ROS::isConnect(){
    if(rosTcpChater->isConnect()){
        emit connectRosIsSuccessSignal(true);
    }else{
        emit connectRosIsSuccessSignal(false);
    }
}
void ROS::receiveTcpMessageSlot(QByteArray byte){
//    data: "{\"result\":0,\"status\":0,\"type\":\"ackLoadMap\"}\n" status 代表任务执行状态，result代表任务执行结果，都为0代表成功完成
    QJsonDocument rootDoc = QJsonDocument::fromJson(byte);
    QJsonObject rootObj = rootDoc.object();
    QString returnType = rootObj.value("type").toString();
    if(returnType=="robotState"){_parseRobotState(rootObj);}
    else if(returnType=="ackLoadMap"){_ackLoadMap(rootObj);}
    else if(returnType=="ackInitialPose"){_ackInitialPose(rootObj);}
    else if(returnType=="ackMoveGoal"){_ackMoveGoal(rootObj);}
    else if(returnType=="ackOpenDoor"){_ackOpenDoor(rootObj);}
    else if(returnType=="ackCloseDoor"){_ackCloseDoor(rootObj);}
    else if(returnType=="upLoadError"){_uploadError(rootObj);}
    else if(returnType=="ackScanDoor"){_ackScanDoor(rootObj);}
    else if(returnType=="ackMoveGoalFile"){_ackMoveGoaFile(rootObj);}
    else if(returnType=="ackRideElevator"){_ackRideElevator(rootObj);}
    else if(returnType=="ackUpdateMap"){_ackUpdateMap(rootObj);}
    else{}
}
void ROS::receiveUdpMessageSlot(QByteArray byte){
    QJsonDocument rootDoc = QJsonDocument::fromJson(byte);
    QJsonObject rootObj = rootDoc.object();
    robotStateValue.IMEI=rootObj.value("IMEI").toString();
    robotStateValue.UCCID=rootObj.value("UCCID").toString();
    robotStateValue.SERVER=rootObj.value("SERVER").toString();
    robotStateValue.CSQ=rootObj.value("CSQ").toInt();
    emit receiveUDPDataSignal();
    if(_isJustReceiveUdpData){
        note->writeLine(Note::M_FROM_ROS,"IMEI",robotStateValue.IMEI);
        note->writeLine(Note::M_FROM_ROS,"UCCID",robotStateValue.UCCID);
        note->writeLine(Note::M_FROM_ROS,"SERVER",robotStateValue.SERVER);
        note->writeLine(Note::M_FROM_ROS,"CSQ",QString::number(robotStateValue.CSQ));
        _isJustReceiveUdpData=false;
    }
}
void ROS::_parseRobotState(QJsonObject rootObj){
    emit robotStateChangedSignal();
    bool charge = bool(rootObj.value("charge").toInt());
    bool collision = rootObj.value("collision").toBool();
    double cur =  rootObj.value("cur").toInt()/100.0;
    bool doorState = rootObj.value("doorState").toBool();
    bool emergency = rootObj.value("emergency").toBool();
    bool imu = rootObj.value("imu").toBool();
    quint8 mah = rootObj.value("mah").toInt();
    QString mapFile = rootObj.value("mapFile").toString();
    QString mcuHwVersion = rootObj.value("mcu_hw_version").toString();
    QString mcuSoftVersion = rootObj.value("mcu_soft_version").toString();
    qint32 mils =rootObj.value("mils").toInt();
    quint8 relay = rootObj.value("relay").toInt();
    QString rosHwVersion = rootObj.value("ros_soft_version").toString();
    QString rosSoftVersion = rootObj.value("ros_soft_version").toString();
    quint8 skip = rootObj.value("skip").toInt();
    bool sonar = rootObj.value("sonar").toBool();
    double vol = rootObj.value("vol").toInt()/100.0;
    bool wheel = rootObj.value("wheel").toBool();
    ///
    robotStateValue.point.x = rootObj.value("x").toDouble();
    robotStateValue.point.y = rootObj.value("y").toDouble();
    robotStateValue.point.angle = rootObj.value("yaw").toDouble();
    robotStateValue.xyzwPoint.x= rootObj.value("x").toDouble();
    robotStateValue.xyzwPoint.y = rootObj.value("y").toDouble();
    robotStateValue.xyzwPoint.z = rootObj.value("z").toDouble();
    robotStateValue.xyzwPoint.w = rootObj.value("w").toDouble();
    if(_isJustRun){
        robotStateValue.charge = charge;
        robotStateValue.collision = collision;
        robotStateValue.cur = cur;
        robotStateValue.doorState = doorState;
        robotStateValue.emergency = emergency;
        robotStateValue.imu = imu;
        robotStateValue.mah = mah;
        robotStateValue.mapFile = mapFile;
        robotStateValue.mcuHwVersion = mcuHwVersion;
        robotStateValue.mcuSoftVersion = mcuSoftVersion;
        robotStateValue.mils = mils;
        robotStateValue.relay = relay;
        robotStateValue.rosHwVersion = rosHwVersion;
        robotStateValue.rosSoftVersion = rosSoftVersion;
        robotStateValue.skip = skip;
        robotStateValue.sonar = sonar;
        robotStateValue.vol = vol;
        robotStateValue.wheel = wheel;
        emit chargeStateChangedSignal(charge);
        note->writeLine(Note::M_FROM_ROS,"是否在充电",QString("%1").arg(bool(charge)?"是":"否"));
        emit collisionStateChangedSignal(collision);
        note->writeLine(Note::M_FROM_ROS,"碰撞是否异常",QString("%1").arg(bool(collision)?"是":"否"));
        emit curValueChangedSignal(cur);
        note->writeLine(Note::M_FROM_ROS,"电流",QString("%1安").arg(cur));
        emit doorStateChangedSignal(doorState);
        note->writeLine(Note::M_FROM_ROS,"门的状态",QString("%1").arg(doorState?"开启":"关闭"));
        emit emergencyStateChangedSignal(emergency);
        note->writeLine(Note::M_FROM_ROS,"急停按钮是否触发",QString("%1").arg(emergency?"是":"否"));
        emit imuStateChangedSignal(imu);
        note->writeLine(Note::M_FROM_ROS,"陀螺仪是否异常",QString("%1").arg(imu?"是":"否"));
        emit mahValueChangedSignal(mah);
        note->writeLine(Note::M_FROM_ROS,"电池电量",QString("%1%").arg(mah));
        emit mapFileChangedSignal(mapFile);
        note->writeLine(Note::M_FROM_ROS,"地图路径",QString("%1").arg(mapFile));
        emit mcuHwVersionChangedSignal(mcuHwVersion);
        note->writeLine(Note::M_FROM_ROS,"下位机版本",QString("%1").arg(mcuHwVersion));
        emit mcuSoftVersionChangedSignal(mcuSoftVersion);
        note->writeLine(Note::M_FROM_ROS,"下位机软件版本",QString("%1").arg(mcuSoftVersion));
        emit milsValueChangedSignal(mils);
        note->writeLine(Note::M_FROM_ROS,"里程数",QString("%1米").arg(mils));
        emit relayChangedSignal(relay);
        note->writeLine(Note::M_FROM_ROS,"继电器状态",QString("%1 {0关闭/1正常/2充电}").arg(relay));
        emit rosHwVersionChangedSignal(rosHwVersion);
        note->writeLine(Note::M_FROM_ROS,"ROS版本",QString("%1").arg(rosHwVersion));
        emit rosSoftVersionChangedSignal(rosSoftVersion);
        note->writeLine(Note::M_FROM_ROS,"ROS软件版本",QString("%1").arg(rosSoftVersion));
        emit skipChangedSignal(skip);
        note->writeLine(Note::M_FROM_ROS,"环境匹配度",QString("%1").arg(skip));
        emit sonarStateChangedSignal(sonar);
        note->writeLine(Note::M_FROM_ROS,"超声波是否异常",QString("%1").arg(sonar?"是":"否"));
        emit volValueChangedSignal(vol);
        note->writeLine(Note::M_FROM_ROS,"电压",QString("%1伏特").arg(vol));
        emit wheelStateChangedSignal(wheel);
        note->writeLine(Note::M_FROM_ROS,"轮子是否异常",QString("%1").arg(wheel?"是":"否"));
        _isJustRun = false;
    }else{
        if(robotStateValue.charge != charge){
            robotStateValue.charge=charge;
            emit chargeStateChangedSignal(charge);
            note->writeLine(Note::M_FROM_ROS,"是否在充电",QString("%1").arg(bool(charge)?"是":"否"));
        }
        if(robotStateValue.collision != collision){
            robotStateValue.collision=collision;
            emit collisionStateChangedSignal(collision);
            note->writeLine(Note::M_FROM_ROS,"碰撞是否异常",QString("%1").arg(bool(collision)?"是":"否"));
        }
        if(robotStateValue.cur != cur){
            robotStateValue.cur = cur;
            emit curValueChangedSignal(cur);
            note->writeLine(Note::M_FROM_ROS,"电流",QString("%1安").arg(cur));
        }
        // 对比门的状态，如果门的状态非想要的状态，一直命令ROS去执行直至门的状态理想
        if(doorState!=_dreamDoorState){
            if(_dreamDoorState){openDoor();}
            else{closeDoor();}
        }
        if(robotStateValue.doorState != doorState){
            robotStateValue.doorState = doorState;
            emit doorStateChangedSignal(doorState);
            note->writeLine(Note::M_FROM_ROS,"门的状态",QString("%1").arg(doorState?"开启":"关闭"));
        }
        if(robotStateValue.emergency != emergency){
            robotStateValue.emergency = emergency;
            emit emergencyStateChangedSignal(emergency);
            note->writeLine(Note::M_FROM_ROS,"急停按钮是否触发",QString("%1").arg(emergency?"是":"否"));
        }
        if(robotStateValue.imu != imu){
            robotStateValue.imu = imu;
            emit imuStateChangedSignal(imu);
            note->writeLine(Note::M_FROM_ROS,"陀螺仪是否异常",QString("%1").arg(imu?"是":"否"));
        }
        if(robotStateValue.mah != mah){
            robotStateValue.mah = mah;
            emit mahValueChangedSignal(mah);
//            note->writeLine(Note::M_FROM_ROS,"电池电量",QString("%1%").arg(mah));
        }
        if(robotStateValue.mapFile != mapFile){
            robotStateValue.mapFile = mapFile;
            emit mapFileChangedSignal(mapFile);
            note->writeLine(Note::M_FROM_ROS,"地图路径",QString("%1").arg(mapFile));
        }
        if(robotStateValue.mcuHwVersion != mcuHwVersion){
            robotStateValue.mcuHwVersion = mcuHwVersion;
            emit mcuHwVersionChangedSignal(mcuHwVersion);
            note->writeLine(Note::M_FROM_ROS,"下位机版本",QString("%1").arg(mcuHwVersion));
        }
        if(robotStateValue.mcuSoftVersion != mcuSoftVersion){
            robotStateValue.mcuSoftVersion = mcuSoftVersion;
            emit mcuSoftVersionChangedSignal(mcuSoftVersion);
            note->writeLine(Note::M_FROM_ROS,"下位机软件版本",QString("%1").arg(mcuSoftVersion));
        }
        if(robotStateValue.mils != mils){
            robotStateValue.mils = mils;
            emit milsValueChangedSignal(mils);
//            note->writeLine(Note::M_FROM_ROS,"里程数",QString("%1米").arg(mils));
        }
        if(robotStateValue.relay != relay){
            robotStateValue.relay = relay;
            emit relayChangedSignal(relay);
            note->writeLine(Note::M_FROM_ROS,"继电器状态",QString("%1 {0关闭/1正常/2充电}").arg(relay));
        }
        if(robotStateValue.rosHwVersion != rosHwVersion){
            robotStateValue.rosHwVersion = rosHwVersion;
            emit rosHwVersionChangedSignal(rosHwVersion);
            note->writeLine(Note::M_FROM_ROS,"ROS版本",QString("%1").arg(rosHwVersion));
        }
        if(robotStateValue.rosSoftVersion != rosSoftVersion){
            robotStateValue.rosSoftVersion = rosSoftVersion;
            emit rosSoftVersionChangedSignal(rosSoftVersion);
            note->writeLine(Note::M_FROM_ROS,"ROS软件版本",QString("%1").arg(rosSoftVersion));
        }
        if(robotStateValue.skip != skip){
            robotStateValue.skip = skip;
            emit skipChangedSignal(skip);
//            note->writeLine(Note::M_FROM_ROS,"环境匹配度",QString("%1").arg(skip));
        }
        if(robotStateValue.sonar != sonar){
            robotStateValue.sonar = sonar;
            emit sonarStateChangedSignal(sonar);
            note->writeLine(Note::M_FROM_ROS,"超声波是否异常",QString("%1").arg(sonar?"是":"否"));
        }
        if((robotStateValue.vol-vol >= 0.05) || (robotStateValue.vol-vol <= -0.05)){
            robotStateValue.vol = vol;
            emit volValueChangedSignal(vol);
//            note->writeLine(Note::M_FROM_ROS,"电压",QString("%1伏特").arg(vol));
        }
        if(robotStateValue.wheel != wheel){
            robotStateValue.wheel = wheel;
            emit wheelStateChangedSignal(wheel);
            note->writeLine(Note::M_FROM_ROS,"轮子是否异常",QString("%1").arg(wheel?"是":"否"));
        }
    }
}
void ROS::_getLaserData(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "pointCloudData");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    if(_isJustGetLarserData){
         note->writeLine(Note::M_TO_ROS,"激光点数据",QString("为节省空间仅保存第一次请求：{%1}").arg(QString(toByte)));
        _isJustGetLarserData = false;
    }
    if(_isJustGetLarserData&&!_isJustReceiveLarserData){
         note->writeLine(Note::M_ROS_ERROR,"激光点数据","未返回激光数据");
    }
    rosTcpChater->sockedSend(toByte);
}
void ROS::_ackUpdateMap(QJsonObject rootObj){
    QString action=rootObj.value("action").toString();
    if(action=="start"){
        note->writeLine(Note::M_FROM_ROS,"更新地图信息","开始");
        emit updateMapSignal(ROS::UPDATE_MAP_START);
    }else if(action=="stop"){
        note->writeLine(Note::M_FROM_ROS,"更新地图信息","停止");
        emit updateMapSignal(ROS::UPDATE_MAP_STOP);
    }else if(action=="save"){
        note->writeLine(Note::M_FROM_ROS,"更新地图信息","保存");
        emit updateMapSignal(ROS::UPDATE_MAP_SAVE);
    }else if(action=="reload"){
        note->writeLine(Note::M_FROM_ROS,"更新地图信息","重载");
        emit updateMapSignal(ROS::UPDATE_MAP_RELOAD);
    }else if(action=="update"){
        note->writeLine(Note::M_FROM_ROS,"更新地图信息","更新");
        emit updateMapSignal(ROS::UPDATE_MAP_UPDATE);
    }else{}
}
void ROS::_ackLoadMap(QJsonObject rootObj){
    quint8 result = rootObj.value("result").toInt();
    quint8 status = rootObj.value("status").toInt();
    if(result==0 && status==0){
        note->writeLine(Note::M_FROM_ROS,"加载地图","成功");
        emit loadMapSignal(ROS::RS_SUCEESS);
    }else if(result!=0&&status==0){
        note->writeLine(Note::M_FROM_ROS,"加载地图","失败");
        emit loadMapSignal(ROS::RS_FAIL);
    }else{
        note->writeLine(Note::M_FROM_ROS,"加载地图","正在执行中");
        emit loadMapSignal(RS_RUNNING);
    }
}
void ROS::_ackInitialPose(QJsonObject rootObj){
    quint8 result = rootObj.value("result").toInt();
    quint8 status = rootObj.value("status").toInt();
    if(result==0 && status==0){
        note->writeLine(Note::M_FROM_ROS,"初始化点","成功");
        emit initialPoseSignal(ROS::RS_SUCEESS);
    }else if(result!=0&&status==0){
        note->writeLine(Note::M_FROM_ROS,"初始化点","失败");
        emit initialPoseSignal(ROS::RS_FAIL);
    }else{
        note->writeLine(Note::M_FROM_ROS,"初始化点","正在执行中");
        emit initialPoseSignal(RS_RUNNING);
    }
}
void ROS::_ackMoveGoal(QJsonObject rootObj){
    quint8 result = rootObj.value("result").toInt();
    quint8 status = rootObj.value("status").toInt();
    QString tag = rootObj.value("tag").toString();
    if(result==0 && status==0){
        note->writeLine(Note::M_FROM_ROS,"前往目标点","成功");
        emit moveGoalSignal(ROS::RS_SUCEESS,tag);
    }else if(result!=0&&status==0){
        note->writeLine(Note::M_FROM_ROS,"前往目标点","失败");
        emit moveGoalSignal(ROS::RS_FAIL,tag);
    }else{
        note->writeLine(Note::M_FROM_ROS,"前往目标点","正在执行中");
        emit moveGoalSignal(RS_RUNNING,tag);
    }
    _moveGoalResult=result;
    _moveGoalStatus=status;
}
void ROS::_ackMoveGoaFile(QJsonObject rootObj){
    quint8 result = rootObj.value("result").toInt();
    quint8 status = rootObj.value("status").toInt();
    if(result==0 && status==0){
        note->writeLine(Note::M_FROM_ROS,"靠右行驶前往目标","成功");
        emit moveGoalFileSignal(ROS::RS_SUCEESS);
    }else if(result!=0&&status==0){
        note->writeLine(Note::M_FROM_ROS,"靠右行驶前往目标","失败");
        emit moveGoalFileSignal(ROS::RS_FAIL);
    }else{
        note->writeLine(Note::M_FROM_ROS,"靠右行驶前往目标","正在执行中");
        emit moveGoalFileSignal(RS_RUNNING);
    }
}
void ROS::_ackOpenDoor(QJsonObject rootObj){
    quint8 result = rootObj.value("result").toInt();
    if(result==0){
        note->writeLine(Note::M_FROM_ROS,"打开车门","成功");
        emit openDoorSignal(ROS::RS_SUCEESS);
    }else{
        note->writeLine(Note::M_FROM_ROS,"打开车门","正在执行中");
        emit openDoorSignal(RS_RUNNING);
    }
}
void ROS::_ackCloseDoor(QJsonObject rootObj){
    quint8 result = rootObj.value("result").toInt();
    if(result==0){
        note->writeLine(Note::M_FROM_ROS,"关闭车门","成功");
        emit closeDoorSignal(ROS::RS_SUCEESS);
    }else{
        note->writeLine(Note::M_FROM_ROS,"关闭车门","正在执行中");
        emit closeDoorSignal(RS_RUNNING);
    }
}
void ROS::_ackRideElevator(QJsonObject rootObj){
    quint8 result = rootObj.value("result").toInt();
    quint8 status = rootObj.value("status").toInt();
    QString elevatorTag = rootObj.value("tag").toString();
    if(result==0 && status==0){
        note->writeLine(Note::M_FROM_ROS,"乘坐电梯","成功");
        emit rideElevatorSignal(ROS::ELE_SUCEESS);
    }else if(result!=0&&status==0){
        note->writeLine(Note::M_FROM_ROS,"乘坐电梯","失败");
        emit rideElevatorSignal(ELE_FAIL);
    }else if(result==0&&status==1){
        note->writeLine(Note::M_FROM_ROS,"乘坐电梯","到电梯口");
        emit rideElevatorSignal(ELE_DAODIANTIKOU);
    }else if(result==0&&status==2){
        note->writeLine(Note::M_FROM_ROS,"乘坐电梯","等待电梯到达（进）");
        emit rideElevatorSignal(ELE_DENGDAIDIANTIDAODA_IN);
    }else if(result==0&&status==3){
        note->writeLine(Note::M_FROM_ROS,"乘坐电梯","扫描电梯门（进）");
        emit rideElevatorSignal(ELE_SAOMIAODIANTIMEN_IN);
    }else if(result==0&&status==4){
        note->writeLine(Note::M_FROM_ROS,"乘坐电梯","进入电梯");
        emit rideElevatorSignal(ELE_JINRUDIANTI);
    }else if(result==0&&status==5){
        note->writeLine(Note::M_FROM_ROS,"乘坐电梯","切换楼层");
        emit rideElevatorSignal(ELE_QIEHUANLOUCENG);
    }else if(result==0&&status==6){
        note->writeLine(Note::M_FROM_ROS,"乘坐电梯","等待电梯到达（出）");
        emit rideElevatorSignal(ELE_DENGDAIDIANTIDAODA_OUT);
    }else if(result==0&&status==7){
        note->writeLine(Note::M_FROM_ROS,"乘坐电梯","扫描电梯门（出）");
        emit rideElevatorSignal(ELE_SAOMIAODIANTIMEN_OUT);
    }else if(result==0&&status==8){
        note->writeLine(Note::M_FROM_ROS,"乘坐电梯","出电梯");
        emit rideElevatorSignal(ELE_CHUDIANTI);
    }else if(result==0&&status==9){
        note->writeLine(Note::M_FROM_ROS,"乘坐电梯","空间不足等下一班电梯");
        emit rideElevatorSignal(ELE_CHUDIANTI);
    }
}
void ROS::_uploadError(QJsonObject rootObj){
    quint32 errcode = rootObj.value("errCode").toVariant().toUInt();
    QString msg = rootObj.value("msg").toString();
    note->writeLine(Note::M_FROM_ROS,"错误信息",QString("错误码：%1，错误信息：%2").arg(errcode).arg(msg));
    if(_currentErrorCode!=errcode){
        emit receiveRosErrorSignal(errcode,msg);
    }
}
void ROS::_ackScanDoor(QJsonObject rootObj){
    quint8 score = rootObj.value("score").toInt();
    note->writeLine(Note::M_FROM_ROS,"电梯门检测",QString("进度：%1 解释信息：0～100,100代表门开了").arg(score));
    emit scanDoorSignal(score);
}
void ROS::initPoint(XYA_POINT point){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "initialPose");
    json.insert("x",point.x);
    json.insert("y", point.y);
    json.insert("yaw",point.angle);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"初始化点",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::gotoPoint(XYA_POINT point,QString tag){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "moveGoal");
    json.insert("x",point.x);
    json.insert("y",point.y);
    json.insert("yaw",point.angle);
    json.insert("tag",tag);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"前往目标点",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::cancelGotoPoint(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "moveCancel");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"取消前往目标点",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::setRelayState(quint8 relayValue){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "RelaySwitch");
    json.insert("relayswitch",relayValue);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"切换继电器状态",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::loadMap(QString mapPath){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "loadMap");
    json.insert("mapFile",mapPath);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"加载地图",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::gotoBuilding(QString filename,QString building){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "moveGoalFile");
    json.insert("fileName",filename);
    json.insert("building",building);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"发送多点靠右导航",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::openDoor(){
    _dreamDoorState = true;
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "door");
    json.insert("open",true);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"打开车门",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::closeDoor(){
    _dreamDoorState = false;
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "door");
    json.insert("open",false);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"关闭车门",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::openPower(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "RelaySwitch");
    json.insert("relayswitch",2);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"开始充电",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::closePower(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "RelaySwitch");
    json.insert("relayswitch",1);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"停止充电",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::shutPower(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "RelaySwitch");
    json.insert("relayswitch",0);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"关机断电",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::openScanDoor(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "scanDoor");
    json.insert("enable",true);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"扫描电梯",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::closeScanDoor(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "scanDoor");
    json.insert("enable",false);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"停止扫描电梯",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::restartMiddleMachine(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "reBootRos");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"重启中位机",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::startMap(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "startMap");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"启动建图",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::saveMap(QString mapName){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "saveMap");
    json.insert("name", mapName);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"保存地图",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::stopMap(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "stopMap");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"停止建图",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::openLarser(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "laserPowerContral");
    json.insert("enable", true);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"扫描电梯",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::closeLarser(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "laserPowerContral");
    json.insert("enable", false);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"停止扫描电梯",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::continueTask(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "moveResume");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"继续任务",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::sendSpeed(double velocity,double angularVelocity){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "cmdVel");
    json.insert("v", velocity);
    json.insert("vth", angularVelocity);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"发送速度",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::rideElevator(qint16 startFloor,qint16 endFloor,QJsonObject elevatorObj){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "rideElevator");
    json.insert("floor_sets", QJsonArray({startFloor,endFloor}));
    json.insert("result",elevatorObj);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"发送电梯信息",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::takePicture(QString filepath){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "camera");
    json.insert("path", filepath);
    json.insert("action","takePicture");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"拍照",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::startRecorder(QString filepath){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "camera");
    json.insert("path", filepath);
    json.insert("action","startRecorder");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"开始录制视频",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::stopRecorder(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "camera");
    json.insert("action","stopRecorder");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"结束录制视频",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::updateDownMachine(QString path){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "upgrade");
    json.insert("target","HW_BOARD");
    json.insert("path",path);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"升级下位机",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::updateMiddleMachine(QString path){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "upgrade");
    json.insert("target","ROS");
    json.insert("path",path);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"升级中位机",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::updateUpperMachine(QString path){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "upgrade");
    json.insert("target","QT");
    json.insert("path",path);
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"升级上位机",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::startUpdateMap(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "updateMap");
    json.insert("action","start");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"开始更新地图",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::saveUpdateMap(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "updateMap");
    json.insert("action","save");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"保存更新地图",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::stopUpdateMap(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "updateMap");
    json.insert("action","stop");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"结束更新地图",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::reloadUpdateMap(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "updateMap");
    json.insert("action","reload");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"结束更新地图",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
void ROS::updateUpdateMap(){
    QJsonObject json;
    QJsonDocument document;
    json.insert("type", "updateMap");
    json.insert("action","update");
    document.setObject(json);
    QByteArray toByte = document.toJson(QJsonDocument::Compact);
    note->writeLine(Note::M_TO_ROS,"结束更新地图",QString(toByte));
    rosTcpChater->sockedSend(toByte);
}
