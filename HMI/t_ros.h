#ifndef ROS_H
#define ROS_H

#define UGVLINK_FRAME_LEN 11
#define BUFF_MAX_LEN 60000

#include "HMI/a_global.h"

typedef enum{
    UGV_ADDR_BOTTOMBOARD = 0x0001,
    UGV_ADDR_MIDDLEBOARD = 0x0002,
    UGV_ADDR_TOPBOARD = 0x0003
}UGV_DeviceAddr;

typedef enum{
    UGV_FRAME_JSON_PACKET = 0x30,
    UGV_FRAME_UNKNOWN = 0xFF
}UGV_FrameType;

/*@@@@@@@ROS交互器@@@@@@@*/
class ROSTCPChater:public QObject{
    Q_OBJECT
public:
    ROSTCPChater(QObject *parent=nullptr);
    void tryConnect();
    void tryConnect(QString m_host,unsigned short int m_port);
    void tryDisConnect();
    void sockedSend(QByteArray data);
    bool isConnect();
private:
    void sockedReceive(unsigned char data);
public slots:
    void connectedSlot();
    void disconnectedSlot();
    void readyReadSlot();
signals:
    void receiveMessageSignal(QByteArray);
    void connectStateSignal(bool);
    void disconnectStateSignal(bool);
private:
    QTcpSocket *socket;
    QString v_host = "192.168.1.101";
    unsigned short int v_port=9999;
    bool v_isConnectSuccess=false;
    unsigned int v_index;
    unsigned char v_sta,v_revbuff[BUFF_MAX_LEN],v_sendbuff[BUFF_MAX_LEN];
};
/*@@@@@@@ROSUDP@@@@@@@@@@*/
class ROSUDPChater:public QObject{
    Q_OBJECT
public:
    ROSUDPChater(QObject *parent=nullptr);
    void tryConnect();
private:
    void sockedReceive(QByteArray);
public slots:
    void readyReadSlot();
signals:
    void receiveMessageSignal(QByteArray);
private:
    QUdpSocket* _socket;
    QString _host="0.0.0.0";
    quint16 _port=10000;
    bool _isConnectSuccess=false;
};
/*@@@@@@@ROS收发信息打包解包@@@@@@@*/
class ROS:public QObject{
    Q_OBJECT
public:
    //机器人状态
    struct ROBOT_STATE_VALUE{
        bool charge;//是否在充电
        bool collision; //碰撞是否异常
        double cur; //电流
        bool doorState; //门的状态
        bool emergency; //急停按钮是否触发
        bool imu; //陀螺仪是否异常
        quint8 mah; //电池电量0～100
        QString mapFile;//地图路径名
        QString mcuHwVersion;//下位机版本
        QString mcuSoftVersion;//下位机软件版本
        qint32 mils;//里程数
        quint8 relay;//继电器状态 0关机 1正常 2充电
        QString rosHwVersion;//ros下位机版本
        QString rosSoftVersion;//ros软件版本
        quint8 skip;//环境匹配度 0～100 值越大越不匹配
        bool sonar;//超声波是否异常
        QString type;//类型为robotState才可解析位置数据
        double vol;//电压
        bool wheel;//轮子是否异常
        XYA_POINT point;//位置
        XYZW_POINT xyzwPoint;
        quint8 CSQ;//信号强度
        QString IMEI;
        QString UCCID;
        QString SERVER;//服务商
    };
    enum RETURN_STATE{
        RS_SUCEESS,
        RS_FAIL,
        RS_RUNNING
    };//执行后返回状态
    enum RETURN_ELEVATOR_STATE{
        ELE_SUCEESS,
        ELE_FAIL,
        ELE_DAODIANTIKOU, //到电梯门口
        ELE_DENGDAIDIANTIDAODA_IN,//等待电梯到达，进
        ELE_SAOMIAODIANTIMEN_IN,//扫描电梯门，进
        ELE_JINRUDIANTI,//进入电梯
        ELE_QIEHUANLOUCENG,//切换楼层
        ELE_DENGDAIDIANTIDAODA_OUT,//等待电梯到达，出
        ELE_SAOMIAODIANTIMEN_OUT,//扫描电梯门，出
        ELE_CHUDIANTI, //出电梯
        ELE_KONGIANBUZU //空间不足等下一班电梯
    };//电梯返回状态
    enum UPDATE_MAP_ENUM{
        UPDATE_MAP_START,
        UPDATE_MAP_STOP,
        UPDATE_MAP_SAVE,
        UPDATE_MAP_RELOAD,
        UPDATE_MAP_UPDATE
    };
public:
    ROS(QObject* parent=nullptr);
    void tryConnect();
    void initPoint(XYA_POINT point); //初始化点
    void gotoPoint(XYA_POINT point,QString tag); //前往目标点
    void cancelGotoPoint();//取消前往目标点
    void setRelayState(quint8 relayValue);//设置继电器
    void loadMap(QString mapPath);//初始化地图（相对路径）
    void gotoBuilding(QString filename,QString building);//发送多点靠右导航，文件名+楼栋
    void openDoor();//打开舱门
    void closeDoor();//关闭舱门
    void openPower();//充电
    void closePower();//停止充电
    void shutPower();//关机
    void openScanDoor();//开启扫描电梯
    void closeScanDoor();//关闭扫描电梯
    void restartMiddleMachine();//重启中位机
    void startMap();//启动建图
    void saveMap(QString mapName="123");//保存地图
    void stopMap();//停止建图
    void openLarser();//打开激光
    void closeLarser();//关闭激光
    void continueTask();//继续任务
    void sendSpeed(double velocity,double angularVelocity);//发送速度，角速度
    void rideElevator(qint16 startFloor,qint16 endFloor,QJsonObject elevatorObj);//乘坐电梯
    void isConnect();
    void takePicture(QString filepath);//拍照
    void startRecorder(QString filepath);//开始录像
    void stopRecorder();//停止录像
    void updateDownMachine(QString path);//更新下位机
    void updateMiddleMachine(QString path);//更新中位机
    void updateUpperMachine(QString path);//更新上位机
    void startUpdateMap();//开始更新地图
    void saveUpdateMap();//保存更新地图
    void stopUpdateMap();//结束更新地图
    void reloadUpdateMap();//重载更新地图
    void updateUpdateMap();//更新更新地图
public slots:
    void receiveTcpMessageSlot(QByteArray byte);
    void receiveUdpMessageSlot(QByteArray byte);
    void connectStateSlot(bool);
    void disconnectStateSlot(bool);
private:
    void _parseRobotState(QJsonObject obj); //解析ros返回数据
    void _getLaserData();//获取激光数据
    void _ackLoadMap(QJsonObject rootObj);//返回加载地图回应
    void _ackInitialPose(QJsonObject rootObj);//初始化点回应
    void _ackMoveGoal(QJsonObject rootObj);//前往目标点回应
    void _ackMoveGoaFile(QJsonObject rootObj);//靠右行驶前往目标点的回应
    void _ackOpenDoor(QJsonObject rootObj);//开门回应
    void _ackCloseDoor(QJsonObject rootObj);//关门回应
    void _uploadError(QJsonObject rootObj);//错误信息
    void _ackScanDoor(QJsonObject rootObj);//呼叫电梯回应
    void _ackRideElevator(QJsonObject rootObj);//乘坐电梯应答
    void _ackUpdateMap(QJsonObject rootObj);//更新地图应答
signals:
    void robotStateChangedSignal();
    void loadMapSignal(ROS::RETURN_STATE);//加载地图的回应
    void initialPoseSignal(ROS::RETURN_STATE);//初始化点的回应
    void moveGoalSignal(ROS::RETURN_STATE,QString);//前往目标点的回应
    void moveGoalFileSignal(ROS::RETURN_STATE);//靠右行驶到达目标前的回应
    void openDoorSignal(ROS::RETURN_STATE);//开门回应
    void closeDoorSignal(ROS::RETURN_STATE);//关门回应
    void rideElevatorSignal(ROS::RETURN_ELEVATOR_STATE);//乘坐电梯的回应
    void scanDoorSignal(quint8);//呼叫电梯的回应
    void laserDataChangedSignal(QString); //激光数据发生变化
    void chargeStateChangedSignal(bool); //是否在充电
    void collisionStateChangedSignal(bool); //碰撞是否异常
    void curValueChangedSignal(double); //电流发生变化
    void doorStateChangedSignal(bool);//门的状态发生变化
    void emergencyStateChangedSignal(bool);//急停按钮是否触发
    void imuStateChangedSignal(bool);//陀螺仪是否异常
    void mahValueChangedSignal(quint8);//电池电量发生变化
    void mapFileChangedSignal(QString);//地图路径发生变化
    void mcuHwVersionChangedSignal(QString);//下位机版本发生变化
    void mcuSoftVersionChangedSignal(QString);//下位机软件版本发生变化
    void milsValueChangedSignal(quint64);//里程数发生变化
    void relayChangedSignal(quint8);//继电器状态发生变化
    void rosHwVersionChangedSignal(QString);//ros下位机版本
    void rosSoftVersionChangedSignal(QString);//ros软件版本
    void skipChangedSignal(quint8);//环境匹配度 0～100 值越大越不匹配
    void sonarStateChangedSignal(bool);//超声波是否异常
    void volValueChangedSignal(quint16);//电压
    void wheelStateChangedSignal(bool);//轮子是否异常
    void robotPointChangedSignal(double,double,double);//坐标xy及角度
    void connectRosIsSuccessSignal(bool);//是否连接Ros成功
    void disconnectRosIsSuccessSignal(bool);//是否断开Ros成功
    void receiveRosErrorSignal(quint32,QString);
    void updateMapSignal(ROS::ROS::UPDATE_MAP_ENUM);
    void receiveUDPDataSignal();
public:
    ROBOT_STATE_VALUE robotStateValue;//实时
    int _moveGoalResult;
    int _moveGoalStatus;
    QMap<QString,QString> operatorNameMap;
private:
    ROSTCPChater* rosTcpChater;
    ROSUDPChater* rosUdpChater;
    bool _isJustRun = true;//是否刚刚启动
    bool _isJustGetLarserData = true;//是否第一次请求激光数据
    bool _isJustReceiveLarserData = false;//是否第一次返回激光数据
    bool _isJustReceiveUdpData=true;
    bool _dreamDoorState=false;//想要的门的状态
    qint64 _currentErrorCode=-1;//初始化-1,真实的错误码为非负数
    quint8 _connectTimerCount=0;
};

#endif // ROS_H
