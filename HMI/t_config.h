#ifndef G_CONFIG_H
#define G_CONFIG_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QDebug>

/*@@@@@@@@@@@180*250@@@@@@@@@@@*/
class Config:public QObject{
    Q_OBJECT
public:
    explicit Config(QObject* parent=nullptr);
    QJsonValue getValue(QString key);
    void setValue(QString key,QString value);
    void setValue(QString key,quint8 value);
    void setValue(QString key,quint16 value);
    void setValue(QString key,quint32 value);
    void setValue(QString key,quint64 value);
    void setValue(QString key,qint8 value);
    void setValue(QString key,qint16 value);
    void setValue(QString key,qint32 value);
    void setValue(QString key,qint64 value);
    void setValue(QString key,bool value);
    void setValue(QString key,QJsonArray value);
    void setValue(QString key,QJsonObject value);
private:
    void save();
    void createConfigFile();//自动生成配置文件
    void coverConfigFile();//覆盖原来的文件
    void repairConfigFile();//修复原来的文件
public:
    QMap<quint8,QString> numberMap;
private:
    QJsonObject _configJson;
    const QString _configPath=QDir::homePath()+"/DeliveryRobot/DeliveryRobot/Config/robot.json";
};

#endif // G_CONFIG_H
