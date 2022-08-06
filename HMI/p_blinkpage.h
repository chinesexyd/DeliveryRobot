#ifndef BLINKPAGE_H
#define BLINKPAGE_H

#include "HMI/a_logic.h"

/*@@@@@@@@@@@@@@@@@@@@@*/
class BlinkPage : public QFrame{
    Q_OBJECT
public:
    BlinkPage(QWidget *parent = nullptr);
protected:
    void showEvent(QShowEvent*);
private:
    ManagerButton* _managerButton;
    PixmapLabel* _sloganLabel01;
    PixmapLabel* _sloganLabel02;
    PixmapLabel* _wechatAppletCodeLabel;
    PixmapLabel* _welcomeLabel;
    AnimationLabel* _blinkLabel;
    bool _isBlinkLeft=false;
};
#endif // HMI::BLINKPAGE_H
