#ifndef T_CAMERA_H
#define T_CAMERA_H

#include "HMI/t_ros.h"
#include "HMI/t_timer.h"

/*@@@@@@@@@@@全局ROS@@@@@@@@@@@*/
extern ROS* ros;
/*@@@@@@@@@@@@@@@@@@@@@@*/
class Camera:public QObject{
    Q_OBJECT
public:
    explicit Camera(QObject* parent=nullptr);
    QString takePicture();//拍照
    void startRecorder(QString orderInfo);//录像
    void stopRecorder();//停止录像,总结束
private:
    void createVideo();
private slots:
    void timer1000TimeOutSlot();
private:
    quint32 _timerCount=0;
    QString _videoDirPath;
};

#endif // T_CAMERA_H
