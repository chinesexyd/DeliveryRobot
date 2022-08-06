#include "p_z_orderretentionpage.h"
/*@@@@@@@@@@订单详情@@@@@@@@@@@*/
MOrderRetentionPage::MOrderRetentionPage(QWidget *parent) : QFrame(parent){
    _orderCustomerInfoFrame=new OrderCustomerInfoFrame(this);
    _orderMoneyFrame=new OrderMoneyFrame(this);
    _goodsFrame= new SlideFrame(this);
    _againOrderButton=new CommonButton(this);
    _againOrderButton->setText("再次送货");
    _againOrderButton->setPixmap(":/Image/ButtonSendGoods.png");
    _resetOrderButton=new CommonButton(this);
    _resetOrderButton->setText("手动完成");
    _resetOrderButton->setPixmap(":/Image/ButtonResetOrder.png");
    _layout=new QGridLayout(this);
    _layout->setContentsMargins(30,30,30,30);
    _layout->setHorizontalSpacing(60);
    _layout->addWidget(_orderCustomerInfoFrame,0,0,1,2);
    _layout->addWidget(_goodsFrame,1,0,5,2);
    _layout->addWidget(_orderMoneyFrame,0,2,2,2);
    _layout->addWidget(_againOrderButton,4,2,1,1);
    _layout->addWidget(_resetOrderButton,4,3,1,1);
    _layout->setColumnStretch(0,6);
    _layout->setColumnStretch(1,6);
    _layout->setColumnStretch(2,2);
    _layout->setColumnStretch(3,2);
    _layout->setRowStretch(0,1);
    _layout->setRowStretch(1,1);
    _layout->setRowStretch(2,1);
    _layout->setRowStretch(3,2);
    _layout->setRowStretch(4,2);
    _layout->setRowStretch(5,2);
    setLayout(_layout);
    connect(_againOrderButton,&QPushButton::clicked,[](){logic->againSendGoods();});
    connect(_resetOrderButton,&QPushButton::clicked,[](){logic->resetOrderState();});
}
void MOrderRetentionPage::showEvent(QShowEvent* event){
    QFrame::showEvent(event);
    _goodsFrame->clearItems();
    for(int i=0;i<logic->orderInfo.goodsList.length();i++){
        HMI::GOODS_INFO goodsInfo=logic->orderInfo.goodsList[i];
        GoodsItemFrame* item=new GoodsItemFrame();
        item->setGoodsInfo(goodsInfo);
        _goodsFrame->addItem(item);
    }
    _orderMoneyFrame->setAmount(logic->orderInfo.orderAmount);
    _orderMoneyFrame->setConcessionary(logic->orderInfo.concessionary);
    _orderMoneyFrame->setPayment(logic->orderInfo.orderAmount-logic->orderInfo.concessionary);
    _orderCustomerInfoFrame->setCustomerName(logic->orderInfo.addressInfo.consigneeName);
    _orderCustomerInfoFrame->setCustomerPhone(logic->orderInfo.addressInfo.consigneePhone);
    _orderCustomerInfoFrame->setCustomerAddress(logic->orderInfo.addressInfo.buildingNumber,
                                                logic->orderInfo.addressInfo.unitNumber,
                                                QString::number(logic->orderInfo.addressInfo.floorNumber),
                                                logic->orderInfo.addressInfo.roomNumber);
    _orderCustomerInfoFrame->setOrderTime(logic->orderInfo.createTime);
}

