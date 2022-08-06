#ifndef USERRECEIVEPAGE_H
#define USERRECEIVEPAGE_H

#include "HMI/a_logic.h"

/*@@@@@@@@@@@@@@@@@@@@*/
class UserReceivePage : public QFrame{
    Q_OBJECT
public:
    explicit UserReceivePage(QWidget *parent = nullptr);
protected:
    void showEvent(QShowEvent*)override;
private:
    SlideFrame* _goodsFrame;
    CommonButton* _confirmGoodsButton;
    OrderMoneyFrame* _orderMoneyFrame;
    QLabel* _telLabel;
    QFormLayout* _layout00;
    QGridLayout* _layout;
};

#endif // USERRECEIVEPAGE_H
