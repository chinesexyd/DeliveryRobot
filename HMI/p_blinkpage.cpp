#include "p_blinkpage.h"

/*@@@@@@@@@@@@@@@@@@@@@*/
BlinkPage::BlinkPage(QWidget *parent): QFrame(parent){
    _managerButton = new ManagerButton(this);
    _sloganLabel01 = new PixmapLabel(this);
    _sloganLabel01->setPixmap(":/Image/Blink04.png");
    _sloganLabel02 = new PixmapLabel(this);
    _sloganLabel02->setPixmap(":/Image/Blink03.png");
    _blinkLabel = new AnimationLabel(this);
    _blinkLabel->setPixmapPath(":/Image/BlinkEye.png");
    _wechatAppletCodeLabel=new PixmapLabel(this);
    _wechatAppletCodeLabel->setPixmap(":/Image/Blink01.png");
    _welcomeLabel=new PixmapLabel(this);
    _welcomeLabel->setPixmap(":/Image/Blink02.png");
    _managerButton->move(10,10);
    _sloganLabel01->setGeometry(140,60,122,29);
    _sloganLabel02->setGeometry(73,107,244,28);
    _blinkLabel->setGeometry(550,90,500,250);
    _wechatAppletCodeLabel->setGeometry(95,150,200,200);
    _welcomeLabel->setGeometry(74,365,242,27);
    connect(_managerButton,&QPushButton::clicked,[](){emit hmi->setPageSignal(HMI::MANAGER_LOGIN);});
}
void BlinkPage::showEvent(QShowEvent *){
    _isBlinkLeft=_isBlinkLeft?false:true;
    if(!_isBlinkLeft){
        _managerButton->move(1185,10);
        _sloganLabel01->setGeometry(1018,60,122,29);
        _sloganLabel02->setGeometry(963,107,244,28);
        _blinkLabel->setGeometry(230,90,500,250);
        _wechatAppletCodeLabel->setGeometry(985,150,200,200);
        _welcomeLabel->setGeometry(964,365,242,27);
    }else{
        _managerButton->move(10,10);
        _sloganLabel01->setGeometry(140,60,122,29);
        _sloganLabel02->setGeometry(73,107,244,28);
        _blinkLabel->setGeometry(550,90,500,250);
        _wechatAppletCodeLabel->setGeometry(95,150,200,200);
        _welcomeLabel->setGeometry(74,365,242,27);
    }
}
