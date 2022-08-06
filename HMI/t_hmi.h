#ifndef HMI_H
#define HMI_H

#include <QObject>
#include <QJsonObject>

/*@@@@@@@@@@@界面信号@@@@@@@@@@@*/
class HMI : public QObject{
    Q_OBJECT
public:
    enum PAGE{
        POWER_ON,
        BLINK,
        USER_LOGIN,
        USER_RECEIVE,
        MANAGER_LOGIN,
        MANAGER,
        SHOW_ORDER,
        USER
    };
    enum MANAGER_PAGE{
        ORDER,
        ADVERT,
        SYSTEM,
        SOUND,
        CAMERA,
        ROBOTINFO,
    };
    enum PACK_TYPE{
        PACK_JINGBAOZHUANG=1,
        PACK_YUBAOZHUANG=2,
        PACK_SANBAOZHUANG=3
    };
    enum USER_PAGE{
        RECEIVE,
        RETURN
    };
    // qint16 //-32768~32767 >0 ERROR <0 WARN
    enum TIP_TYPE{
        TIP_WARN,
        TIP_ERROR,
    };
    enum VOICE_PERSON{
        DUXIAOYU=1,//度小宇
        DUXIAOMEI=0,//度小美
        DUXIAOYAO=3,//度逍遥
        DUYAYA=4,//度丫丫
    };
    /*@@@@@@@@@@@@@@@@@@@@@*/
    //这四个是基础音库
    //精品音库要钱（度逍遥（精品）=5003，度小鹿=5118，度博文=106，度小童=110，度小萌=111，度米朵=103，度小娇=5）
    struct VOICE_INFO{
        quint8 spd=5;//语速，取值0-15，默认为5中语速
        quint8 pit = 5;//音调，取值0-15，默认为5中语调
        quint8 vol = 5;//音量，取值0-15，默认为5中音量（取值为0时为音量最小值，并非为无声）
        HMI::VOICE_PERSON per = HMI::DUXIAOMEI;//默认为度小美
    };
    struct GOODS_INFO{                        //订单商品列表
        qint64 orderGoodsID;
        qint64 goodsID;
        qint64 speID;
        QString goodsNumber;
        QString goodsName;
        QString speInfo;
        QString listPicture;
        qint16 unitWeight;
        double sellingPrice;
        qint32 buyNumber;
        HMI::PACK_TYPE packType;
    };
public:
    explicit HMI(QObject *parent = nullptr);
signals:
    void setPageSignal(HMI::PAGE);//设置主页面
    void setFuctionSignal(HMI::MANAGER_PAGE);//设置设置页面
    void sendLogicTextSignal(QString);//设置状态栏当前动作文字
    void exitSignal();//退出软件
    void soundIsFinishedSignal(QString);//声音播放完成
    void createAdvertVideoSignal();
    void setAutoUpdateMapBoxEnabledSignal(bool);
};

#endif // HMI_H
