#ifndef TTS_H
#define TTS_H

#include"a_global.h"

class TTS:public QObject{
    Q_OBJECT
public:
    explicit TTS(QObject *parent = nullptr);
    void saveVoiceInfo(HMI::VOICE_INFO);//设置时保存到文件
    void setVoiceInfo(HMI::VOICE_INFO);//试听时设置
    void createSpeechFromText(QString text,QString filename);
    void runAdvertisements(QStringList);//播放广告
    void stopAdvertisements();//停止播放广告
private:
    void _receiveToken(QByteArray);
    void _receiveVoice(QByteArray);
    void _createToken();
    void _checkTokenIsAvilable();//检查token是否过期
public slots:
    void finishedSlot(QNetworkReply*);
signals:
    void createSoundSuccess(QString);
private:
    QNetworkAccessManager* _netManager;
    QString _baiduTtsUrl = "http://tsn.baidu.com/text2audio";
    QString _baiduTokenUrl;
    QString _appid = "17650598";
    QString _apikey = "SBUTHmVXmQixFkQHitoRz7sb";
    QString _secretkey = "OyGGh3GaFFq1lb539zac54gGTwUAOjIa";
    QString _filename;
    HMI::VOICE_INFO _voiceInfo;
};
#endif // TTS_H
