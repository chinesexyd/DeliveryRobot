#ifndef MORDERPAGE03_H
#define MORDERPAGE03_H

#include "HMI/a_logic.h"

/*@@@@@@@@@@订单详情@@@@@@@@@@@*/
class MOrderInfoPage : public QFrame{
    Q_OBJECT
public:
    explicit MOrderInfoPage(QWidget *parent = nullptr);
protected:
    void showEvent(QShowEvent* event)override;
public slots:
    void timer1000TimeoutSlot();
private:
    OrderCustomerInfoFrame* _orderCustomerInfoFrame;
    OrderMoneyFrame* _orderMoneyFrame;
    SlideFrame* _goodsFrame;
    RobotInfoLabel* _orderStatusLabel;
    CommonButton* _cancelOrderButton;
    QGridLayout* _layout;
};

#endif
