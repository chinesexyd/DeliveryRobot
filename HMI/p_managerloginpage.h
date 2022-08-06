#ifndef MANAGERLOGINPAGE_H
#define MANAGERLOGINPAGE_H

#include "HMI/a_logic.h"

/*@@@@@@@@@@@@@@@@@@@@@@*/
class ManagerLoginPage : public QFrame{
    Q_OBJECT
public:
    explicit ManagerLoginPage(QWidget *parent = nullptr);
public slots:
    void checkCodeSlot(QString);
private:
    KeyboardFrame* _keyboardFrame;
    KeyboardLine* _passwordLine;
    QHBoxLayout* _hLayout;
    ReturnButton* _returnButton;
    bool _isumpManagerOrderPage=false;
};

#endif // MANAGERLOGINPAGE_H
