#ifndef P_STATUSBAR_H
#define P_STATUSBAR_H

#include "HMI/a_logic.h"

class StatusBar : public QWidget{
    Q_OBJECT
public:
    explicit StatusBar(QWidget *parent = nullptr);
    ~StatusBar();
public slots:
    void timeLabelUpdateSlot();
    void mahValueChangedSlot(quint8);
    void chargeStateChangedSlot(bool);
    void emergencyStateChangedSlot(bool);
    void collisionStateChangedSlot(bool);
    void wheelStateChangedSlot(bool);
    void imuStateChangedSlot(bool);
    void sonarStateChangedSlot(bool);
    void pingHttpIsSuccessSlot(bool);
    void connectRosIsSuccessSlot(bool);
    void connectMqIsSuccessSlot(bool);
    void disconnectRosIsSuccessSlot(bool);
    void receiveRosErrorSlot(quint32 flag,QString info); //-32768~32767
    void receiveLogicTextSlot(QString);
    void receiveUDPDataSlot();
private:
    QSound* _noPowerSound;
    QHBoxLayout* _layout;
    QHBoxLayout* _layout00;
    PowerFrame* _powerFrame;
    SignalFrame* _signalFrame;
    QLabel* _operatorNameLabel;
    QLabel* _powerLabel;
    QLabel* _timeLabel;
    PixmapLabel* _sonarLabel;
    PixmapLabel* _collisionLabel;
    PixmapLabel* _imuLabel;
    PixmapLabel* _wheelLabel;
    PixmapLabel* _stopLabel;
    PixmapLabel* _rosLabel;
    PixmapLabel* _mqLabel;
    PixmapLabel* _httpLabel;
    PixmapLabel* _logoLabel;
    QLabel* _logicLabel;
    TipLabel* _tipLabel;
    quint8 _timerCount=0;
    quint8 _lowPowerTimerCount = 0;
    quint8 _tipCount=0;
    bool _isCharging;
};

#endif // P_STATUSBAR_H
