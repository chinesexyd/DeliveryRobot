#ifndef USERLOGINPAGE_H
#define USERLOGINPAGE_H

#include "HMI/a_logic.h"

/*@@@@@@@@@@@@@@@@@@@@*/
class UserLoginPage : public QFrame{
    Q_OBJECT
public:
    explicit UserLoginPage(QWidget *parent = nullptr);
private:
    KeyboardFrame* _keyboardFrame;
    KeyboardLine* _codeLine;
    QVBoxLayout* _vLayout;
    QHBoxLayout* _hLayout;
};

#endif // USERLOGINPAGE_H
