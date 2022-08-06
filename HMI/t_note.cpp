#include "t_note.h"

Note::Note(){}
void Note::writeLine(MESSAGE_TYPE messageType,QString type,QString info){
    QString _nowDay = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString _nowFilePath = QString("%1/%2.txt").arg(NOTE_PATH).arg(_nowDay);
    QFile _nowFile(_nowFilePath);
    QString message = createMessage(messageType,type,info);
    if(!_nowFile.exists()){
        // 新建文建
        removeFrontFile();
        _nowFile.open(QIODevice::WriteOnly |QIODevice::Append|QIODevice::Text);
        _nowFile.write("————————————————————————————————————————\n");
        _nowFile.write("仅保存一个月的日志文件\n");
        _nowFile.write(QString("日期：%1\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd dddd")).toUtf8());
        _nowFile.write("格式：yyyy-MM-dd hh:mm:ss:zzz [消息来源] (类型) : 内容\n");
        _nowFile.write("————————————————————————————————————————\n");
        _nowFile.write(message.toUtf8());
        _nowFile.flush();
        _nowFile.close();
    }else{
        _nowFile.open(QIODevice::WriteOnly |QIODevice::Append|QIODevice::Text);
        _nowFile.write(message.toUtf8());
        _nowFile.flush();
        _nowFile.close();
    }
}
void Note::removeFrontFile(){
    QDateTime frontMonthDay = QDateTime::currentDateTime().addMonths(-1);
    qint64 modifyTime=frontMonthDay.toSecsSinceEpoch();
    QDir dir(NOTE_PATH);
    foreach (const QFileInfo& info, dir.entryInfoList()){
        if(info.lastModified().toSecsSinceEpoch()<=modifyTime){
            QFile file(info.filePath());
            file.remove();
        }
    }
    //清理Ros日志
    QDir rosDir(ROS_LOG_PATH);
    if(rosDir.exists()){
        foreach(const QFileInfo& fileInfo,rosDir.entryInfoList()){
            if(fileInfo.lastModified().toSecsSinceEpoch()<=modifyTime){
                QFile rosfile(fileInfo.filePath());
                rosfile.remove();
            }
        }
    }
}
QString Note::createMessage(MESSAGE_TYPE messageType,QString type,QString info){
    QString _nowTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    QString _messageType;
    switch (messageType){
    case M_MQ:
        _messageType = "MQ";
        break;
    case M_ROS:
        _messageType = "ROS";
        break;
    case M_HTTP:
        _messageType = "HTTP";
        break;
    case M_MQ_ERROR:
        _messageType = "MQ-ERROR";
        break;
    case M_ROS_ERROR:
        _messageType = "ROS-ERROR";
        break;
    case M_HTTP_ERROR:
        _messageType = "HTTP-ERROR";
        break;
    case M_FROM_MQ:
        _messageType = "MQ->ROBOT";
        break;
    case M_FROM_ROS:
        _messageType = "ROS->ROBOT";
        break;
    case M_FROM_HTTP:
        _messageType = "HTTP->ROBOT";
        break;
    case M_TO_MQ:
        _messageType = "ROBOT->MQ";
        break;
    case M_TO_ROS:
        _messageType = "ROBOT->ROS";
        break;
    case M_TO_HTTP:
        _messageType = "ROBOT->HTTP";
        break;
    case M_HMI:
        _messageType = "HMI";
        break;
    case M_HMI_WARN:
        _messageType = "HMI-WARN";
        break;
    case M_HMI_ERROR:
        _messageType = "HMI-ERROR";
        break;
    case M_TTS:
        _messageType = "TTS";
        break;
    default:
        break;
    }
    qDebug()<<QString("%1 [%2] (%3) : %4").arg(_nowTime).arg(_messageType).arg(type).arg(info);
    return QString("\n%1 [%2] (%3) : %4").arg(_nowTime).arg(_messageType).arg(type).arg(info);
}
