#include "p_userreceivepage.h"

/*@@@@@@@@@@@@@@@@@@@@*/
UserReceivePage::UserReceivePage(QWidget *parent) : QFrame(parent){
    _orderMoneyFrame=new OrderMoneyFrame(this);
    _confirmGoodsButton = new CommonButton(this);
    _confirmGoodsButton->setText("确认收货");
    _confirmGoodsButton->setPixmap(":/Image/ButtonReceiveGoods.png");
    _goodsFrame = new SlideFrame(this);
    _telLabel = new QLabel(this);
    _telLabel->setStyleSheet("color:#ffffff;font:75 12pt \"Ubuntu\"");
    _telLabel->setAlignment(Qt::AlignCenter);
    _layout=new QGridLayout(this);
    _layout->setContentsMargins(30,30,30,30);
    _layout->setSpacing(60);
    _layout->addWidget(_goodsFrame,0,0,6,2);
    _layout->addWidget(_orderMoneyFrame,0,2,3,2);
    _layout->addWidget(_confirmGoodsButton,4,2,1,2,Qt::AlignCenter);
    _layout->addWidget(_telLabel,5,2,1,2);
    _layout->setColumnStretch(0,6);
    _layout->setColumnStretch(1,6);
    _layout->setColumnStretch(2,2);
    _layout->setColumnStretch(3,2);
    setLayout(_layout);
    _confirmGoodsButton->move(1020,300);
    connect(_confirmGoodsButton,&QPushButton::clicked,[](){logic->confirmGoods();});
}
void UserReceivePage::showEvent(QShowEvent *event){
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
    _telLabel->setText(http->initCommunityConfigInfo.cuyInfo.phoneNumber);
}
