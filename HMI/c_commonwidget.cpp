#include "c_commonwidget.h"
/*@@@@@@@@@@@常规背景@@@@@@@@@*/
BackgroundFrame::BackgroundFrame(QWidget* parent):QWidget(parent){
    _pixmap=QPixmap(":/Image/FrameBackground.png");
}
void BackgroundFrame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter.drawPixmap(rect(),_pixmap,_pixmap.rect());
    painter.restore();
}
/*@@@@@@@@@@@管理员背景@@@@@@@@@*/
ManagerBackgroundFrame::ManagerBackgroundFrame(QWidget* parent):QWidget(parent){
    _pixmap=QPixmap(":/Image/FrameBackground.png");
    _leftPixmap=QPixmap(":/Image/FrameBackgroundLeft.png");
    _lightPixmap=QPixmap(":/Image/FrameBackgroundLight.png");
}
void ManagerBackgroundFrame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter.drawPixmap(rect(),_pixmap,_pixmap.rect());
    QRect leftRect(-5,-5,65,height()+10);;
    painter.drawPixmap(leftRect,_lightPixmap,_lightPixmap.rect());
    painter.restore();
}
/*@@@@@@@@@@@设置图片@@@@@@@@@*/
PixmapLabel::PixmapLabel(QWidget* parent):QWidget(parent){}
void PixmapLabel::setPixmap(QString path){
    _pixmap=QPixmap(path);
    update();
}
void PixmapLabel::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter.drawPixmap(rect(),_pixmap,_pixmap.rect());
    painter.restore();
}
/*@@@@@@@@@@@电池图标@@@@@@@@@*/
PowerFrame::PowerFrame(QWidget* parent):QWidget(parent){
    colorChange = new ColorChange();
    colorChange->setRange(_min,_max);
    colorChange->setStartColor(_startColor);
    colorChange->setEndColor(_endColor);
}
PowerFrame::~PowerFrame(){
    delete colorChange;
}
void PowerFrame::setPower(int currentNum){
    if(currentNum>=0&&currentNum<=100){
        _currentNum = currentNum;
        _currentColor = colorChange->getColor(currentNum);
        update();
    }
}
void PowerFrame::setCharge(bool isCharging){
    _isCharging = isCharging;
    update();
}
void PowerFrame::paintEvent(QPaintEvent *){
    setFixedSize(height()*2.3,height());
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    QPen pen;
    pen.setColor(QColor(_borderColor));
    pen.setWidthF(2);
    painter.setPen(pen);
    QRectF re(1,1,rect().width()-5,rect().height()-2);
    painter.drawRoundedRect(re,2,2);
    painter.setBrush(QBrush(QColor(_borderColor)));
    painter.setPen(Qt::NoPen);
    QRectF re1(rect().width()-4,(height()-10)/2.0,4,10);
    painter.drawRoundedRect(re1,2,2);
    painter.setBrush(QBrush(QColor(_currentColor.r,_currentColor.g,_currentColor.b)));
    QRectF re2(3,3,(rect().width()-9)/100.0*_currentNum,rect().height()-6);
    painter.drawRoundedRect(re2,2,2);
    if(_isCharging){
        double w = width()-5;
        QPointF robotArrow[6]={
            QPointF(w/4.0*2,height()/5.0),
            QPointF(w/4.0*2,height()/5.0*2),
            QPointF(w/4.0*3,height()/5.0),
            QPointF(w/4.0*2,height()/5.0*4),
            QPointF(w/4.0*2,height()/5.0*3),
            QPointF(w/4.0,height()/5.0*4),
        };
        painter.setBrush(QBrush(QColor(_charingColor)));
        painter.drawPolygon(robotArrow,6);
    }
    painter.restore();
}
/*@@@@@@@@@@@信号图标@@@@@@@@@*/
SignalFrame::SignalFrame(QWidget* parent):QWidget(parent){}
void SignalFrame::setSignal(quint8 CSQ){
    if(CSQ>0&&CSQ<=11){
        _signalValue=1;
    }else if(CSQ<18&&CSQ>11){
        _signalValue=2;
    }else if(CSQ==99||CSQ==0){
        _signalValue=0;
    }else{
        _signalValue=3;
    }
    update();
}
void SignalFrame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    double wunit=width()/11.0;
    double hunit=height()/8.0;
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush("#818181"));
    QRectF re00=QRectF(0,hunit*6,wunit*2,hunit*2);
    QRectF re01=QRectF(wunit*3,hunit*4,wunit*2,hunit*4);
    QRectF re02=QRectF(wunit*6,hunit*2,wunit*2,hunit*6);
    QRectF re03=QRectF(wunit*9,0,wunit*2,hunit*8);
    painter.drawRoundedRect(re00,2,2);
    painter.drawRoundedRect(re01,2,2);
    painter.drawRoundedRect(re02,2,2);
    painter.drawRoundedRect(re03,2,2);
    painter.setBrush(QBrush("#FFFFFF"));
    if(_signalValue==0){
        painter.drawRoundedRect(re00,2,2);
    }else if(_signalValue==1){
        painter.drawRoundedRect(re00,2,2);
        painter.drawRoundedRect(re01,2,2);
    }else if(_signalValue==2){
        painter.drawRoundedRect(re00,2,2);
        painter.drawRoundedRect(re01,2,2);
        painter.drawRoundedRect(re02,2,2);
    }else if(_signalValue==3){
        painter.drawRoundedRect(re00,2,2);
        painter.drawRoundedRect(re01,2,2);
        painter.drawRoundedRect(re02,2,2);
        painter.drawRoundedRect(re03,2,2);
    }
    painter.restore();
}
/*@@@@@@@@@@@商品条目@@@@@@@@@*/
GoodsItemFrame::GoodsItemFrame(QWidget* parent):QWidget(parent){
    _manager=new QNetworkAccessManager(this);
    _backgroundColor=QColor("#FFFFFF");
    _backgroundColor.setAlpha(60);
    setFixedHeight(50);
    connect(_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(finishedSlot(QNetworkReply*)));
}
void GoodsItemFrame::setGoodsInfo(HMI::GOODS_INFO goodsInfo){
    _goodsInfo=goodsInfo;
    QUrl imageUrl(goodsInfo.listPicture);
    QNetworkRequest request;
    request.setUrl(imageUrl);
    request.setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
    _manager->get(request);
    update();
}
void GoodsItemFrame::finishedSlot(QNetworkReply* reply){
    QByteArray bytes=reply->readAll();
    _pixmap.loadFromData(bytes);
    reply->close();
    reply->deleteLater();
    update();
}
void GoodsItemFrame::paintEvent(QPaintEvent *){
    QRect pictureRect(2,2,46,46);
    QRect nameRect(60,0,width()-260,25);
    QRect priceRect(60,25,width()-260,25);
    QRect numberRect(width()-300,0,150,50);
    QRect amountRect(width()-150,0,150,50);
    QString name=_goodsInfo.goodsName;
    QString number;
    QString amount;
    QString price;
    if(_goodsInfo.packType==HMI::PACK_JINGBAOZHUANG||_goodsInfo.packType==HMI::PACK_YUBAOZHUANG){
        number=QString::number(_goodsInfo.buyNumber)+"个";
        amount="¥"+QString::number(_goodsInfo.sellingPrice*_goodsInfo.buyNumber,'f',2)+"元";
        price="¥"+QString::number(_goodsInfo.sellingPrice,'f',2)+"元/"+_goodsInfo.speInfo;
    }else{
        number=QString::number(_goodsInfo.buyNumber)+"克";
        amount="¥"+QString::number(_goodsInfo.sellingPrice/(float)_goodsInfo.unitWeight*_goodsInfo.buyNumber,'f',2)+"元";
        price="¥"+QString::number(_goodsInfo.sellingPrice,'f',2)+"元/"+QString::number(_goodsInfo.unitWeight)+"克";
    }
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing,true);
    painter.setPen("#2A9D8F");
    painter.setBrush(QBrush(_backgroundColor));
    painter.drawRoundedRect(rect(),5,5);
    QPen pen;
    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QFont font;
    font.setPointSize(15);
    painter.setFont(font);
    painter.drawText(nameRect,Qt::AlignVCenter|Qt::AlignLeft,name);
    painter.drawText(numberRect,Qt::AlignVCenter|Qt::AlignLeft,number);
    painter.drawText(amountRect,Qt::AlignVCenter|Qt::AlignLeft,amount);
    QPen pen0;
    pen0.setColor(QColor("#C5C5C5"));
    painter.setPen(pen0);
    QFont font0;
    font0.setPointSize(13);
    painter.setFont(font0);
    painter.drawText(priceRect,Qt::AlignVCenter|Qt::AlignLeft,price);
    QPainterPath path;
    path.addRoundedRect(pictureRect,5,5);
    painter.setClipPath(path);
    painter.drawPixmap(pictureRect,_pixmap,_pixmap.rect());
    painter.restore();
}
/*@@@@@@@@@@@开关条目@@@@@@@@@*/
SwitchItemFrame::SwitchItemFrame(QWidget* parent):QWidget(parent){
    _backgroundColor=QColor("#FFFFFF");
    _backgroundColor.setAlpha(60);
    setFixedHeight(50);
    _sound=new QSound(":/Music/check.wav");
}
void SwitchItemFrame::setName(QString name){
    _name=name;
    update();
}
void SwitchItemFrame::setTip(QString tip){
    _tip=tip;
    update();
}
void SwitchItemFrame::setPixmap(QString path){
    _pixmap=QPixmap(path);
    update();
}
void SwitchItemFrame::setBool(bool isTrue){
    _isTrue=isTrue;
    if(_isTrue){
        _ellipseRect=QRect(width()-58,7,36,36);
        _switchColor=QColor("#2A9D8F");
    }else{
        _ellipseRect=QRect(width()-78,7,36,36);
        _switchColor=QColor("#A5A5A5");
    }
    update();
}
void SwitchItemFrame::resizeEvent(QResizeEvent *){
    _switchRect=QRect(width()-80,5,60,40);
    if(_isTrue){
        _ellipseRect=QRect(width()-58,7,36,36);
        _switchColor=QColor("#2A9D8F");
    }else{
        _ellipseRect=QRect(width()-78,7,36,36);
        _switchColor=QColor("#A5A5A5");
    }
}
void SwitchItemFrame::mousePressEvent(QMouseEvent *event){
    QPoint pos=event->pos();
    if(_switchRect.contains(pos)){
        if(_isTrue){
            _isTrue=false;
        }else{
            _isTrue=true;
        }
        emit changedSignal(_isTrue);
        _sound->play();
        if(_isTrue){
            _ellipseRect=QRect(width()-58,7,36,36);
            _switchColor=QColor("#2A9D8F");
        }else{
            _ellipseRect=QRect(width()-78,7,36,36);
            _switchColor=QColor("#A5A5A5");
        }
        update();
    }
}
void SwitchItemFrame::paintEvent(QPaintEvent *){
    QRect pixmapRect(10,10,30,30);
    QRect nameRect(50,0,width()-80,25);
    QRect tipRect(50,25,width()-80,25);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing,true);
    painter.setPen("#2A9D8F");
    painter.setBrush(QBrush(_backgroundColor));
    painter.drawRoundedRect(rect(),5,5);
    painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
    painter.setPen(QPen(QColor("#FFFFFF")));
    QFont font;
    font.setPointSize(15);
    painter.setFont(font);
    painter.drawText(nameRect,Qt::AlignVCenter|Qt::AlignLeft,_name);
    painter.setPen(QPen(QColor("#D7D7D7")));
    QFont font01;
    font.setPointSize(12);
    painter.setFont(font);
    painter.drawText(tipRect,Qt::AlignVCenter|Qt::AlignLeft,_tip);
    painter.setBrush(_switchColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(_switchRect,20,20);
    painter.setBrush(QColor("#FFFFFF"));
    painter.drawEllipse(_ellipseRect);
    painter.restore();
}
/*@@@@@@@@@@@滑动窗口@@@@@@@@@*/
SlideFrame::SlideFrame(QWidget* parent):QWidget(parent){
    _centralFrame=new QWidget(this);
    _centralLayout=new QVBoxLayout(_centralFrame);
    _centralLayout->setContentsMargins(0,0,0,0);
    _centralLayout->setSpacing(5);
    _centralFrame->setLayout(_centralLayout);
}
void SlideFrame::addItem(QWidget* item){
    item->setParent(_centralFrame);
    _centralLayout->addWidget(item);
    _itemList.append(item);
    _centralHeight+=55;
    _centralFrame->setFixedHeight(_centralHeight);
}
void SlideFrame::clearItems(){
    _centralHeight=0;
    _centralFrame->setFixedHeight(_centralHeight);
    for(int i=0;i<_itemList.length();i++){
        _centralLayout->removeWidget(_itemList[i]);
        _itemList[i]->deleteLater();
    }
    _itemList.clear();
}
void SlideFrame::resizeEvent(QResizeEvent *){
    _centralFrame->setFixedWidth(width());
}
void SlideFrame::mousePressEvent(QMouseEvent *event){
    _startPos=event->pos();
}
void SlideFrame::mouseMoveEvent(QMouseEvent *event){
    QPointF distance=event->pos()-_startPos;
    keep(_centralY+distance.y());
}
void SlideFrame::mouseReleaseEvent(QMouseEvent *){
    _centralY=_centralFrame->y();
}
void SlideFrame::keep(double y){
    double ch = _centralFrame->height();
    double h = height();
    if(y>0){y=0;}
    if(ch>h&&y<(h-ch)){y = h-ch;}
    if(ch<=h){y=0;}
    _centralFrame->move(0,y);
}
/*@@@@@@@@@@@键盘按键@@@@@@@@@*/
KeyboardButton::KeyboardButton(QWidget* parent):QPushButton(parent){
    _parent=static_cast<KeyboardFrame*>(parent);
    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setFixedSize(100,100);
    _backgroundColor=QColor("#FFFFFF");
    _backgroundColor.setAlpha(60);
    _sound=new QSound(":/Music/number.wav",this);
    connect(this,SIGNAL(clicked()),this,SLOT(clickedSlot()));
}
void KeyboardButton::setNumber(quint8 number){
    if(number!=10&&number!=11){
        _isDrawPixmap=false;
    }else if(number==10){
        _isDrawPixmap=true;
        _pixmap=QPixmap(":/Image/KeyDelete.png");
    }else{
        _isDrawPixmap=true;
        _pixmap=QPixmap(":/Image/KeyOk.png");
    }
    _number=QString::number(number);
    update();
}
void KeyboardButton::clickedSlot(){
    _sound->play();
    emit _parent->sendKeySignal(_number);
}
void KeyboardButton::mousePressEvent(QMouseEvent* event){
    QPushButton::mousePressEvent(event);
    _backgroundColor.setAlpha(100);
    update();
}
void KeyboardButton::mouseReleaseEvent(QMouseEvent* event){
    QPushButton::mouseReleaseEvent(event);
    _backgroundColor.setAlpha(60);
    update();
}
void KeyboardButton::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing,true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(_backgroundColor));
    painter.drawEllipse(rect());
    if(_isDrawPixmap){
        QRect pixmapRect(20,20,60,60);
        painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
    }else{
        painter.setPen(QColor("#FFFFFF"));
        QFont font;
        font.setBold(true);
        font.setPointSize(45);
        painter.setFont(font);
        painter.drawText(rect(),Qt::AlignCenter,_number);
    }
    painter.restore();
}
/*@@@@@@@@@@@键盘@@@@@@@@@*/
KeyboardFrame::KeyboardFrame(QWidget* parent):QWidget(parent){
    _key0=new KeyboardButton(this);
    _key0->setNumber(0);
    _key1=new KeyboardButton(this);
    _key1->setNumber(1);
    _key2=new KeyboardButton(this);
    _key2->setNumber(2);
    _key3=new KeyboardButton(this);
    _key3->setNumber(3);
    _key4=new KeyboardButton(this);
    _key4->setNumber(4);
    _key5=new KeyboardButton(this);
    _key5->setNumber(5);
    _key6=new KeyboardButton(this);
    _key6->setNumber(6);
    _key7=new KeyboardButton(this);
    _key7->setNumber(7);
    _key8=new KeyboardButton(this);
    _key8->setNumber(8);
    _key9=new KeyboardButton(this);
    _key9->setNumber(9);
    _key10=new KeyboardButton(this);
    _key10->setNumber(10);
    _key11=new KeyboardButton(this);
    _key11->setNumber(11);
    _layout=new QGridLayout(this);
    _layout->setContentsMargins(0,0,0,0);
    _layout->setSpacing(10);
    _layout->addWidget(_key1,0,0,1,1);
    _layout->addWidget(_key2,0,1,1,1);
    _layout->addWidget(_key3,0,2,1,1);
    _layout->addWidget(_key10,0,3,1,1);
    _layout->addWidget(_key4,1,0,1,1);
    _layout->addWidget(_key5,1,1,1,1);
    _layout->addWidget(_key6,1,2,1,1);
    _layout->addWidget(_key0,1,3,1,1);
    _layout->addWidget(_key7,2,0,1,1);
    _layout->addWidget(_key8,2,1,1,1);
    _layout->addWidget(_key9,2,2,1,1);
    _layout->addWidget(_key11,2,3,1,1);
    setLayout(_layout);
}
/*@@@@@@@@@@@键盘输入框@@@@@@@@@@@*/
KeyboardLine::KeyboardLine(QWidget* parent):QWidget(parent){
    setFixedSize(300,60);
    _line=new QLineEdit(this);
    _line->setEchoMode(QLineEdit::Password);
    _line->setPlaceholderText("请输入");
    _line->setStyleSheet("background:transparent;color:#FFFFFF;border:none;font:75 20pt \"Ubuntu\";");
    _layout=new QHBoxLayout(this);
    _layout->setContentsMargins(20,10,10,10);
    _layout->setSpacing(0);
    _layout->addWidget(_line);
    setLayout(_layout);
}
void KeyboardLine::receiveKeySlot(QString key){
    if(key=="10"){
        if(_text.length()>=1){
            _text=_text.left(_text.length()-1);
            _line->setText(_text);
        }
    }else if(key=="11"){
        emit sendLineText(_text);
    }else{
        _text+=key;
        _line->setText(_text);
    }
}
void KeyboardLine::showEvent(QShowEvent *){
    _line->clear();
    _text.clear();
    _line->setFocus();
}
void KeyboardLine::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    QPen pen;
    pen.setColor(QColor("#E9C46A"));
    pen.setWidth(3);
    painter.setPen(pen);
    QRect rect(5,5,width()-10,height()-10);
    painter.drawRoundedRect(rect,15,15);
    painter.restore();
}
/*@@@@@@@@@@@菜单按钮@@@@@@@@@*/
MenuButton::MenuButton(QWidget* parent):QPushButton(parent){
    setFixedSize(40,40);
    setCheckable(true);
    _sound=new QSound(":/Music/button.wav",this);
    connect(this,&QPushButton::clicked,[this](){_sound->play();});
}
void MenuButton::setUncheckedPixmap(QString path){
    _uncheckedPixmap=QPixmap(path);
}
void MenuButton::setCheckedPixmap(QString path){
    _checkedPixmap=QPixmap(path);
}
void MenuButton::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    if(isChecked()){
        painter.drawPixmap(rect(),_checkedPixmap,_checkedPixmap.rect());
    }else{
        painter.drawPixmap(rect(),_uncheckedPixmap,_uncheckedPixmap.rect());
    }
    painter.restore();
}
/*@@@@@@@@@@@动画标签@@@@@@@@@@@*/
AnimationLabel::AnimationLabel(QWidget* parent):QLabel(parent){}
AnimationLabel::~AnimationLabel(){
    _aninormal->stop();
}
void AnimationLabel::setPixmapPath(QString path){
    QPixmap pixmap(path);
    for(int i=0;i<10;i++){
        _pixAninormalList<<pixmap.copy(i*(pixmap.width()/10),0,pixmap.width()/10,pixmap.height());
    }
    _aninormal = new QPropertyAnimation(this,"");
    _aninormal->setStartValue(0);
    _aninormal->setEndValue(9);
    _aninormal->setDuration(1200);
    connect(_aninormal,SIGNAL(valueChanged(QVariant)),this,SLOT(animationValueChangedSlot(QVariant)));
    _aninormal->start();
}
void AnimationLabel::animationValueChangedSlot(QVariant value){
    _pixmapIndex = value.toInt();
    update();
}
void AnimationLabel::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.drawPixmap(rect(),_pixAninormalList.at(_pixmapIndex));
    if(_pixmapIndex==9){
        _aninormal->start();
    }
}
/*@@@@@@@@@@@返回按钮@@@@@@@@@@@*/
ReturnButton::ReturnButton(QWidget *parent) : QPushButton(parent){
    setFixedSize(50,50);
    _pixmap=QPixmap(":/Image/ButtonReturn.png");
    _sound=new QSound(":/Music/button.wav",this);
    connect(this,&QPushButton::clicked,[this](){_sound->play();});
}
void ReturnButton::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter.drawPixmap(rect(),_pixmap,_pixmap.rect());
    painter.restore();
}
/*@@@@@@@@@@@管理员按钮@@@@@@@@@@@*/
ManagerButton::ManagerButton(QWidget* parent):QPushButton(parent){
    setFixedSize(85,85);
    _pixmap=QPixmap(":/Image/ButtonManager.png");
    _sound=new QSound(":/Music/button.wav",this);
    connect(this,&QPushButton::clicked,[this](){_sound->play();});
}
void ManagerButton::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing,true);
    painter.drawPixmap(QRect(22,10,38,40),_pixmap);
    painter.setFont(QFont("Ubuntu",13));
    painter.setPen("#FFFFFF");
    painter.drawText(QRectF(0,50,85,22),Qt::AlignCenter,"管理员");
    painter.restore();
}
/*@@@@@@@@@@@大按钮@@@@@@@@@@@*/
PowerButton::PowerButton(QWidget* parent):QPushButton(parent){
    setFixedSize(160,60);
    _sound=new QSound(":/Music/change.wav",this);
    connect(this,&QPushButton::clicked,[this](){_sound->play();});
}
void PowerButton::setText(QString text){
    _text=text;
    update();
}
void PowerButton::setPixmap(QString path){
    _pixmap=QPixmap(path);
    update();
}
void PowerButton::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing,true);
    QColor color("#2A9D8F");
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(),10,10);
    QRect pixmapRect(5,5,50,50);
    painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
    painter.setPen(QPen(QColor("#FFFFFF")));
    QFont font;
    font.setBold(true);
    font.setPointSize(20);
    painter.setFont(font);
    QRect textRect(50,0,110,60);
    painter.drawText(textRect,Qt::AlignCenter,_text);
    painter.restore();
}
/*@@@@@@@@@@@通用按钮@@@@@@@@@@@*/
CommonButton::CommonButton(QWidget* parent):QPushButton(parent){
    setFixedSize(85,85);
    _sound=new QSound(":/Music/change.wav",this);
    _backgroundColor=QColor("#2A9D8F");
}
void CommonButton::setText(QString text){
    _text=text;
    update();
}
void CommonButton::setPixmap(QString path){
    _pixmap=QPixmap(path);
    update();
}
void CommonButton::mousePressEvent(QMouseEvent *event){
    QPushButton::mousePressEvent(event);
    _sound->play();
    _backgroundColor=QColor("#E76F51");
    update();
}
void CommonButton::mouseReleaseEvent(QMouseEvent *event){
    QPushButton::mouseReleaseEvent(event);
    _backgroundColor=QColor("#2A9D8F");
    update();
}
void CommonButton::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing,true);
    painter.setBrush(_backgroundColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundRect(rect(),20,20);
    painter.drawPixmap(QRect(22,10,40,40),_pixmap);
    painter.setPen(QColor("#FFFFFF"));
    painter.setFont(QFont("Ubuntu",13));
    painter.drawText(QRectF(0,50,85,22),Qt::AlignCenter,_text);
    painter.restore();
}
/*@@@@@@@@@@@@二维码@@@@@@@@@@*/
CodeFrame::CodeFrame(QWidget* parent):QWidget(parent){
    setFixedSize(240,240);
}
CodeFrame::~CodeFrame(){
    delete _qrcode;
}
void CodeFrame::setCodeText(QString codeText){
    _isSetCodeText=true;
    _qrcode=QRcode_encodeString(codeText.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
    update();
}
void CodeFrame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing,true);
    QColor color("#FFFFFF");
    color.setAlpha(60);
    painter.setBrush(color);
    painter.setPen("#2A9D8F");
    painter.drawRoundedRect(rect(),10,10);
    painter.setPen(QColor("#FFFFFF"));
    QFont font;
    font.setPointSize(15);
    painter.setFont(font);
    painter.drawText(rect(),Qt::AlignCenter,"二维码区域");
    if(_isSetCodeText){
        double unit= (double)200/(double)_qrcode->width;
        painter.setBrush(QColor("#FFFFFF"));
        painter.setPen(Qt::NoPen);
        QRect rect(10,10,width()-20,height()-20);
        painter.drawRoundedRect(rect,10,10);
        painter.setBrush(QColor("#000000"));
        for (qint32 y=0;y<_qrcode->width;y++){
            for (qint32 x=0;x<_qrcode->width;x++){
                unsigned char b =_qrcode->data[y*_qrcode->width+x];
                if (b & 0x01){
                    QRectF r(20+x*unit,20+y*unit,unit,unit);
                    painter.drawRect(r);
                }
            }
        }
    }
    painter.restore();
}
/*@@@@@@@@@@@@提示栏@@@@@@@@@@@@@*/
TipLabel::TipLabel(QWidget *parent) : QLabel(parent){
    setStyleSheet("color:#FF0000;font:75 13pt \"Ubuntu\";");
    setAlignment(Qt::AlignRight);
}
void TipLabel::showTip(TIP_INFO info){
    switch (info.type) {
    case TT_ERROR:
        setText(QString("错误码：%1 信息：%2").arg(info.code).arg(info.detail));
        break;
    case TT_NORMAL:
        clear();
        break;
    default:
        break;
    }
}
/*@@@@@@@@@@@订单金额@@@@@@@@@@@*/
OrderMoneyFrame::OrderMoneyFrame(QWidget* parent):QWidget(parent){
    _amountLabel=new QLabel(this);
    _amountLine=new QLabel(this);
    _concessionaryLabel=new QLabel(this);
    _concessionaryLine=new QLabel(this);
    _paymentLabel=new QLabel(this);
    _paymentLine=new QLabel(this);
    _layout=new QFormLayout(this);
    _amountLabel->setStyleSheet("color:#FFFFFF;font:75 15pt \"Ubuntu\"");
    _concessionaryLabel->setStyleSheet("color:#FFFFFF;font:75 15pt \"Ubuntu\"");
    _paymentLabel->setStyleSheet("color:#FFFFFF;font:75 15pt \"Ubuntu\"");
    _amountLine->setStyleSheet("color:#FFFFFF;font:75 20pt \"Ubuntu\"");
    _concessionaryLine->setStyleSheet("color:#FFFFFF;font:75 20pt \"Ubuntu\"");
    _paymentLine->setStyleSheet("color:#FFFFFF;font:75 20pt \"Ubuntu\"");
    _amountLabel->setAlignment(Qt::AlignLeft);
    _concessionaryLabel->setAlignment(Qt::AlignLeft);
    _paymentLabel->setAlignment(Qt::AlignLeft);
    _amountLine->setAlignment(Qt::AlignRight);
    _concessionaryLine->setAlignment(Qt::AlignRight);
    _paymentLine->setAlignment(Qt::AlignRight);
    _amountLabel->setText("总价");
    _concessionaryLabel->setText("优惠");
    _paymentLabel->setText("实付");
    _layout->setContentsMargins(10,10,10,10);
    _layout->setSpacing(10);
    _layout->addRow(_amountLabel,_amountLine);
    _layout->addRow(_concessionaryLabel,_concessionaryLine);
    _layout->addRow(_paymentLabel,_paymentLine);
    setLayout(_layout);
}
void OrderMoneyFrame::setAmount(double amount){
    _amountLine->setText("¥"+QString::number(amount,'f',2));
}
void OrderMoneyFrame::setConcessionary(double concessionary){
    _concessionaryLine->setText("¥"+QString::number(concessionary,'f',2));
}
void OrderMoneyFrame::setPayment(double payment){
    _paymentLine->setText("¥"+QString::number(payment,'f',2));
}
void OrderMoneyFrame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing,true);
    painter.save();
    QColor color("#FFFFFF");
    color.setAlpha(60);
    painter.setBrush(color);
    painter.setPen("#2A9D8F");
    painter.drawRoundedRect(rect(),10,10);
    painter.restore();
}
/*@@@@@@@@@@@订单用户信息@@@@@@@@@@@*/
OrderCustomerInfoFrame::OrderCustomerInfoFrame(QWidget* parent):QWidget(parent){
    _customerNameLabel=new PixmapLabel(this);
    _customerNameLabel->setFixedSize(25,25);
    _customerNameLine=new QLabel(this);
    _customerTelLabel=new PixmapLabel(this);
    _customerTelLabel->setFixedSize(25,25);
    _customerTelLine=new QLabel(this);
    _customerAddressLabel=new PixmapLabel(this);
    _customerAddressLabel->setFixedSize(25,25);
    _customerAddressLine=new QLabel(this);
    _orderTimeLabel=new PixmapLabel(this);
    _orderTimeLabel->setFixedSize(25,25);
    _orderTimeLine=new QLabel(this);
    _customerNameLabel->setPixmap(":/Image/OrderCustomerName.png");
    _customerTelLabel->setPixmap(":/Image/OrderCustomerPhone.png");
    _customerAddressLabel->setPixmap(":/Image/OrderCustomerAddress.png");
    _orderTimeLabel->setPixmap(":/Image/OrderTime.png");
    _customerNameLine->setStyleSheet("color:#FFFFFF;font:75 15pt \"Ubuntu\"");
    _customerTelLine->setStyleSheet("color:#FFFFFF;font:75 15pt \"Ubuntu\"");
    _customerAddressLine->setStyleSheet("color:#FFFFFF;font:75 15pt \"Ubuntu\"");
    _orderTimeLine->setStyleSheet("color:#FFFFFF;font:75 15pt \"Ubuntu\"");
    _customerNameLine->setAlignment(Qt::AlignLeft);
    _customerTelLine->setAlignment(Qt::AlignLeft);
    _customerAddressLine->setAlignment(Qt::AlignLeft);
    _orderTimeLine->setAlignment(Qt::AlignLeft);
    _layout00=new QFormLayout();
    _layout00->setContentsMargins(0,0,0,0);
    _layout00->setSpacing(10);
    _layout00->addRow(_customerNameLabel,_customerNameLine);
    _layout00->addRow(_customerTelLabel,_customerTelLine);
    _layout01=new QFormLayout();
    _layout01->setContentsMargins(0,0,0,0);
    _layout01->setSpacing(10);
    _layout01->addRow(_customerAddressLabel,_customerAddressLine);
    _layout01->addRow(_orderTimeLabel,_orderTimeLine);
    _layout=new QHBoxLayout(this);
    _layout->setContentsMargins(10,10,10,10);
    _layout->setSpacing(10);
    _layout->addLayout(_layout00);
    _layout->addLayout(_layout01);
    setLayout(_layout);
}
void OrderCustomerInfoFrame::setCustomerName(QString name){
    _customerNameLine->setText(name);
}
void OrderCustomerInfoFrame::setCustomerPhone(QString phone){
    _customerTelLine->setText(phone);
}
void OrderCustomerInfoFrame::setCustomerAddress(QString building,QString unit,QString floor,QString room){
    _customerAddressLine->setText(building+"-"+unit+"-"+floor+"楼-"+room+"房间");
}
void OrderCustomerInfoFrame::setOrderTime(QString time){
    _orderTimeLine->setText(time);
}
void OrderCustomerInfoFrame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing,true);
    painter.save();
    QColor color("#FFFFFF");
    color.setAlpha(60);
    painter.setBrush(color);
    painter.setPen("#2A9D8F");
    painter.drawRoundedRect(rect(),10,10);
    painter.restore();
}
/*@@@@@@@@@@@滑动条@@@@@@@@@@@*/
SlideLineBar::SlideLineBar(QWidget* parent):QWidget(parent){
    setFixedHeight(50);
    _sound=new QSound(":/Music/slide.wav",this);
}
void SlideLineBar::setRange(quint8 min,quint8 max){
    _min=min;
    _max=max;
}
void SlideLineBar::setPixmap(QString path){
    _pixmap=QPixmap(path);
    update();
}
void SlideLineBar::setName(QString name){
    _name=name;
    update();
}
void SlideLineBar::setValue(quint8 value){
    _value=value;
    valueToRect(value);
}
void SlideLineBar::paintEvent(QPaintEvent *){
    QRect pixmapRect(10,10,30,30);
    QRect textRect(50,0,width()-50,25);
    QRect sliderRect(52,27,width()-57,21);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing|QPainter::SmoothPixmapTransform,true);
    painter.save();
    QColor color("#FFFFFF");
    color.setAlpha(60);
    painter.setBrush(color);
    painter.setPen("#2A9D8F");
    painter.drawRoundedRect(rect(),5,5);
    painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
    QFont font;
    font.setPointSize(15);
    painter.setFont(font);
    painter.setPen(QColor("#FFFFFF"));
    painter.drawText(textRect,Qt::AlignLeft|Qt::AlignVCenter,_name);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#264653"));
    painter.drawRoundedRect(sliderRect,10.5,10.5);
    painter.setBrush(QColor("#2A9D8F"));
    painter.drawRoundedRect(_valueRect,8.5,8.5);
    QFont font01;
    font.setPointSize(10);
    painter.setFont(font);
    painter.setPen(QColor("#FFFFFF"));
    painter.drawText(_valueRect,Qt::AlignCenter,QString::number(_value));
    painter.restore();
}
void SlideLineBar::resizeEvent(QResizeEvent *){
    _unit=(width()-50-2-2-5-2)/(double)(_max-_min+1);
    valueToRect(_value);
}
void SlideLineBar::mousePressEvent(QMouseEvent *event){
    xToRect(event->x());
}
void SlideLineBar::mouseMoveEvent(QMouseEvent *event){
    xToRect(event->x());
}
void SlideLineBar::mouseReleaseEvent(QMouseEvent *event){
    xToRect(event->x());
}
void SlideLineBar::xToRect(qint32 x){
    if(x<=50+2+2){
        _value=_min;
    }else if(x>=width()-4){
        _value=_max;
    }else{
        double dValue=(double)(x-57)/(double)_unit+_min-0.5;
        _value=qRound(dValue);
    }
    _value=_value<_min?_min:_value;
    _value=_value>_max?_max:_value;
    emit changedSignal(_value);
    valueToRect(_value);
}
void SlideLineBar::valueToRect(quint8 value){
    if(value!=_max){
        double x=50+2+2+_unit*(value-_min);
        double y=25+2+2;
        double w=_unit;
        double h=17;
        _valueRect=QRectF(x,y,w,h);
    }else{
        _valueRect=QRectF(width()-_unit-7,29,_unit,17);
    }
    update();
}
/*@@@@@@@@@@@下拉框@@@@@@@@@@@*/
ChooseCommbo::ChooseCommbo(QWidget* parent):QWidget(parent){
    setFixedHeight(50);
    _layout=new QHBoxLayout(this);
    _layout->setContentsMargins(52,29,7,4);
    _layout->setSpacing(2);
    setLayout(_layout);
    _buttonGroup=new QButtonGroup(this);
    _sound=new QSound(":/Music/check.wav",this);
}
void ChooseCommbo::setStringList(QStringList list){
    for(int i=0;i<list.length();i++){
        QString name=list[i];
        QPushButton* button=new QPushButton(this);
        button->setProperty("name",name);
        button->setText(name);
        button->setCheckable(true);
        button->setStyleSheet("QPushButton{background-color:#2A9D8F;color: white;border-radius: 8px;}"
                              "QPushButton:checked{background-color:#E76F51;};");
        _buttonmap.insert(name,button);
        _layout->addWidget(button);
        _buttonGroup->addButton(button);
        connect(button,&QPushButton::clicked,[this,button](){_sound->play();emit changedSignal(button->property("name").toString());});
    }
}
void ChooseCommbo::setPixmap(QString path){
    _pixmap=QPixmap(path);
}
void ChooseCommbo::setName(QString name){
    _name=name;
}
void ChooseCommbo::setChoose(QString choose){
    if(_buttonmap.contains(choose)){
        _buttonmap[choose]->click();
    }
}
QString ChooseCommbo::getChoose(){
    return _buttonGroup->checkedButton()->property("name").toString();
}
void ChooseCommbo::paintEvent(QPaintEvent *){
    QRect pixmapRect(10,10,30,30);
    QRect textRect(50,0,width()-50,25);
    QRect sliderRect(50,27,width()-55,21);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing|QPainter::SmoothPixmapTransform,true);
    painter.save();
    QColor color("#FFFFFF");
    color.setAlpha(60);
    painter.setBrush(color);
    painter.setPen("#2A9D8F");
    painter.drawRoundedRect(rect(),5,5);
    painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
    QFont font;
    font.setPointSize(15);
    painter.setFont(font);
    painter.setPen(QColor("#FFFFFF"));
    painter.drawText(textRect,Qt::AlignLeft|Qt::AlignVCenter,_name);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#264653"));
    painter.drawRoundedRect(sliderRect,10.5,10.5);
    painter.restore();
}
/*@@@@@@@@@@@SoftwareFrame@@@@@@@@@*/
SoftwareLine::SoftwareLine(QWidget* parent):QWidget(parent){}
void SoftwareLine::setInfo(SOFTWARELINE_INFO info){
    _softwareInfo=info;
    update();
}
void SoftwareLine::paintEvent(QPaintEvent *){
    QRect pixmapRect(0,0,height(),height());
    QRect textRect(height()+5,0,width()-height()-5,height());
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing|QPainter::SmoothPixmapTransform,true);
    painter.save();
    painter.drawPixmap(pixmapRect,_softwareInfo.pixmap,_softwareInfo.pixmap.rect());
    QFont font;
    font.setPointSize(15);
    painter.setFont(font);
    painter.setPen(QColor("#FFFFFF"));
    painter.drawText(textRect,Qt::AlignLeft|Qt::AlignVCenter,_softwareInfo.title+"："+_softwareInfo.version);
    font.setPointSize(11);
    painter.setFont(font);
    painter.drawText(textRect,Qt::AlignRight|Qt::AlignVCenter,_softwareInfo.detail);
    painter.restore();
}
SoftwareFrame::SoftwareFrame(QWidget* parent):QWidget(parent){
    setFixedHeight(90);
    _titleLine=new QLabel(this);
    _cloudLine=new SoftwareLine(this);
    _localLine=new SoftwareLine(this);
    _cloudInfo.pixmap=QPixmap(":/Image/SoftCloud.png");
    _cloudInfo.title="后台版本";
    _cloudLine->setInfo(_cloudInfo);
    _localInfo.pixmap=QPixmap(":/Image/SoftLocal.png");
    _localInfo.title="本地版本";
    _localLine->setInfo(_localInfo);
    _tipLine=new QLabel(this);
    _titleLine->setStyleSheet("color:#FFFFFF;font:Bold 20pt \"Ubuntu\"");
    _tipLine->setStyleSheet("color:#FFFFFF;font:75 11pt \"Ubuntu\"");
    //background-color:#264653;
    _statusLabel=new PixmapLabel(this);
    _statusLabel->setFixedSize(48,48);
    _layout0=new QVBoxLayout();
    _layout0->setContentsMargins(0,0,0,0);
    _layout0->setSpacing(5);
    _layout0->addWidget(_cloudLine);
    _layout0->addWidget(_localLine);
    _layout0->addWidget(_tipLine);
    _layout=new QHBoxLayout(this);
    _layout->setContentsMargins(25,5,25,5);
    _layout->setSpacing(25);
    _layout->addWidget(_titleLine);
    _layout->addLayout(_layout0);
    _layout->addWidget(_statusLabel,Qt::AlignCenter);
    _layout->setStretch(0,1);
    _layout->setStretch(1,8);
    _layout->setStretch(2,1);
    setLayout(_layout);
    _sound=new QSound(":/Music/button.wav",this);
    _manager=new QNetworkAccessManager(this);
}
SoftwareFrame::~SoftwareFrame(){
    _layout0->deleteLater();
}
void SoftwareFrame::setTip(QString tip){
    _tipLine->setText(tip);
}
void SoftwareFrame::setTitle(QString title){
    _titleLine->setText(title);
}
void SoftwareFrame::setCloud(CLOUD cloud){
    _cloud=cloud;
    _cloudInfo.version=cloud.versionName;
    _cloudInfo.detail=cloud.remark;
    _cloudLine->setInfo(_cloudInfo);
    downloadIsEnable();
}
void SoftwareFrame::setLocalVersion(QString version){
    _localVersion=version;
    _localInfo.version=version;
    _localLine->setInfo(_localInfo);
    downloadIsEnable();
}
void SoftwareFrame::downloadIsEnable(){
    if(_localVersion!=_cloud.versionName){
        if(_cloud.versionName.isEmpty()||_localVersion.isEmpty()){
            _status=SW_ERROR;
            _statusLabel->setPixmap(":/Image/SoftError.png");
        }else{
            _status=SW_HASNEW;
            _statusLabel->setPixmap(":/Image/SoftHasNew.png");
        }
    }else{
        if(_localVersion.isEmpty()||_cloud.versionName.isEmpty()){
            _status=SW_ERROR;
            _statusLabel->setPixmap(":/Image/SoftError.png");
        }else{
            _status=SW_DOWNLOADED;
            _statusLabel->setPixmap(":/Image/SoftDownloaded.png");
        }
    }
    update();
}
void SoftwareFrame::mousePressEvent(QMouseEvent *){
    if(_status==SW_HASNEW){
        _sound->play();
        QNetworkRequest request;
        request.setUrl(QUrl(_cloud.url));
        request.setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
        _reply=_manager->get(request);
        connect(_reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProcessSlot(qint64,qint64)));
    }
}
void SoftwareFrame::downloadProcessSlot(qint64 receive,qint64 all){
    _downloadProcess=receive/(double)all;
    if(receive==all){
        QByteArray bytes=_reply->readAll();
        QFile hmiZipFile(_cloud.savePath);
        hmiZipFile.open(QIODevice::WriteOnly);
        hmiZipFile.write(bytes);
        hmiZipFile.close();
        _reply->deleteLater();
        _statusLabel->setPixmap(":/Image/SoftDownloaded.png");
        emit finishedSignal(_cloud.savePath);
    }else{
        _statusLabel->setPixmap(":/Image/SoftDownloading.png");
        _status=SW_DOWNLOADING;
    }
    update();
}
void SoftwareFrame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing,true);
    painter.save();
    if(_status==SW_ERROR||_status==SW_DOWNLOADED){
        painter.setPen("#2A9D8F");
        QColor color("#FFFFFF");
        color.setAlpha(60);
        painter.setBrush(color);
        painter.drawRoundedRect(rect(),5,5);
    }else if(_status==SW_HASNEW){
        painter.setPen("#2A9D8F");
        QColor color("#2A9D8F");
        painter.setBrush(color);
        painter.drawRoundedRect(rect(),5,5);
    }else{
        painter.setPen("#2A9D8F");
        painter.setBrush(QColor("#2A9D8F"));
        painter.drawRoundedRect(rect(),5,5);
        painter.setBrush(QColor("#e23e57"));
        QRectF re(0,0,width()*_downloadProcess,height());
        painter.drawRoundedRect(re,5,5);
    }
    painter.restore();
}
/*@@@@@@@@@@@机器信息栏@@@@@@@@@*/
RobotInfoLabel::RobotInfoLabel(QWidget* parent):QWidget(parent){
    setFixedHeight(50);
}
void RobotInfoLabel::setDirect(DIRECT direct){
    _direct=direct;
    update();
}
void RobotInfoLabel::setPixmap(QString path){
    _pixmap=QPixmap(path);
    update();
}
void RobotInfoLabel::setName(QString name){
    _name=name;
    update();
}
void RobotInfoLabel::setValue(QString value){
    _value=value;
    update();
}
void RobotInfoLabel::setValueColor(QColor color){
    _color=color;
    update();
}
void RobotInfoLabel::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing|QPainter::SmoothPixmapTransform,true);
    painter.save();
    QColor color("#FFFFFF");
    color.setAlpha(60);
    painter.setBrush(color);
    painter.setPen("#2A9D8F");
    painter.drawRoundedRect(rect(),5,5);
    if(_direct==LEFT){
        QRect pixmapRect(width()-40,10,30,30);
        QRect textRect(5,0,width()-60,height());
        painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
        QFont font;
        font.setPointSize(12);
        painter.setFont(font);
        painter.setPen("#FFFFFF");
        painter.drawText(textRect,Qt::AlignRight|Qt::AlignVCenter,_name);
        painter.setPen(_color);
        font.setPointSize(15);
        painter.setFont(font);
        painter.drawText(textRect,Qt::AlignLeft|Qt::AlignVCenter,_value);
    }else{
        QRect pixmapRect(10,10,30,30);
        QRect textRect(55,0,width()-60,height());
        painter.drawPixmap(pixmapRect,_pixmap,_pixmap.rect());
        QFont font;
        font.setPointSize(12);
        painter.setFont(font);
        painter.setPen("#FFFFFF");
        painter.drawText(textRect,Qt::AlignLeft|Qt::AlignVCenter,_name);
        painter.setPen(_color);
        font.setPointSize(15);
        painter.setFont(font);
        painter.drawText(textRect,Qt::AlignRight|Qt::AlignVCenter,_value);
    }
    painter.restore();
}
