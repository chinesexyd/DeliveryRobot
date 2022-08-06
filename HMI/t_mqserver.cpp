#include "t_mqserver.h"

/*@@@@@@@MQServer@@@@@@@*/
MQServer::MQServer(QObject* parent):QObject(parent){
    _server=new QTcpServer(this);
    _server->setMaxPendingConnections(1);
}
MQServer::~MQServer(){
    int killResult=system("killall RabbitMQ");
    note->writeLine(Note::M_HMI,"杀死RabbitMQ进程",QString("MQServer析构函数。system：killall RabbitMQ [result:%2]").arg(killResult));
}
void MQServer::openServer(){
    bool isSuccess=_server->listen(QHostAddress("127.0.0.1"),8888);
    if(isSuccess){
        int killResult=system("killall RabbitMQ");
        note->writeLine(Note::M_HMI,"杀死RabbitMQ进程",QString("首次打开，杀一杀。system：killall RabbitMQ [result:%2]").arg(killResult));
        connect(_server,SIGNAL(newConnection()),this,SLOT(newConnectionSlot()));
        connect(timer,SIGNAL(timer1000TimeoutSignal()),this,SLOT(timer1000TimeoutSlot()));
        int restartResult=system(QString(QDir::homePath()+"/Documents/RabbitMQ &").toUtf8().data());
        note->writeLine(Note::M_HMI,"开启RabbitMQ进程",QString("首次启动RabbitMQ。system:%1/Documents/RabbitMQ & [result:%2]").arg(QDir::homePath()).arg(restartResult));
    }else{
        note->writeLine(Note::M_HMI_ERROR,"打开MQTCPServer","失败");
    }
}
void MQServer::newConnectionSlot(){
    _socket=_server->nextPendingConnection();
    QString ip=_socket->peerAddress().toString();
    quint16 port=_socket->peerPort();
    note->writeLine(Note::M_HMI,"MQ通信TCP建立连接",QString("地址：%1,端口：%2").arg(ip).arg(port));
    connect(_socket,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
    _timeCount=Const::checkMqIsconnectTime;
}
void MQServer::readyReadSlot(){
    _timeCount=0;
    QByteArray bytes=_socket->readAll();
    _dealMessage(bytes);
}
void MQServer::_dealMessage(QByteArray bytes){
    if(bytes.contains("\n")){
        int index=bytes.indexOf("\n");
        _wholeBytes+=bytes.mid(0,index);
        emitMessage(_wholeBytes);
        _wholeBytes.clear();
        _dealMessage(bytes.mid(index+1,bytes.length()));
    }else{
        _wholeBytes+=bytes;
    }
}
void MQServer::emitMessage(QByteArray wholeBytes){
    QJsonDocument doc=QJsonDocument::fromJson(wholeBytes);
    QJsonObject obj=doc.object();
    QString routingKey=obj.value("routingKey").toString();
    QJsonObject message=obj.value("message").toObject();
    if(routingKey==_ackRobotStateKey){
        emit ackRobotStateSignal();
    }else if(routingKey==_receiveOrderKey.arg(macAddress)){
        note->writeLine(Note::M_FROM_MQ,"收到订单",wholeBytes);
        emit receiveOrderInfoSignal(message);
    }else if(routingKey==_pickupCodeKey.arg(macAddress)){
        note->writeLine(Note::M_FROM_MQ,"取货验证",wholeBytes);
        emit receivePickupCodeSignal(message);
    }else if(routingKey==_ackNoteKey.arg(macAddress)){
        note->writeLine(Note::M_FROM_MQ,"索要日志",wholeBytes);
        quint64 timeStamp=message.value("date").toVariant().toULongLong();
        emit ackNoteSignal(timeStamp);
    }else{
        note->writeLine(Note::M_MQ_ERROR,"不认识的数据",wholeBytes);
    }
}
void MQServer::sendMessage(QByteArray message){
    _socket->write(message+"\n");
    _socket->flush();
}
void MQServer::timer1000TimeoutSlot(){
    if(_timeCount<Const::checkMqIsconnectTime){
        _isConnect=true;
    }else if(_timeCount>=Const::checkMqIsconnectTime&&_timeCount<Const::reconnectMqTime){
        _isConnect=false;
    }else if(_timeCount==Const::reconnectMqTime){
        int killResult=system("killall RabbitMQ");
        note->writeLine(Note::M_HMI,"杀死RabbitMQ进程",QString("%1秒没有收到MQ消息。system：killall RabbitMQ [result:%2]").arg(Const::reconnectMqTime).arg(killResult));
        Sleep(500);
        int restartResult=system(QString(QDir::homePath()+"/Documents/RabbitMQ &").toUtf8().data());
        note->writeLine(Note::M_HMI,"开启RabbitMQ进程",QString("重启一下RabbitMQ。system:%1/Documents/RabbitMQ & [result:%2]").arg(QDir::homePath()).arg(restartResult));
        _timeCount=Const::checkMqIsconnectTime;
        _isConnect=false;
    }
    if(_isConnect!=_isConnectTmp){
        _isConnectTmp=_isConnect;
        emit connectMqIsSuccessSignal(_isConnect);
    }
    _timeCount++;
}
