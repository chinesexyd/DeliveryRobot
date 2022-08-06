#include "p_managerloginpage.h"

/*@@@@@@@@@@@背景@@@@@@@@@@@*/
ManagerLoginPage::ManagerLoginPage(QWidget *parent) : QFrame(parent){
    _passwordLine = new KeyboardLine(this);
    _keyboardFrame = new KeyboardFrame(this);
    _keyboardFrame->setFixedSize(500,350);
    _returnButton = new ReturnButton(this);
    _passwordLine->move(100,200);
    _returnButton->move(10,10);
    _keyboardFrame->move(700,60);
    connect(_returnButton,&QPushButton::clicked,[](){emit hmi->setPageSignal(HMI::BLINK);});
    connect(_keyboardFrame,SIGNAL(sendKeySignal(QString)),_passwordLine,SLOT(receiveKeySlot(QString)));
    connect(_passwordLine,SIGNAL(sendLineText(QString)),this,SLOT(checkCodeSlot(QString)));
}
void ManagerLoginPage::checkCodeSlot(QString password){
    if(password==config->getValue("password").toString()){
        emit hmi->setPageSignal(HMI::MANAGER);
        emit hmi->setFuctionSignal(HMI::ORDER);
    }else{}
}
