#include "p_showorderpage.h"

ShowOrderPage::ShowOrderPage(QWidget *parent) : QFrame(parent){
    _orderMoneyFrame=new OrderMoneyFrame(this);
    _startSendGoodsButton = new CommonButton(this);
    _startSendGoodsButton->setText("开始送货");
    _startSendGoodsButton->setPixmap(":/Image/ButtonSendGoods.png");
    _cancelOrderButton = new CommonButton(this);
    _cancelOrderButton->setText("放弃订单");
    _cancelOrderButton->setPixmap(":/Image/ButtonCancelOrder.png");
    _openDoorButton = new CommonButton(this);
    _openDoorButton->setText("打开车门");
    _openDoorButton->setPixmap(":/Image/ButtonOpenDoor.png");
    _closeDoorButton = new CommonButton(this);
    _closeDoorButton->setText("关闭车门");
    _closeDoorButton->setPixmap(":/Image/ButtonCloseDoor.png");
    _goodsFrame = new SlideFrame(this);
    _layout=new QGridLayout(this);
    _layout->setContentsMargins(30,30,30,30);
    _layout->setSpacing(60);
    _layout->addWidget(_goodsFrame,0,0,6,2);
    _layout->addWidget(_orderMoneyFrame,0,2,3,2);
    _layout->addWidget(_startSendGoodsButton,3,2,1,1);
    _layout->addWidget(_cancelOrderButton,3,3,1,1);
    _layout->addWidget(_openDoorButton,4,2,1,1);
    _layout->addWidget(_closeDoorButton,4,3,1,1);
    _layout->setColumnStretch(0,6);
    _layout->setColumnStretch(1,6);
    _layout->setColumnStretch(2,2);
    _layout->setColumnStretch(3,2);
    setLayout(_layout);
    connect(_startSendGoodsButton,&QPushButton::clicked,[](){logic->startSendGoods();});
    connect(_openDoorButton,&QPushButton::clicked,[](){ros->openDoor();});
    connect(_closeDoorButton,&QPushButton::clicked,[](){ros->closeDoor();});
    connect(_cancelOrderButton,&QPushButton::clicked,[](){logic->cancelOrder();});
}
void ShowOrderPage::showEvent(QShowEvent *event){
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
}



