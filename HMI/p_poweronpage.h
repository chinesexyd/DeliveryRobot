#ifndef P_POWERONPAGE_H
#define P_POWERONPAGE_H

#include "HMI/a_logic.h"

class PowerOnPage : public QFrame{
    Q_OBJECT
public:
    explicit PowerOnPage(QWidget *parent = nullptr);
public slots:
    void pingHttpIsSuccessSlot(bool);
    void connectMqIsSuccessSlot(bool);
    void connectRosIsSuccessSlot(bool);
    void loadMapSlot(ROS::RETURN_STATE);
    void initialPoseSlot(ROS::RETURN_STATE);
    void startingUpSlot(HTTP::CODE_VALUE);
private:
    void checkCanInitable();
private:
    PowerButton* _initButton;
    PowerButton* _managerButton;
    QTextEdit* _textEdit;
    QLabel* _tittleLabel;
    CodeFrame* _codeFrame;
    QVBoxLayout* _layout00;
    QHBoxLayout* _layout01;
    QVBoxLayout* _layout;
private:
    bool _isConnectHttp=false;
    bool _isConnectMq=false;
    bool _isConnectRos=false;
};

#endif // P_POWERONPAGE_H
