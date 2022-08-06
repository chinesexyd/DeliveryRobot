#include "p_mainwindow.h"

/*@@@@@@@@@@@@@@@@@@@@@*/
MainWindow::MainWindow(QWidget *parent) : BackgroundFrame(parent){
    setWindowFlags(Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint);
    _vLayout = new QVBoxLayout(this);
    _vLayout->setContentsMargins(0,0,0,0);
    _vLayout->setSpacing(0);
    _statusBar = new StatusBar(this);
    _sLayout = new QStackedLayout();
    _blinkPage = new BlinkPage(this);
    _managerLoginPage = new ManagerLoginPage(this);
    _managerPage = new ManagerPage(this);
    _powerOnPage = new PowerOnPage(this);
    _showOrderPage = new ShowOrderPage(this);
    _userReceivePage = new UserReceivePage(this);
    _userLoginPage = new UserLoginPage(this);
    _sLayout->addWidget(_powerOnPage);
    _sLayout->addWidget(_blinkPage);
    _sLayout->addWidget(_managerLoginPage);
    _sLayout->addWidget(_managerPage);
    _sLayout->addWidget(_showOrderPage);
    _sLayout->addWidget(_userReceivePage);
    _sLayout->addWidget(_userLoginPage);
    _vLayout->addWidget(_statusBar);
    _vLayout->addLayout(_sLayout);
    setLayout(_vLayout);
    initGlobal();
    connect(hmi,SIGNAL(setPageSignal(HMI::PAGE)),this,SLOT(changePageSlot(HMI::PAGE)));
    connect(hmi,SIGNAL(exitSignal()),this,SLOT(exitSlot()));
}
MainWindow::~MainWindow(){
    hmi->deleteLater();
    timer->deleteLater();
    http->deleteLater();
    mq->deleteLater();
    ros->deleteLater();
    logic->deleteLater();
    config->deleteLater();
    camera->deleteLater();
    tts->deleteLater();
    voice->deleteLater();
    obs->deleteLater();
    _sLayout->deleteLater();
    delete note;
}
void MainWindow::initGlobal(){
    timer->setTimerParent(this);
    logic->firstStart();
}
void MainWindow::exitSlot(){
    QApplication* app;
    app->exit(0);
    note->writeLine(Note::M_HMI,"退出界面","用户点击的退出界面");
}
void MainWindow::mousePressEvent(QMouseEvent *e){
    BackgroundFrame::mousePressEvent(e);
    activateWindow();
    if(_currentPage==HMI::BLINK){
        voice->playRoundSound(S_LOW,"dontTouchMe.wav",1);
    }
}
void MainWindow::showEvent(QShowEvent *event){
#if QT_VERSION >= 0x050000
    this->setAttribute(Qt::WA_Mapped);
#endif
    BackgroundFrame::showEvent(event);
}
void MainWindow::changePageSlot(HMI::PAGE page){
    _currentPage=page;
    switch (page) {
    case HMI::POWER_ON:
        _sLayout->setCurrentWidget(_powerOnPage);
        note->writeLine(Note::M_HMI,"切换页面","开机初始化界面");
        break;
    case HMI::BLINK:
        _sLayout->setCurrentWidget(_blinkPage);
        note->writeLine(Note::M_HMI,"切换页面","眨眼页面");
        break;
    case HMI::MANAGER_LOGIN:
        _sLayout->setCurrentWidget(_managerLoginPage);
        note->writeLine(Note::M_HMI,"切换页面","管理员登陆页面");
        break;
    case HMI::MANAGER:
        _sLayout->setCurrentWidget(_managerPage);
        note->writeLine(Note::M_HMI,"切换页面","管理员界面");
        break;
    case HMI::SHOW_ORDER:
        _sLayout->setCurrentWidget(_showOrderPage);
        note->writeLine(Note::M_HMI,"切换页面","显示订单页面，用于管理员放入货品");
        break;
    case HMI::USER_RECEIVE:
        _sLayout->setCurrentWidget(_userReceivePage);
        note->writeLine(Note::M_HMI,"切换页面","用户接收货品页面");
        break;
    case HMI::USER_LOGIN:
        _sLayout->setCurrentWidget(_userLoginPage);
        note->writeLine(Note::M_HMI,"切换页面","用户输入验证码页面");
        break;
    default:
        break;
    }
}
