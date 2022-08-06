#include "p_statusbar.h"

StatusBar::StatusBar(QWidget *parent) : QWidget(parent){
    setFixedHeight(25);
    setStyleSheet("background-color:transparent;");
    _timeLabel = new QLabel(this);
    _timeLabel->setStyleSheet("color:#FFFFFF;font:75 15pt \"Ubuntu\"");
    _timeLabel->setMaximumWidth(60);
    _operatorNameLabel=new QLabel(this);
    _operatorNameLabel->setStyleSheet("color:#FFFFFF;font:75 15pt \"Ubuntu\"");
    _signalFrame=new SignalFrame(this);
    _signalFrame->setFixedWidth(30);
    _powerFrame = new PowerFrame(this);
    _powerFrame->setFixedWidth(60);
    _powerFrame->setPower(0);
    _powerLabel = new QLabel(this);
    _powerLabel->setStyleSheet("color:#FFFFFF;font:75 15pt \"Ubuntu\"");
    _powerLabel->setFixedWidth(50);
    _sonarLabel=new PixmapLabel(this);
    _sonarLabel->setFixedSize(21,21);
    _sonarLabel->setPixmap(":/Image/StatusSonar.png");
    _sonarLabel->hide();
    _collisionLabel=new PixmapLabel(this);
    _collisionLabel->setFixedSize(21,21);
    _collisionLabel->setPixmap(":/Image/StatusCollision.png");
    _collisionLabel->hide();
    _imuLabel=new PixmapLabel(this);
    _imuLabel->setFixedSize(21,21);
    _imuLabel->setPixmap(":/Image/StatusImu.png");
    _imuLabel->hide();
    _wheelLabel=new PixmapLabel(this);
    _wheelLabel->setFixedSize(21,21);
    _wheelLabel->setPixmap(":/Image/StatusWheel.png");
    _wheelLabel->hide();
    _stopLabel = new PixmapLabel(this);
    _stopLabel->setFixedSize(21,21);
    _stopLabel->setPixmap(":/Image/StatusStop.png");
    _stopLabel->hide();
    _rosLabel = new PixmapLabel(this);
    _rosLabel->setPixmap(":/Image/StatusRosDisconnect.png");
    _rosLabel->setFixedSize(21,21);
    _mqLabel = new PixmapLabel(this);
    _mqLabel->setPixmap(":/Image/StatusMqDisconnect.png");
    _mqLabel->setFixedSize(21,21);
    _httpLabel = new PixmapLabel(this);
    _httpLabel->setPixmap(":/Image/StatusHttpDisconnect.png");
    _httpLabel->setFixedSize(21,21);
    _layout00 = new QHBoxLayout();
    _layout00->setContentsMargins(0,0,0,0);
    _layout00->setSpacing(5);
    _layout00->addItem(new QSpacerItem(60,30,QSizePolicy::Preferred,QSizePolicy::Expanding));
    _layout00->addWidget(_sonarLabel);
    _layout00->addWidget(_collisionLabel);
    _layout00->addWidget(_imuLabel);
    _layout00->addWidget(_wheelLabel);
    _layout00->addWidget(_stopLabel);
    _layout00->addWidget(_rosLabel);
    _layout00->addWidget(_mqLabel);
    _layout00->addWidget(_httpLabel);
    _layout00->addWidget(_operatorNameLabel);
    _layout00->addWidget(_signalFrame);
    _layout00->addWidget(_timeLabel);
    _layout00->addWidget(_powerFrame);
    _layout00->addWidget(_powerLabel);
    _logicLabel = new QLabel(this);
    _logicLabel->setAlignment(Qt::AlignCenter);
    _logicLabel->setStyleSheet("color:#FFFFFF;font:75 15pt \"Ubuntu\"");
    _tipLabel = new TipLabel(this);
    _tipLabel->setAlignment(Qt::AlignCenter);
    _logoLabel = new PixmapLabel(this);
    _logoLabel->setPixmap(":/Image/Logo.png");
    _logoLabel->setFixedSize(54,25);
    _layout = new QHBoxLayout(this);
    _layout->setContentsMargins(2,2,2,2);
    _layout->setSpacing(5);
    _layout->addWidget(_logoLabel);
    _layout->addWidget(_logicLabel);
    _layout->addWidget(_tipLabel);
    _layout->addLayout(_layout00);
    _layout->setStretch(0,75);
    _layout->setStretch(1,1);
    _layout->setStretch(2,400);
    _layout->setStretch(3,1);
    _layout->setStretch(4,400);
    _layout->setStretch(5,388);
    setLayout(_layout);
    connect(timer,SIGNAL(timer1000TimeoutSignal()),this,SLOT(timeLabelUpdateSlot()));
    connect(ros,SIGNAL(mahValueChangedSignal(quint8)),this,SLOT(mahValueChangedSlot(quint8)));
    connect(ros,SIGNAL(chargeStateChangedSignal(bool)),this,SLOT(chargeStateChangedSlot(bool)));
    connect(ros,SIGNAL(emergencyStateChangedSignal(bool)),this,SLOT(emergencyStateChangedSlot(bool)));
    connect(ros,SIGNAL(collisionStateChangedSignal(bool)),this,SLOT(collisionStateChangedSlot(bool)));
    connect(ros,SIGNAL(imuStateChangedSignal(bool)),this,SLOT(imuStateChangedSlot(bool)));
    connect(ros,SIGNAL(wheelStateChangedSignal(bool)),this,SLOT(wheelStateChangedSlot(bool)));
    connect(ros,SIGNAL(sonarStateChangedSignal(bool)),this,SLOT(sonarStateChangedSlot(bool)));
    connect(http,SIGNAL(pingHttpIsSuccessSignal(bool)),this,SLOT(pingHttpIsSuccessSlot(bool)));
    connect(ros,SIGNAL(connectRosIsSuccessSignal(bool)),this,SLOT(connectRosIsSuccessSlot(bool)));
    connect(ros,SIGNAL(disconnectRosIsSuccessSignal(bool)),this,SLOT(disconnectRosIsSuccessSlot(bool)));
    connect(mq,SIGNAL(connectMqIsSuccessSignal(bool)),this,SLOT(connectMqIsSuccessSlot(bool)));
    connect(ros,SIGNAL(receiveRosErrorSignal(quint32,QString)),this,SLOT(receiveRosErrorSlot(quint32,QString)));
    connect(hmi,SIGNAL(sendLogicTextSignal(QString)),this,SLOT(receiveLogicTextSlot(QString)));
    connect(ros,SIGNAL(receiveUDPDataSignal()),this,SLOT(receiveUDPDataSlot()));
}
StatusBar::~StatusBar(){
    _layout00->deleteLater();
}
void StatusBar::timeLabelUpdateSlot(){
    QTime nowTime = QTime::currentTime();
    _timeLabel->setText(nowTime.toString("hh:mm"));
}
void StatusBar::mahValueChangedSlot(quint8 value){
    _powerFrame->setPower(value);
    _powerLabel->setText(QString("%1%").arg(value));
    //界面提示电池电量低于30%不能接订单
    if(value<30){
        if(!_isCharging){
            if(config->getValue("isUsingLowPowerVoice").toBool()){
                voice->playRoundSound(S_MIDDLE,"noPower.wav",1);
            }
        }else{
            if(config->getValue("isUsingLowPowerVoice").toBool()){
                voice->stop("noPower.wav");
            }
        }
    }else{
        voice->stop("noPower.wav");
    }
}
void StatusBar::chargeStateChangedSlot(bool isCharging){
    _isCharging=isCharging;
    _powerFrame->setCharge(isCharging);
}
void StatusBar::pingHttpIsSuccessSlot(bool isSuccess){
    if(isSuccess){
        _httpLabel->setPixmap(":/Image/StatusHttpConnect.png");
    }else{
        _httpLabel->setPixmap(":/Image/StatusHttpDisconnect.png");
    }
}
void StatusBar::connectRosIsSuccessSlot(bool isSuccess){
    if(isSuccess){
        _rosLabel->setPixmap(":/Image/StatusRosConnect.png");
    }
}
void StatusBar::connectMqIsSuccessSlot(bool isSuccess){
    if(isSuccess){
        _mqLabel->setPixmap(":/Image/StatusMqConnect.png");
    }else{
        _mqLabel->setPixmap(":/Image/StatusMqDisconnect.png");
    }
}
void StatusBar::disconnectRosIsSuccessSlot(bool isSuccess){
    if(isSuccess){
        _rosLabel->setPixmap(":/Image/StatusRosDisconnect.png");
    }else{
        _rosLabel->setPixmap(":/Image/StatusRosConnect.png");
    }
}
void StatusBar::emergencyStateChangedSlot(bool bo){
    if(bo){
        _stopLabel->show();
        if(logic->workState!=HTTP::WS_UNINIT){
            voice->playRoundSound(S_HIGH,"dontTouchStop.wav",3);
        }
    }else{
        _stopLabel->hide();
    }
}
void StatusBar::collisionStateChangedSlot(bool isTrue){
    if(isTrue){
        _collisionLabel->show();
    }else{
        _collisionLabel->hide();
    }
}
void StatusBar::wheelStateChangedSlot(bool isTrue){
    if(isTrue){
        _wheelLabel->show();
    }else{
        _wheelLabel->hide();
    }
}
void StatusBar::imuStateChangedSlot(bool isTrue){
    if(isTrue){
        _imuLabel->show();
    }else{
        _imuLabel->hide();
    }
}
void StatusBar::sonarStateChangedSlot(bool isTrue){
    if(isTrue){
        _sonarLabel->show();
    }else{
        _sonarLabel->hide();
    }
}
void StatusBar::receiveRosErrorSlot(quint32 code,QString detail){
    TipLabel::TIP_INFO tipInfo;
    tipInfo.code=code;
    tipInfo.detail=detail;
    if(code==1){
        tipInfo.type=TipLabel::TT_NORMAL;
    }else{
        tipInfo.type=TipLabel::TT_ERROR;
    }
    _tipLabel->showTip(tipInfo);
}
void StatusBar::receiveLogicTextSlot(QString text){
    _logicLabel->setText(text);
}
void StatusBar::receiveUDPDataSlot(){
    _operatorNameLabel->setText(ros->operatorNameMap.value(ros->robotStateValue.SERVER));
    _signalFrame->setSignal(ros->robotStateValue.CSQ);
}
