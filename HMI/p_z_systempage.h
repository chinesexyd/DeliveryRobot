#ifndef MDEBUGPAGE_H
#define MDEBUGPAGE_H

#include "HMI/a_logic.h"
/*@@@@@@@@@@@@@@@@@@@@@*/
class MSystemPage : public QFrame{
    Q_OBJECT
public:
    explicit MSystemPage(QWidget *parent = nullptr);
    ~MSystemPage();
private:
    void initBox();
public slots:
    void isUseLowPowerChangedSlot(bool);
    void doorSlot(bool);
    void powerSlot(bool);
    void isUsingAutoUpdateMapChangedSlot(bool);
    void openDoorSlot(ROS::RETURN_STATE);
    void closeDoorSlot(ROS::RETURN_STATE);
    void chargeStateChangedSlot(bool);
    void lowerLocalVersionChangedSlot(QString);
    void midleLocalVersionChangedSlot(QString);
    void checkSoftwareSlot(HTTP::SOFT_TYPE,SoftwareFrame::CLOUD);
private:
    CommonButton* _initButton;//初始化
    CommonButton* _exitUiSoftButton;//退出界面程序
    CommonButton* _restartMidSoftButton;//重启中位机
    CommonButton* _shutdownButton;//关机
    SlideFrame* _checkBoxSlide;
    SwitchItemFrame* _isUseLowPowerVoiceBox;
    SwitchItemFrame* _doorBox;
    SwitchItemFrame* _isUsingAutoUpdateMapBox;
    SwitchItemFrame* _powerBox;
    SoftwareFrame* _lowerSoftFrame;
    SoftwareFrame* _midleSoftFrame;
    SoftwareFrame* _upperSoftFrame;
    QVBoxLayout* _layout0;
    QHBoxLayout* _layout1;
    QVBoxLayout* _layout2;
    QGridLayout* _layout;
};

#endif // MDEBUGPAGE_H
