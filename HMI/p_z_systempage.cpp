#include "p_z_systempage.h"

MSystemPage::MSystemPage(QWidget *parent) : QFrame(parent){
    _initButton = new CommonButton(this);
    _exitUiSoftButton = new CommonButton(this);
    _restartMidSoftButton = new CommonButton(this);
    _shutdownButton = new CommonButton(this);
    _initButton->setText("初始化");
    _exitUiSoftButton->setText("退出界面");
    _restartMidSoftButton->setText("重启中位机");
    _shutdownButton->setText("关机");
    _initButton->setPixmap(":/Image/ButtonSystemInit.png");
    _exitUiSoftButton->setPixmap(":/Image/ButtonSystemExitUi.png");
    _restartMidSoftButton->setPixmap(":/Image/ButtonSystemRestart.png");
    _shutdownButton->setPixmap(":/Image/ButtonSystemExit.png");
    _layout1=new QHBoxLayout();
    _layout1->setContentsMargins(0,0,0,0);
    _layout1->addWidget(_initButton);
    _layout1->addWidget(_exitUiSoftButton);
    _layout1->addWidget(_restartMidSoftButton);
    _layout1->addWidget(_shutdownButton);

    _checkBoxSlide = new SlideFrame(this);
    _isUseLowPowerVoiceBox = new SwitchItemFrame(this);
    _powerBox = new SwitchItemFrame(this);
    _isUsingAutoUpdateMapBox=new SwitchItemFrame(this);
    _isUseLowPowerVoiceBox->setName("低电量提示音");
    _powerBox->setName("充电控制");
    _isUsingAutoUpdateMapBox->setName("自动更新地图");
    _isUseLowPowerVoiceBox->setTip("电量小于30%时语音提示");
    _powerBox->setTip("连接电源 打开充电 关闭断电");
    _isUsingAutoUpdateMapBox->setTip("送货时实时更新地图");
    _isUseLowPowerVoiceBox->setPixmap(":/Image/ButtonSystemLowPower.png");
    _powerBox->setPixmap(":/Image/ButtonSystemCharge.png");
    _isUsingAutoUpdateMapBox->setPixmap(":/Image/ButtonSystemMap.png");
    _checkBoxSlide->addItem(_isUseLowPowerVoiceBox);
    _checkBoxSlide->addItem(_powerBox);
    _checkBoxSlide->addItem(_isUsingAutoUpdateMapBox);

    _lowerSoftFrame=new SoftwareFrame(this);
    _midleSoftFrame=new SoftwareFrame(this);
    _upperSoftFrame=new SoftwareFrame(this);
    _lowerSoftFrame->setTitle("下位机");
    _midleSoftFrame->setTitle("中位机");
    _upperSoftFrame->setTitle("上位机");
    _upperSoftFrame->setTip("提示信息：下载完成后，点击 退出界面，再重新打开即可");
    _layout0=new QVBoxLayout();
    _layout0->addWidget(_upperSoftFrame);
    _layout0->addWidget(_midleSoftFrame);
    _layout0->addWidget(_lowerSoftFrame);

    _doorBox = new SwitchItemFrame(this);
    _doorBox->setName("车门");
    _doorBox->setTip("打开车门或关闭车门");
    _doorBox->setPixmap(":/Image/ButtonSystemDoor.png");

    _layout=new QGridLayout(this);
    _layout->setContentsMargins(30,30,30,30);
    _layout->setSpacing(50);
    _layout->addLayout(_layout0,0,0,1,1);
    _layout->addLayout(_layout1,1,0,1,1);
    _layout->addWidget(_checkBoxSlide,0,1,1,1);
    _layout->addWidget(_doorBox,1,1,1,1);
    _layout->setColumnStretch(0,7);
    _layout->setColumnStretch(1,3);
    _layout->setRowStretch(0,7);
    _layout->setRowStretch(1,3);
    setLayout(_layout);

    initBox();
    connect(_isUseLowPowerVoiceBox,SIGNAL(changedSignal(bool)),this,SLOT(isUseLowPowerChangedSlot(bool)));
    connect(_doorBox,SIGNAL(changedSignal(bool)),this,SLOT(doorSlot(bool)));
    connect(_powerBox,SIGNAL(changedSignal(bool)),this,SLOT(powerSlot(bool)));
    connect(_isUsingAutoUpdateMapBox,SIGNAL(changedSignal(bool)),this,SLOT(isUsingAutoUpdateMapChangedSlot(bool)));
    connect(_initButton,&QPushButton::clicked,[](){logic->initRobot();});
    connect(_exitUiSoftButton,&QPushButton::clicked,[](){logic->exitApp();});
    connect(_restartMidSoftButton,&QPushButton::clicked,[](){ros->restartMiddleMachine();});
    connect(_shutdownButton,&QPushButton::clicked,[](){ros->shutPower();});
    connect(ros,SIGNAL(openDoorSignal(ROS::RETURN_STATE)),this,SLOT(openDoorSlot(ROS::RETURN_STATE)));
    connect(ros,SIGNAL(closeDoorSignal(ROS::RETURN_STATE)),this,SLOT(closeDoorSlot(ROS::RETURN_STATE)));
    connect(ros,SIGNAL(chargeStateChangedSignal(bool)),this,SLOT(chargeStateChangedSlot(bool)));
    connect(ros,SIGNAL(mcuSoftVersionChangedSignal(QString)),this,SLOT(lowerLocalVersionChangedSlot(QString)));
    connect(ros,SIGNAL(rosSoftVersionChangedSignal(QString)),this,SLOT(midleLocalVersionChangedSlot(QString)));
    connect(http,SIGNAL(checkSoftwareSignal(HTTP::SOFT_TYPE,SoftwareFrame::CLOUD)),this,SLOT(checkSoftwareSlot(HTTP::SOFT_TYPE,SoftwareFrame::CLOUD)));
    connect(_upperSoftFrame,&SoftwareFrame::finishedSignal,[](QString path){ros->updateUpperMachine(path);});
    connect(_midleSoftFrame,&SoftwareFrame::finishedSignal,[](QString path){ros->updateMiddleMachine(path);});
    connect(_lowerSoftFrame,&SoftwareFrame::finishedSignal,[](QString path){ros->updateDownMachine(path);});
}
MSystemPage::~MSystemPage(){
    delete _layout0;
    delete _layout1;
}
void MSystemPage::initBox(){
    _isUseLowPowerVoiceBox->setBool(config->getValue("isUsingLowPowerVoice").toBool());
    _upperSoftFrame->setLocalVersion(VERSON);
    _isUsingAutoUpdateMapBox->setBool(config->getValue("isUsingAutoUpdateMap").toBool());
}
void MSystemPage::isUseLowPowerChangedSlot(bool isTrue){
    config->setValue("isUsingLowPowerVoice",isTrue);
}
void MSystemPage::isUsingAutoUpdateMapChangedSlot(bool isTrue){
    config->setValue("isUsingAutoUpdateMap",isTrue);
}
void MSystemPage::doorSlot(bool isTrue){
    if(isTrue){
        ros->openDoor();
    }else{
        ros->closeDoor();
    }
}
void MSystemPage::powerSlot(bool isTrue){
    if(isTrue){
        ros->openPower();
    }else{
        ros->closePower();
    }
}
void MSystemPage::openDoorSlot(ROS::RETURN_STATE state){
    if(state==ROS::RS_SUCEESS){
        _doorBox->setBool(true);
    }else{
        _doorBox->setBool(false);
    }
}
void MSystemPage::closeDoorSlot(ROS::RETURN_STATE state){
    if(state==ROS::RS_SUCEESS){
        _doorBox->setBool(true);
    }else{
        _doorBox->setBool(false);
    }
}
void MSystemPage::chargeStateChangedSlot(bool isCharge){
    _powerBox->setBool(isCharge);
}
void MSystemPage::lowerLocalVersionChangedSlot(QString version){
    _lowerSoftFrame->setLocalVersion(version);
}
void MSystemPage::midleLocalVersionChangedSlot(QString version){
    _midleSoftFrame->setLocalVersion(version);
}
void MSystemPage::checkSoftwareSlot(HTTP::SOFT_TYPE type,SoftwareFrame::CLOUD cloud){
    switch (type) {
    case HTTP::ST_HMI:
        _upperSoftFrame->setCloud(cloud);
        break;
    case HTTP::ST_ROS:
        _midleSoftFrame->setCloud(cloud);
        break;
    case HTTP::ST_BAN:
        _lowerSoftFrame->setCloud(cloud);
        break;
    default:
        break;
    }
}
