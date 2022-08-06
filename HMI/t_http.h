#ifndef HTTP_H
#define HTTP_H

#include "a_global.h"
#include <private/qzipreader_p.h>
#include <private/qzipwriter_p.h>

class HTTP:public QObject{
    Q_OBJECT
public:
    enum ACTION_POINT{
        AP_INIT=0,               //初始化点
        AP_WAIT=1,               //等待点
        AP_WAY=2                 //离开送货
    };
    enum WORK_STATE{
        WS_UNINIT=0,                //未初始化
        WS_UNUSED=1,                //闲置
        WS_RECEIVED_ORDER=2,        //已接单
        WS_RECEIVED_GOODS=3,        //已接货 用不到
        WS_SENDING_GOODS=4,         //送货中
        WS_RETURN=5                 //返航
    };
    enum ORDER_STATE{
        OS_ORDER_TAKING=202,                //接单
        OS_DELIVER_GOODS=203,               //发货中
        OS_WAITING_FOR_TAKE_GOODS=210,      //等待取货
        OS_FINISHED_TAKING_GOODS=301,       //取完货
        OS_DETENTION=405,                   //滞留
        OS_DETENTION_REFUND=406,            //滞留退款
        OS_DETENTION_REFUSAL_REFUND=407,    //滞留拒退
        OS_ELEVATOR_ENTRANCE=205,           //电梯口
        OS_IN_ELEVATOR=206                  //电梯内
    };
    enum LOGISTICS_STATE{           //物流状态
        LS_ORDER_TAKING=201,        //接单
        LS_ORDER_START=301,         //送单
        LS_ORDER_SENDING=305,       //送货中
        LS_RETURN=401,              //返回
        LS_RETURNING=405,           //返航中
        LS_FINISH=501,              //完成
    };
    enum HTTP_CODE{
        CODE_OK=200,            //正常
        CODE_UNAUTHORIZED=401,  //未授权
        CODE_FORBIDDEN=403,     //被禁止
        CODE_NOTFOUND=404       //未找到
    };
    struct PARAM_UploadCarSysInfo{
        QString cameraModel;    //摄像头型号
        qint32 carID;           //无人车ID
        QString carIP;          //无人车IP
        QString driveModel;     //驱动轮型号
        QString emergencyModel; //急停型号
        QString imei;           //imei串号
        QString imuModel;       //IMU型号
        QString larserModel;    //激光型号
        QString lowerVersion;   //下位机版本
        QString operatorName;   //运营商名称
        QString osVersion;      //系统版本
        QString routerIP;       //路由器IP
        QString routerMac;      //路由器MAC
        QString routerModel;    //路由器型号
        QString routerVersion;  //路由器版本
        QString sonarModel;     //声纳型号
        QString uccidNumber;    //uccid卡号
        QString uiVersion;      //界面版本
        QString uuID;           //下位机UUID
    };
    struct PARAM_DevWarningReport{
        qint64 buildingID=0;        //当前楼栋ID
        qint64 cuyID=0;             //小区ID
        qint32 deviceID=0;          //设备ID
        qint32 deviceType=1;        //设备类型：1无人车、2收银机、3梯控
        qint64 elevatorID=0;        //当前电梯ID
        QString errorCode;          //错误码
        QString errorRemarks;       //报警描述
        qint64 floorID=0;           //当前楼层
        qint64 logisticsID=0;       //物流单ID
        XYA_POINT point;
        qint64 unitID=0.0;          //当前单元ID
    };
    enum CODE_VALUE{
        CV_DISABLE=1016,        //无人车禁用
        CV_UNAUTHORIZED=1017,   //未授权
        CV_UNBIND=1018,         //未绑定
        CV_NORMAL=0             //正常
    };
    struct COMMUNITY_INFO{
        qint32 carID;
        QString carNumber;
        QString carMac;
        QString carName;
        qint32 carType;
        qint32 carState;
        QString carPassword;
        qint64 cuyID;
        qint32 deviceState;
        bool isSucess=false;
    };
    struct POINT_INFO{
        qint16 pointID;
        qint16 cuyID;
        QString pointName;
        qint16 pointType;
        XYA_POINT xyaPoint;
    };
    struct INIT_COMMUNITY_CONFIG_INFO{
        struct CUYINFO{
            qint16 cuyID;
            QString cuyName;
            QString cuyAddress;
            qint64 provinceID;
            qint64 cityID;
            qint64 countyID;
            QString districtName;
            QString countyNumber;
            double latitude;
            double logitude;
            QString facadeUrl;
            qint32 businessState;
            QString businessTime;
            QString personName;
            QString phoneNumber;
            qint32 operatorID;
            QString mapFile;
            qint16 locationFloor;
            QString mapPath;         
        }cuyInfo;
        POINT_INFO initPoint;
        QMap<QString,QMap<QString,XYA_POINT>> buildingList;
    };
    enum SOFT_TYPE{
        ST_HMI,
        ST_ROS,
        ST_BAN
    };
private:
    struct BASE_URL{
//        const QString test="192.168.100.24";    //测试地址
        const QString test="mqs-test.k8s.jilida.net.cn";    //测试地址
        const QString preview="mqs.preview.jilida.net.cn";  //预发布地址
        const QString real="mqs.jilida.net.cn";             //正式地址
    };
    struct EXACT_URL{
        const QString initCommunityConfig="/center/community/initCommunityConfig";
        const QString advertList="/center/advert/advertList";
        const QString carAction="/device/car/carAction";
        const QString startingUp="/device/car/startingUp";
        const QString uploadCarSysInfo="/device/car/uploadCarSysInfo";
        const QString sendSmsCode="/center/sms/sendSmsCode";
        const QString resetOrder="/branch/order/resetOrder";
        const QString trackOrder="/branch/order/trackOrder";
        const QString devWarningReport="/device/warning/devWarningReport";
        const QString uploadLog="/device/log/uploadLog";
        const QString lockElevator="/device/elevator/lockElevator";
        const QString unlockElevator="/device/elevator/unlockElevator";
        const QString sendVoiceCode="/center/voice/sendVoiceCode";
        const QString checkSoftware="/sys/software/checkSoftware";
    };
    struct URL{
        QString initCommunityConfigUrl; //初始化小区配置
        QString advertListUrl;          //小区广告列表
        QString carActionUrl;           //无人车调度执行
        QString startingUpUrl;          //开机接口
        QString uploadCarSysInfoUrl;    //上传小车系统信息
        QString sendSmsCodeUrl;         //发送短信验证码
        QString resetOrderUrl;          //重置订单状态
        QString trackOrderUrl;          //更新订单状态
        QString devWarningReportUrl;    //设备警报上报
        QString uploadLogUrl;           //日志上传
        QString lockElevatorUrl;        //锁定电梯
        QString unlockElevatorUrl;      //解锁电梯
        QString sendVoiceCodeUrl;       //发送语音验证码
        QString checkSoftwareUrl;       //软件检查
    };
    struct SOFT_TAG{
        QString hmi="HMI";//界面
        QString ros="ROS";//中位机
        QString ban="BAN";//下位机 板子
    };
    struct HTTP_REQUEST{
       QString sendSmsCode_code;
       QString sendSmsCode_mobile;
       QString sendVoiceCode_code;
       QString sendVoiceCode_mobile;
       qint32 lockElevator_startFloor;
       qint32 lockElevator_targetFloor;
       qint64 lockElevator_unitID;
       QString unlockElevator_elevatorMac;
    };
    struct HTTP_TIME_CHECK_INFO{
        bool isRequest=false;       //是否发送请求
        quint8 timeCount=0;         //计时
        quint8 overtime=30;         //超时时间 30为3s 1个单位为100ms
    };
public:
    explicit HTTP(QObject* parent=nullptr);
    ~HTTP();
    void isConnect();
    void initCommunityConfig();
    void advertList();
    void carAction(HTTP::ACTION_POINT pointAction,HTTP::WORK_STATE workState);
    void startingUp();
    void uploadCarSysInfo(HTTP::PARAM_UploadCarSysInfo uploadCarSysInfo);
    void sendSmsCode(QString code,QString mobile);
    void devWarningReport(HTTP::PARAM_DevWarningReport devWarningReport);
    void uploadLog(qint64 timestamp,QString pictureName);
    void lockElevator(qint32 startFloor,qint32 targetFloor,qint64 unitID);
    void unlockElevator(QString elevatorMac);
    void sendVoiceCode(QString code,QString mobile);
    void trackOrder(qint64 logisticsID,qint64 orderID,HTTP::ORDER_STATE orderState,LOGISTICS_STATE logisticsState);
    void resetOrder(qint32 carID,qint64 logisticsID,qint64 orderID);
    void checkSoftware(HTTP::SOFT_TYPE type);
private:
    void _initCommunityConfig(QJsonObject obj);
    void _advertList(QJsonObject obj);
    void _carAction(QJsonObject obj);
    void _startingUp(QJsonObject obj);
    void _uploadCarSysInfo(QJsonObject obj);
    void _sendSmsCode(QJsonObject obj);
    void _devWarningReport(QJsonObject obj);
    void _uploadLog(QJsonObject obj);
    void _lockElevator(QJsonObject obj);
    void _unlockElevator(QJsonObject obj);
    void _sendVoiceCode(QJsonObject obj);
    void _trackOrder(QJsonObject obj);
    void _resetOrder(QJsonObject obj);
    void _checkSoftware(QNetworkReply* reply);
    void post(QString url,QByteArray bytes);
    QString getHost();
public slots:
    void finishedSlot(QNetworkReply* reply);
    void timer100TimeoutSlot();
signals:
    void pingHttpIsSuccessSignal(bool);
    void initCommunityConfigIsSuccessSignal(bool);
    void advertListSignal(QStringList);
    void advertListIsSucessSignal(bool);
    void carActionSignal(HTTP::POINT_INFO);
    void carActionIsSucessSignal(bool);
    void startingUpSignal(HTTP::CODE_VALUE);
    void uploadCarSysInfoIsSuccessSignal(bool);
    void sendSmsCodeIsSuccessSignal(bool);
    void devWarningReportIsSuccessSignal(bool);
    void uploadLogIsSuccessSignal(bool);
    void lockElevatorSignal(QJsonObject obj);
    void unlockElevatorIsSucessSignal(bool);
    void sendVoiceCodeIsSuccessSignal(bool);
    void trackOrderIsSuccessSignal(bool);
    void resetOrderIsSuccessSignal(qint32);
    void checkSoftwareSignal(HTTP::SOFT_TYPE,SoftwareFrame::CLOUD);
public:
    COMMUNITY_INFO commnuityInfo;
    INIT_COMMUNITY_CONFIG_INFO initCommunityConfigInfo;
private:
    QNetworkAccessManager* _netManager;
    QNetworkAccessManager* _downloadManager;
    QHttpMultiPart* _multiPart=nullptr;
    BASE_URL _baseUrl;
    EXACT_URL _exactUrl;
    URL _url;
    SOFT_TAG _softTag;
    HTTP_REQUEST _httpRequest;
    QMap<QString,HTTP_TIME_CHECK_INFO> _checkInfo;
};
#endif // HTTP_H
