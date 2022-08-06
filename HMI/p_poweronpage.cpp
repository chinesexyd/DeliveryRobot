#include "p_poweronpage.h"

PowerOnPage::PowerOnPage(QWidget *parent) : QFrame(parent){
    _initButton = new PowerButton(this);
    _initButton->setText("初始化");
    _initButton->setPixmap(":/Image/ButtonInit.png");
    _managerButton = new PowerButton(this);
    _managerButton->setText("管理员");
    _managerButton->setPixmap(":/Image/ButtonManager.png");
    _layout00 = new QVBoxLayout();
    _layout00->setContentsMargins(0,0,0,0);
    _layout00->setSpacing(50);
    _layout00->addWidget(_initButton);
    _layout00->addWidget(_managerButton);
    _textEdit = new QTextEdit(this);
    _textEdit->setReadOnly(true);
    _textEdit->setStyleSheet("background-color:transparent;color:white;border:none;");
    _textEdit->setDocumentTitle("输出信息:");
    _textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _codeFrame=new CodeFrame(this);
    _layout01 = new QHBoxLayout();
    _layout01->setContentsMargins(0,0,0,0);
    _layout01->setSpacing(100);
    _layout01->addLayout(_layout00);
    _layout01->addWidget(_textEdit);
    _layout01->addWidget(_codeFrame);
    _layout01->setStretch(0,1);
    _layout01->setStretch(1,1);
    _layout01->setStretch(2,1);
    _tittleLabel = new QLabel(this);
    _tittleLabel->setAlignment(Qt::AlignCenter);
    _tittleLabel->setText("「开机启动 请耐心等待」");
    _tittleLabel->setStyleSheet("color:#FFFFFF;font:75 20pt \"Ubuntu\"");
    _layout = new QVBoxLayout(this);
    _layout->setContentsMargins(200,50,200,50);
    _layout->setSpacing(30);
    _layout->addWidget(_tittleLabel);
    _layout->addLayout(_layout01);
    setLayout(_layout);
    _initButton->hide();
    connect(_initButton,&QPushButton::clicked,[](){logic->initRobot();});
    connect(_managerButton,&QPushButton::clicked,[](){hmi->setPageSignal(HMI::MANAGER_LOGIN);});
    connect(http,SIGNAL(pingHttpIsSuccessSignal(bool)),this,SLOT(pingHttpIsSuccessSlot(bool)));
    connect(mq,SIGNAL(connectMqIsSuccessSignal(bool)),this,SLOT(connectMqIsSuccessSlot(bool)));
    connect(ros,SIGNAL(connectRosIsSuccessSignal(bool)),this,SLOT(connectRosIsSuccessSlot(bool)));
    connect(ros,SIGNAL(loadMapSignal(ROS::RETURN_STATE)),this,SLOT(loadMapSlot(ROS::RETURN_STATE)));
    connect(ros,SIGNAL(initialPoseSignal(ROS::RETURN_STATE)),this,SLOT(initialPoseSlot(ROS::RETURN_STATE)));
    connect(http,SIGNAL(startingUpSignal(HTTP::CODE_VALUE)),this,SLOT(startingUpSlot(HTTP::CODE_VALUE)));
}
void PowerOnPage::pingHttpIsSuccessSlot(bool isSuccess){
    if(isSuccess){
        _isConnectHttp=true;
        _textEdit->append("<p>连接服务后台(H)成功</p>");
        disconnect(http,SIGNAL(pingHttpIsSuccessSignal(bool)),this,SLOT(pingHttpIsSuccessSlot(bool)));
        checkCanInitable();
        http->checkSoftware(HTTP::ST_ROS);
        http->checkSoftware(HTTP::ST_HMI);
        http->checkSoftware(HTTP::ST_BAN);
    }
}
void PowerOnPage::connectMqIsSuccessSlot(bool isSuccess){
    if(isSuccess){
        _isConnectMq=true;
        _textEdit->append("<p>连接消息队列(M)成功</p>");
        disconnect(mq,SIGNAL(connectMqIsSuccessSignal(bool)),this,SLOT(connectMqIsSuccessSlot(bool)));
        checkCanInitable();
    }
}
void PowerOnPage::connectRosIsSuccessSlot(bool isSuccess){
    if(isSuccess){
        _isConnectRos=true;
        _textEdit->append("<p>连接机器系统(R)成功</p>");
        disconnect(ros,SIGNAL(connectRosIsSuccessSignal(bool)),this,SLOT(connectRosIsSuccessSlot(bool)));
        checkCanInitable();
    }
}
void PowerOnPage::checkCanInitable(){
    if(_isConnectMq&&_isConnectRos&&_isConnectHttp){
        _textEdit->append("<p>正在检查该设备是否授权绑定</p>");
        http->startingUp();//检查配置
    }else{}
}
void PowerOnPage::loadMapSlot(ROS::RETURN_STATE state){
    if(state==ROS::RS_SUCEESS){
        _textEdit->append("<p>初始化地图成功</p>");
        disconnect(ros,SIGNAL(loadMapSignal(ROS::RETURN_STATE)),this,SLOT(loadMapSlot(ROS::RETURN_STATE)));
    }else if(state==ROS::RS_RUNNING){
        _textEdit->append("<p>正在初始化地图</p>");
    }else{
        _textEdit->append("<p>初始化地图失败</p>");
    }
}
void PowerOnPage::initialPoseSlot(ROS::RETURN_STATE state){
    if(state==ROS::RS_SUCEESS){
        _textEdit->append("<p>初始化点成功.</p>");
        disconnect(ros,SIGNAL(initialPoseSignal(ROS::RETURN_STATE)),this,SLOT(initialPoseSlot(ROS::RETURN_STATE)));
    }else if(state==ROS::RS_RUNNING){
        _textEdit->append("<p>正在初始化点</p>");
    }else{
        _textEdit->append("<p>初始化点失败</p>");
    }
}
void PowerOnPage::startingUpSlot(HTTP::CODE_VALUE code){
    switch (code) {
    case HTTP::CV_NORMAL://正常 可以点击初始化按钮
        _textEdit->append("<p>已通过授权绑定检查</p>");
        _textEdit->append("<p>现在可以点击初始化</p>");
        _textEdit->append("<p>欢迎使用</p>");
        _initButton->show();
        break;
    case HTTP::CV_DISABLE://无人车禁用 联系管理员处理
        _textEdit->append("<p>未通过授权绑定检查</p>");
        _textEdit->append("<p>此设备已被禁用</p>");
        _textEdit->append("<p>请联系相关人员了解详情</p>");
        break;
    case HTTP::CV_UNAUTHORIZED://未授权 提示设备尚未授权
        _textEdit->append("<p>未通过授权绑定检查</p>");
        _textEdit->append("<p>此设备尚未授权</p>");
        _textEdit->append("<p>请联系相关人员了解详情</p>");
        break;
    case HTTP::CV_UNBIND://未绑定 弹出二维码进行绑定
        _codeFrame->setCodeText(macAddress);
        _textEdit->append("<p>未通过授权绑定检查</p>");
        _textEdit->append("<p>此设备尚未绑定</p>");
        _textEdit->append("<p>请扫描右侧二维码进行绑定</p>");
        break;
    default:
        break;
    }
}

