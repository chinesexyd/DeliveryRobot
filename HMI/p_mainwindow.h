#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "HMI/p_blinkpage.h"
#include "HMI/p_managerloginpage.h"
#include "HMI/p_managerpage.h"
#include "HMI/p_poweronpage.h"
#include "HMI/p_showorderpage.h"
#include "HMI/p_statusbar.h"
#include "HMI/p_userreceivepage.h"
#include "HMI/p_userloginpage.h"

/*@@@@@@@@@@@@@@@@@@@@@*/
class MainWindow : public BackgroundFrame{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initGlobal();
public slots:
    void changePageSlot(HMI::PAGE);
    void exitSlot();
protected:
    void mousePressEvent(QMouseEvent *) override;
    void showEvent(QShowEvent *event) override;
private:
    StatusBar* _statusBar;
    QVBoxLayout* _vLayout;
    QStackedLayout *_sLayout;
    BlinkPage* _blinkPage;
    ManagerLoginPage* _managerLoginPage;
    ManagerPage* _managerPage;
    PowerOnPage* _powerOnPage;
    UserReceivePage* _userReceivePage;
    ShowOrderPage* _showOrderPage;
    UserLoginPage* _userLoginPage;
    QList<HMI::PAGE> _pageList;
    HMI::PAGE _currentPage=HMI::POWER_ON;
};

#endif // MAINWINDOW_H
