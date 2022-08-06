#ifndef ROOT_H
#define ROOT_H
////////////////
#include <QWidget>
#include <QFrame>
#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <QTextEdit>
#include <QtSingleApplication>
#include <QPropertyAnimation>
#include <QThread>
////////////////
#include <QStackedLayout>
#include <QButtonGroup>
#include <QFormLayout>
////////////////
#include <QPainter>
#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QList>
#include <QCryptographicHash>
#include <QSound>
////////////////
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QSslConfiguration>
#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUdpSocket>
////////////////
#include "iostream"
#include "sstream"
#include "HMI/t_hmi.h"
#include "HMI/t_config.h"
#include "HMI/a_const.h"
#include "qrencode.h"
////////////////
extern "C"{
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
////////////////
#define VERSON "20220805"
/*=============全局=============*/
extern HMI* hmi;
extern Config* config;
/*==========================*/
typedef struct{
    double x;
    double y;
    double angle;
}XYA_POINT;
typedef struct{
    double x;
    double y;
    double z;
    double w;
}XYZW_POINT;
/*============路径==============*/
extern QString HOME_PATH;//home路径
extern QString NOTE_PATH;
extern QString PROVINCES_PATH;
extern QString MAP_PATH;
extern QString BAIDUVOICE_PATH;
extern QString ROS_LOG_PATH;
extern QString VIDEO_PATH;
extern QString IMAGE_PATH;
extern QString UPPERSOFT_PATH;
extern QString MIDDLESOFT_PATH;
extern QString LOWERSOFT_PATH;
extern QString ROBOTINFO_PATH;
/*============颜色渐变器==============*/
typedef struct{
    int r;
    int g;
    int b;
}RGB_COLOR;
class ColorChange{
public:
    ColorChange();
    ~ColorChange();
    void setStartColor(QString startColor);
    void setStartColor(int r,int g,int b);
    void setStartColor(RGB_COLOR startColor);
    void setEndColor(QString endColor);
    void setEndColor(int r,int g,int b);
    void setEndColor(RGB_COLOR endColor);
    void setMinNum(int min);
    void setMaxNum(int max);
    void setRange(int min,int max);
    RGB_COLOR getColor(int currentNum);
private:
    RGB_COLOR _startColor;
    RGB_COLOR _endColor;
    int _min;
    int _max;
};

#endif // ROOT_H
