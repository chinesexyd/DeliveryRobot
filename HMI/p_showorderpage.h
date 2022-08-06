#ifndef P_SHOWORDERPAGE_H
#define P_SHOWORDERPAGE_H

#include "HMI/a_logic.h"

/*@@@@@@@@@@@@@@@@@@@@@*/
class ShowOrderPage : public QFrame{
    Q_OBJECT
public:
    explicit ShowOrderPage(QWidget *parent = nullptr);
protected:
    void showEvent(QShowEvent*)override;
private:
    SlideFrame* _goodsFrame;
    OrderMoneyFrame* _orderMoneyFrame;
    CommonButton* _startSendGoodsButton;
    CommonButton* _cancelOrderButton;
    CommonButton* _closeDoorButton;
    CommonButton* _openDoorButton;
    QGridLayout* _layout;
};
#endif // P_SHOWORDERPAGE_H
