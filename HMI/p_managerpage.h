#ifndef MANAGERPAGE_H
#define MANAGERPAGE_H

#include "HMI/a_logic.h"
#include "HMI/p_z_orderinfopage.h"
#include "HMI/p_z_orderemptypage.h"
#include "HMI/p_z_orderretentionpage.h"
#include "HMI/p_z_advertpage.h"
#include "HMI/p_z_systempage.h"
#include "HMI/p_z_soundpage.h"
#include "HMI/p_z_camerapage.h"
#include "HMI/p_z_robotinfopage.h"

/*@@@@@@@@@@@@@@@@@@@@*/
class ManagerPage : public ManagerBackgroundFrame{
    Q_OBJECT
public:
    explicit ManagerPage(QWidget *parent = nullptr);
    ~ManagerPage();
public slots:
    void setFuctionSlot(HMI::MANAGER_PAGE);
    void orderButtonClickedSlot();
private:
    QHBoxLayout* _hLayout;
    QVBoxLayout* _vLayout;
    ReturnButton* _returnButton;
    MenuButton* _orderButton;
    MenuButton* _soundButton;
    MenuButton* _advertButton;
    MenuButton* _systemButton;
    MenuButton* _cameraButton;
    MenuButton* _robotInfoButton;
    QButtonGroup* _buttonGroup;
    QStackedLayout* _sLayout;
    MOrderEmptyPage* _orderEmptyPage;
    MOrderInfoPage* _orderInfoPage;
    MOrderRetentionPage* _orderRetentionPage;
    MCameraPage* _cameraPage;
    MAdvertPage* _advertPage;
    MSystemPage* _systemPage;
    MSoundPage* _soundPage;
    MRobotInfoPage* _robotInfoPage;
};

#endif // MANAGERPAGE_H
