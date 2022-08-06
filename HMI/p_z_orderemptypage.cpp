#include "p_z_orderemptypage.h"

/*@@@@@@@@@@正常@@@@@@@@@@@*/
MOrderEmptyPage::MOrderEmptyPage(QWidget *parent) : QFrame(parent){
    _pixmapLabel = new PixmapLabel(this);
    _pixmapLabel->setPixmap(":/Image/NoOrder.png");
    _pixmapLabel->setFixedSize(128,128);
    _pixmapLabel->move(550,100);
    _noOrderLabel = new QLabel(this);
    _noOrderLabel->setText("当前无订单可处理哦！");
    _noOrderLabel->setStyleSheet("color:#FFFFFF;font:75 20pt \"Ubuntu\"");
    _noOrderLabel->move(485,250);
}
