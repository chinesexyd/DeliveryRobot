#include "p_userloginpage.h"

/*@@@@@@@@@@@@@@@@@@@@*/
UserLoginPage::UserLoginPage(QWidget *parent) : QFrame(parent){
    _codeLine = new KeyboardLine(this);
    _keyboardFrame = new KeyboardFrame(this);
    _keyboardFrame->setFixedSize(500,350);
    _codeLine->move(100,200);
    _keyboardFrame->move(700,60);
    connect(_keyboardFrame,SIGNAL(sendKeySignal(QString)),_codeLine,SLOT(receiveKeySlot(QString)));
    connect(_codeLine,&KeyboardLine::sendLineText,[](QString code){logic->checkMsgCode(code);});
}
