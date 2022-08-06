#include "p_z_orderinfopage.h"

/*@@@@@@@@@@订单详情@@@@@@@@@@@*/
MOrderInfoPage::MOrderInfoPage(QWidget *parent) : QFrame(parent){
    _orderCustomerInfoFrame=new OrderCustomerInfoFrame(this);
    _orderMoneyFrame=new OrderMoneyFrame(this);
    _goodsFrame= new SlideFrame(this);
    _orderStatusLabel=new RobotInfoLabel(this);
    _orderStatusLabel->setName("订单状态");
    _orderStatusLabel->setPixmap(":/Image/StatusOrder.png");
    _cancelOrderButton=new CommonButton(this);
    _cancelOrderButton->setText("放弃订单");
    _cancelOrderButton->setPixmap(":/Image/ButtonCancelOrder.png");
    _cancelOrderButton->hide();
    _layout=new QGridLayout(this);
    _layout->setContentsMargins(30,30,30,30);
    _layout->setHorizontalSpacing(60);
    _layout->addWidget(_orderCustomerInfoFrame,0,0,1,2);
    _layout->addWidget(_goodsFrame,1,0,5,2);
    _layout->addWidget(_orderMoneyFrame,0,2,2,2);
    _layout->addWidget(_orderStatusLabel,2,2,1,2,Qt::AlignCenter);
    _layout->addWidget(_cancelOrderButton,3,2,1,2,Qt::AlignCenter);
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
    connect(timer,SIGNAL(timer1000TimeoutSignal()),this,SLOT(timer1000TimeoutSlot()));
    connect(_cancelOrderButton,&QPushButton::clicked,[](){logic->cancelOrder();});
}
void MOrderInfoPage::showEvent(QShowEvent* event){
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
void MOrderInfoPage::timer1000TimeoutSlot(){
    switch (logic->orderState) {
    case HTTP::OS_DELIVER_GOODS:
        _orderStatusLabel->setValue("发货中");
        break;
    case HTTP::OS_WAITING_FOR_TAKE_GOODS:
        _orderStatusLabel->setValue("等待取货");
        break;
    case HTTP::OS_FINISHED_TAKING_GOODS:
        _orderStatusLabel->setValue("取完货");
        break;
    case HTTP::OS_DETENTION:
        _orderStatusLabel->setValue("滞留");
        break;
    case HTTP::OS_ELEVATOR_ENTRANCE:
        _orderStatusLabel->setValue("电梯口");
        break;
    case HTTP::OS_IN_ELEVATOR:
        _orderStatusLabel->setValue("电梯内");
        break;
    default:
        _orderStatusLabel->setValue("未知");
        break;
    }
    switch (logic->workState) {
    case HTTP::WS_UNINIT:
        _cancelOrderButton->hide();
        break;
    case HTTP::WS_UNUSED:
        _cancelOrderButton->hide();
        break;
    case HTTP::WS_RECEIVED_ORDER:
        _cancelOrderButton->show();
        break;
    case HTTP::WS_RECEIVED_GOODS:
        _cancelOrderButton->hide();
        break;
    case HTTP::WS_SENDING_GOODS:
        _cancelOrderButton->show();
        break;
    case HTTP::WS_RETURN:
        _cancelOrderButton->hide();
        break;
    default:
        _cancelOrderButton->hide();
        break;
    }
}
