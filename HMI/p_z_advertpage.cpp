#include "p_z_advertpage.h"

MAdvertPage::MAdvertPage(QWidget *parent) : QFrame(parent){
    _checkBoxSlide = new SlideFrame(this);
    _checkBoxSlide->setFixedWidth(390);
    _updateADButton = new CommonButton(this);
    _updateADButton->setText("更新广告");
    _updateADButton->setPixmap(":/Image/ButtonUpdateAdvert.png");
    _elevatorSwitchItem = new SwitchItemFrame(this);
    _elevatorSwitchItem->setName("电梯中使用");
    _elevatorSwitchItem->setTip("在电梯中播放左侧文字对应语音");
    _elevatorSwitchItem->setPixmap(":/Image/SwitchAdvertElevator.png");
    _testAdSwitchItem = new SwitchItemFrame(this);
    _testAdSwitchItem->setName("试听");
    _testAdSwitchItem->setTip("试听广告语");
    _testAdSwitchItem->setPixmap(":/Image/SwitchTestAdvert.png");
    _advertText = new QTextEdit(this);
    _advertText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _advertText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _advertText->setReadOnly(true);
    _advertText->setStyleSheet("background-color:transparent;color:white;border:none;font:13pt;");
    _checkBoxSlide->addItem(_testAdSwitchItem);
    _checkBoxSlide->addItem(_elevatorSwitchItem);
    _layout=new QGridLayout(this);
    _layout->setContentsMargins(30,30,30,30);
    _layout->setHorizontalSpacing(60);
    _layout->addWidget(_advertText,0,0,1,1);
    _layout->addWidget(_checkBoxSlide,0,1,1,1);
    _layout->addWidget(_updateADButton,1,0,1,1,Qt::AlignCenter);
    initAdvertList();
    connect(_updateADButton,&QPushButton::clicked,[](){http->advertList();});
    connect(http,SIGNAL(advertListSignal(QStringList)),this,SLOT(advertListSlot(QStringList)));
    connect(_elevatorSwitchItem,SIGNAL(changedSignal(bool)),this,SLOT(elevatorSwitchChangedSlot(bool)));
    connect(_testAdSwitchItem,SIGNAL(changedSignal(bool)),this,SLOT(testAdSwitchChangedSlot(bool)));
}
void MAdvertPage::initAdvertList(){
    QString advert;
    quint8 count=0;
    QStringList advertList = config->getValue("advertWordList").toVariant().toStringList();
    foreach(advert,advertList){
        count++;
        _advertText->append(QString("<p>%1.    %2</p>").arg(QString::number(count,10)).arg(advert));
    }
    _elevatorSwitchItem->setBool(config->getValue("isUsingElevatorAdvert").toBool());
    _testAdSwitchItem->setBool(false);
}
void MAdvertPage::advertListSlot(QStringList advertList){
    _advertText->clear();
    QString advert;
    quint8 count=0;
    foreach(advert,advertList){
        count++;
        _advertText->append(QString("<p>%1.    %2</p>").arg(QString::number(count,10)).arg(advert));
    }
    emit hmi->createAdvertVideoSignal();
}
void MAdvertPage::elevatorSwitchChangedSlot(bool isTrue){
    config->setValue("isUsingElevatorAdvert",isTrue);
}
void MAdvertPage::testAdSwitchChangedSlot(bool isTrue){
    if(isTrue){
        voice->playRoundAdvert(-1);
    }else{
        voice->stop();
    }
}
