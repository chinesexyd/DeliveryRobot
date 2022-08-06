#ifndef ROBOTCONST_H
#define ROBOTCONST_H

#include <QObject>

class Const{
public:
    static const quint8 reconnectRosTime=20;//重新连接Ros的时间 单位秒
    static const quint8 pingHttpTime=60;
    static const quint8 checkMqIsconnectTime=20;
    static const quint8 reconnectMqTime=60;//重新连接MQ的时间，单位秒
};

#endif // ROBOTCONST_H
