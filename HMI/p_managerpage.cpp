#include "p_managerpage.h"

/*@@@@@@@@@@@@@@@@@@@@*/
ManagerPage::ManagerPage(QWidget *parent) : ManagerBackgroundFrame(parent){
    _returnButton=new ReturnButton(this);
    _returnButton->setCheckable(false);
    _returnButton->setFixedSize(40,40);
    _orderButton = new MenuButton(this);
    _soundButton = new MenuButton(this);
    _advertButton = new MenuButton(this);
    _systemButton = new MenuButton(this);
    _cameraButton = new MenuButton(this);
    _robotInfoButton  = new MenuButton(this);
    _orderButton->setCheckedPixmap(":/Image/ButtonOrderChecked.png");
    _orderButton->setUncheckedPixmap(":/Image/ButtonOrderUnchecked.png");
    _soundButton->setCheckedPixmap(":/Image/ButtonVoiceChecked.png");
    _soundButton->setUncheckedPixmap(":/Image/ButtonVoiceUnchecked.png");
    _advertButton->setCheckedPixmap(":/Image/ButtonAdvertChecked.png");
    _advertButton->setUncheckedPixmap(":/Image/ButtonAdvertUnchecked.png");
    _systemButton->setCheckedPixmap(":/Image/ButtonSystemChecked.png");
    _systemButton->setUncheckedPixmap(":/Image/ButtonSystemUnchecked.png");
    _cameraButton->setCheckedPixmap(":/Image/ButtonCameraChecked.png");
    _cameraButton->setUncheckedPixmap(":/Image/ButtonCameraUnchecked.png");
    _robotInfoButton->setCheckedPixmap(":/Image/ButtonRobotChecked.png");
    _robotInfoButton->setUncheckedPixmap(":/Image/ButtonRobotUnchecked.png");
    _buttonGroup = new QButtonGroup(this);
    _buttonGroup->addButton(_orderButton);
    _buttonGroup->addButton(_advertButton);
    _buttonGroup->addButton(_soundButton);
    _buttonGroup->addButton(_cameraButton);
    _buttonGroup->addButton(_systemButton);
    _buttonGroup->addButton(_robotInfoButton);
    _vLayout = new QVBoxLayout();
    _vLayout->setContentsMargins(10,0,0,0);
    _vLayout->setSpacing(0);
    _vLayout->addWidget(_returnButton);
    _vLayout->addWidget(_orderButton);
    _vLayout->addWidget(_advertButton);
    _vLayout->addWidget(_soundButton);
    _vLayout->addWidget(_cameraButton);
    _vLayout->addWidget(_systemButton);
    _vLayout->addWidget(_robotInfoButton);
    _orderEmptyPage = new MOrderEmptyPage(this);
    _orderInfoPage = new MOrderInfoPage(this);
    _orderRetentionPage=new MOrderRetentionPage(this);
    _systemPage = new MSystemPage(this);
    _advertPage = new MAdvertPage(this);
    _soundPage = new MSoundPage(this);
    _cameraPage = new MCameraPage(this);
    _robotInfoPage = new MRobotInfoPage(this);
    _sLayout = new QStackedLayout();
    _sLayout->setContentsMargins(0,0,0,0);
    _sLayout->addWidget(_orderEmptyPage);
    _sLayout->addWidget(_orderInfoPage);
    _sLayout->addWidget(_orderRetentionPage);
    _sLayout->addWidget(_systemPage);
    _sLayout->addWidget(_advertPage);
    _sLayout->addWidget(_soundPage);
    _sLayout->addWidget(_cameraPage);
    _sLayout->addWidget(_robotInfoPage);
    _hLayout = new QHBoxLayout(this);
    _hLayout->setContentsMargins(0,0,0,0);
    _hLayout->setSpacing(0);
    _hLayout->addLayout(_vLayout);
    _hLayout->addLayout(_sLayout);
    setLayout(_hLayout);
    connect(hmi,SIGNAL(setFuctionSignal(HMI::MANAGER_PAGE)),this,SLOT(setFuctionSlot(HMI::MANAGER_PAGE)));
    connect(_returnButton,&QPushButton::clicked,[](){hmi->setPageSignal(HMI::BLINK);});
    connect(_orderButton,SIGNAL(clicked()),this,SLOT(orderButtonClickedSlot()));
    connect(_systemButton,&QPushButton::clicked,[this](){_sLayout->setCurrentWidget(_systemPage);});
    connect(_advertButton,&QPushButton::clicked,[this](){_sLayout->setCurrentWidget(_advertPage);});
    connect(_soundButton,&QPushButton::clicked,[this](){_sLayout->setCurrentWidget(_soundPage);});
    connect(_cameraButton,&QPushButton::clicked,[this](){_sLayout->setCurrentWidget(_cameraPage);});
    connect(_robotInfoButton,&QPushButton::clicked,[this](){_sLayout->setCurrentWidget(_robotInfoPage);});
}
ManagerPage::~ManagerPage(){
    _sLayout->deleteLater();
}
void ManagerPage::setFuctionSlot(HMI::MANAGER_PAGE page){
    switch (page) {
    case HMI::ORDER:
        _orderButton->click();
        break;
    case HMI::SYSTEM:
        _systemButton->click();
        break;
    case HMI::ADVERT:
        _advertButton->click();
        break;
    case HMI::SOUND:
        _soundButton->click();
        break;
    case HMI::CAMERA:
        _cameraButton->click();
        break;
    case HMI::ROBOTINFO:
        _robotInfoButton->click();
        break;
    default:
        break;
    }
}
void ManagerPage::orderButtonClickedSlot(){
    if(logic->workState==HTTP::WS_UNINIT){
        _sLayout->setCurrentWidget(_orderEmptyPage);
    }else if(logic->workState==HTTP::WS_UNUSED){
        _sLayout->setCurrentWidget(_orderEmptyPage);
    }else if(logic->workState==HTTP::WS_RETURN&&logic->orderState==HTTP::OS_DETENTION){
        _sLayout->setCurrentWidget(_orderRetentionPage);
    }else{
        _sLayout->setCurrentWidget(_orderInfoPage);
    }
}

