#ifndef T_MQSERVER_H
#define T_MQSERVER_H

#include "a_global.h"

/*@@@@@@@MQServer@@@@@@@*/
class MQServer:public QObject{
    Q_OBJECT
public:
    struct ORDER_INFO{
        qint64 logisticsID;             //物流单ID
        QString logisticsNumber;        //物流单号
        qint64 orderID;                 //订单ID
        qint64 userID;                  //用户ID
        double orderAmount;             //订单总额
        double concessionary;           //优惠
        QString pickupCode;             //验证码
        QString createTime;             //创建时间
        QString vehicleMac;             //无人车MAC
        QList<HMI::GOODS_INFO> goodsList;
        struct{
            QString consigneeName;
            QString consigneePhone;
            qint64 cuyID;
            QString cuyName;
            qint64 buildingID;
            QString buildingNumber;
            QString buildingNum;
            qint64 unitID;
            QString unitNumber;
            QString unitNum;
            qint64 floorID=0;
            qint16 floorNumber;
            qint64 roomID;
            QString roomNumber;
            QString goPathFlag;
            QString backPathFlag;
            XYA_POINT point;
        }addressInfo;
    };
public:
    explicit MQServer(QObject* parent=nullptr);
    ~MQServer();
    void sendMessage(QByteArray message);
    void openServer();
signals:
    void ackRobotStateSignal();
    void receiveOrderInfoSignal(QJsonObject);
    void ackNoteSignal(quint64);
    void receivePickupCodeSignal(QJsonObject);
    void connectMqIsSuccessSignal(bool);
private slots:
    void newConnectionSlot();
    void readyReadSlot();
    void timer1000TimeoutSlot();
private:
    void _dealMessage(QByteArray bytes);
    void emitMessage(QByteArray bytes);
private:
    QTcpServer* _server;//监听套接字
    QTcpSocket* _socket;//通信套接字
    const QString _ackRobotStateKey="jld_test.server.vehicle.state.achieve.";//服务端索要数据 服务端发送
    const QString _receiveOrderKey="jld_test.server.hand.order.%1"; //服务端发送订单详情
    const QString _ackNoteKey = "jld_test.server.vehicle.log.achieve.%1";//服务端索要日志
    const QString _pickupCodeKey="jld_test.server.pickupCode.%1";//MQ发送取货吗信息
    quint8 _timeCount=Const::checkMqIsconnectTime;
    bool _isConnect=false,_isConnectTmp=false;
    QByteArray _wholeBytes="";
};

#endif // T_MQSERVER_H
