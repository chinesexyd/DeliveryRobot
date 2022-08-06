#ifndef T_OBS_H
#define T_OBS_H

#include "a_global.h"

class OBS:public QThread{
    Q_OBJECT
public:
    struct SIGNATRUE{
        QString httpVerb="";
        QString contentType="";
        QString contentMD5="";
        QString date="";
        QString canonicalizedHeaders="";
        QString canonicalizedResource="";
    };
    struct PUT{
        quint64 orderId;
        QString customerAddress;
        QString status;
    };
public:
    explicit OBS(QObject* parent=nullptr);
    void put(quint64 orderId,QString customerAddress,QString status);
    inline void initMapPath(QString mapPath){_mapPath=mapPath;};
protected:
    void run() override;
private:
    QByteArray hmacSha1(QByteArray key, QByteArray baseString);
    QByteArray createAuthorization(SIGNATRUE signature);
    QString formatDate();
signals:
    void putSuccessSignal(bool);
    void sendData(QByteArray data);
public slots:
    void finishedSlot(QNetworkReply* reply);
    void receiveData(QByteArray data);
private:
    QNetworkAccessManager* _netManager;
    const QString _userName="hw27575955";
    const QString _accessKeyId="VMV7KZXOXCPLEB4J90DY";                        
    const QString _secretAccessKey="MUg18JONwN5EJHfvGXXIABwaDvN4elhiGJsWPAcA";
    const QString _endPoint="obs.cn-east-3.myhuaweicloud.com";
    const QString _accountID="0aa5cdabd280f5fe0f0fc01c85f7f7a0";//帐号
    const QString _accountAccessDomainName="https://ros-map.obs.cn-east-3.myhuaweicloud.com";//访问域名
    QString _mapPath;
    PUT _put;
};

#endif // T_OBS_H

