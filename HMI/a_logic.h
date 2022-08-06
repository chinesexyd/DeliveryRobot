#ifndef LOGIC_H
#define LOGIC_H

#include "HMI/t_http.h"
#include "HMI/t_tts.h"
#include "HMI/t_camera.h"
#include "HMI/t_voice.h"
#include "HMI/t_mqserver.h"
#include "HMI/t_obs.h"

/*@@@@@@@@@@@@@@@@@@@@*/
extern HTTP* http;
extern Camera* camera;
extern TTS* tts;
extern Voice* voice;
extern MQServer* mq;
extern OBS* obs;
/*@@@@@@@@@@@检查网络连接状态@@@@@@@@@@@*/
class CheckNetWorkStateThread:public QThread{
    Q_OBJECT
public:
    explicit CheckNetWorkStateThread(QObject* parent=nullptr);
protected:
    void run() override;
private slots:
    void timerTimeoutSlot();
private:
    quint8 _timerCount=0;
};
/*@@@@@@@@@@@@@@@@@@@@*/
class Logic: public QObject{
    Q_OBJECT
public:
    struct POINT_TAG{
        QString initWait="B";       //初始化点
        QString returnWait = "C";   //返航申请的点，靠右行驶结束后前往
        QString custormer = "D";    //客户门口点
        QString SEW = "E";          //Sendgoods Elevator Wait point
        QString CEW="F";            //Return Elevator Wait point
    };
    enum ERROR{
        ERR_UnableToApplyForElevator=3000, //请求不到电梯
    };
public:
    explicit Logic();
    void firstStart();                              //初次启动的准备工作
    void initRobot();                               //初始化点
    void startSendGoods();                          //开始送货
    void checkMsgCode(QString code);                //检查验证码
    void confirmGoods();                            //确认收货
    void againSendGoods();                          //再次送货
    void cancelOrder();                             //取消订单
    void resetOrderState();                         //滞留重置订单状态
    void exitApp();
private:
    void connectSignals();                          //连接信号
    void makeDirs();                                //创建文件夹
    void startUpdateMap();                          //开始更新地图
    void stopUpdateMap();                           //结束更新地图
public slots:
    void loadMapSlot(ROS::RETURN_STATE);                    //初始化地图应答
    void initialPoseSlot(ROS::RETURN_STATE);                //初始化点应答
    void moveGoalSlot(ROS::RETURN_STATE,QString);           //前往目标点应答
    void moveGoalFileSlot(ROS::RETURN_STATE);               //靠右行驶应答
    void rideElevatorSlot(ROS::RETURN_ELEVATOR_STATE);      //Ros告知出电梯了
    void mapFileChangedSlot(QString);                       //切换楼层
    void receiveRosErrorSlot(quint32,QString);              //ROS报错
    void updateMapSlot(ROS::ROS::UPDATE_MAP_ENUM);          //更新地图
    void relayRobotStateSlot();                             //回应MQ小车当前状态
    void receivePickupCodeSlot(QJsonObject);                //用户通过手机端打开车门取货验证
    void dealOrderInfoSlot(QJsonObject);                    //接收到订单
    void relayNoteSlot(quint64);                            //上传日志
    void initCommunityConfigIsSuccessSlot(bool);            //初始化配置成功
    void carActionSlot(HTTP::POINT_INFO);                   //http告知避让点位置
    void lockElevatorSlot(QJsonObject);                     //请求电梯应答
    void unlockElevatorIsSucessSlot(bool);                  //释放电梯应答
    void trackOrderIsSuccessSlot(bool);                     //上报订单是否成功
    void resetOrderIsSuccessSlot(qint32);                     //重置订单是否成功
    void receiveGoodsTimerSlot();                           //计时
    void receiveUDPDataSlot();
public:
    HTTP::WORK_STATE workState;
    HTTP::ORDER_STATE orderState;
    MQServer::ORDER_INFO orderInfo;
private:
    CheckNetWorkStateThread* _checkNetWorkStateThread;
    QTimer* _receiveGoodsTimer;
    struct{
        bool isAWEPGo=false;        //送货是否抵达等待电梯的点
        bool isAWEPBack=false;      //返航是否抵达等待电梯的点
        QString EMacGo;             //送货时电梯Mac 用于释放电梯
        QString EMacBack;           //返回时电梯Mac 用于释放电梯
        XYA_POINT EWEP_GO;          //送货等待电梯到达的点
        XYA_POINT EWEP_BACK;        //返航等待电梯到达的点
        qint64 currentFloorID=0;    //用于汇报小车当前楼层
        POINT_TAG pointTag;         //前往某点的标记
        quint32 timerCount=0;       //用于计时
        quint32 codeTrueTimerCount; //用户输入验证码正确的时间
        bool isSavePoint=false;     //是否保存点
        XYA_POINT savePoint;        //返航出电梯，暂存避让点
        bool isCodeTrue=false;      //验证码是否正确
        bool isLockButton=false;    //为防止多点按钮出错
        bool canUpdateMap=true;     //能够上传地图差异
        qint64 elevatorID;         //用于汇报电梯ID
        bool isSendCarSysInfo=false; //是否发送小车基本信息
        bool isInit=false;              //是否初始化
        bool isLockElevator=false;  //在申请电梯后并且未释放电梯时关闭软件时释放电梯
        QStringList iWantGoFloor;
        quint64 applyElevatorCount=0;
    }logicParams;
};
/*@@@@@@@@@@@@@@@@@@@@*/
extern Logic* logic;

#endif // LOGIC_H
