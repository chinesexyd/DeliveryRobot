#ifndef note_H
#define note_H

#include "HMI/a_root.h"
/*@@@@@@@日志@@@@@@@*/
class Note{
public:
    enum MESSAGE_TYPE{
        M_MQ,
        M_ROS,
        M_HTTP,
        M_MQ_ERROR,
        M_ROS_ERROR,
        M_HTTP_ERROR,
        M_FROM_MQ,
        M_FROM_ROS,
        M_FROM_HTTP,
        M_TO_MQ,
        M_TO_ROS,
        M_TO_HTTP,
        M_HMI,
        M_HMI_WARN,
        M_HMI_ERROR,
        M_TTS,
    };
public:
    explicit Note();
    void writeLine(MESSAGE_TYPE,QString type,QString info);
    void removeFrontFile();
private:
    QString createMessage(MESSAGE_TYPE messageType,QString type,QString info);
};

#endif // note_H
