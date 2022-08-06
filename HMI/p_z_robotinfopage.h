#ifndef P_ROBOTINFOPAGE_H
#define P_ROBOTINFOPAGE_H

#include "HMI/a_logic.h"

/*@@@@@@@@@@@@@@@@@@@@@*/
class MRobotInfoPage:public QFrame{
    Q_OBJECT
public:
    explicit MRobotInfoPage(QWidget *parent = nullptr);
public slots:
    void changedSlot();
private:
    QHBoxLayout* _layout;
    QVBoxLayout* _layout00;
    QVBoxLayout* _layout01;
    PixmapLabel* _robot;
    RobotInfoLabel* _macLabel;
    RobotInfoLabel* _buildTimeLabel;
    RobotInfoLabel* _collisionLabel;
    RobotInfoLabel* _curLabel;
    RobotInfoLabel* _imuLabel;
    RobotInfoLabel* _mapFileLabel;
    RobotInfoLabel* _milsLabel;
    RobotInfoLabel* _relayLabel;
    RobotInfoLabel* _skipLabel;
    RobotInfoLabel* _sonarLabel;
    RobotInfoLabel* _volLabel;
    RobotInfoLabel* _wheelLabel;
    RobotInfoLabel* _pointLabel;
    struct {
        QColor common=QColor("#FFFFFF");
        QColor warn=QColor("#FF0000");
        QColor health=QColor("#00FF00");
    }_color;
};
#endif // P_ROBOTINFOPAGE_H
