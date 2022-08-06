#ifndef P_ZORDERRETENTIONPAGE_H
#define P_ZORDERRETENTIONPAGE_H
#include "HMI/a_logic.h"
/*@@@@@@@@@@@正常@@@@@@@@@@*/
class MOrderRetentionPage : public QFrame{
    Q_OBJECT
public:
    explicit MOrderRetentionPage(QWidget *parent = nullptr);
protected:
    void showEvent(QShowEvent*)override;
private:
    OrderCustomerInfoFrame* _orderCustomerInfoFrame;
    OrderMoneyFrame* _orderMoneyFrame;
    SlideFrame* _goodsFrame;
    QGridLayout* _layout;
    CommonButton* _againOrderButton;
    CommonButton* _resetOrderButton;
};
#endif // P_ZORDERRETENTIONPAGE_H
