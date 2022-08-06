#ifndef T_VOICE_H
#define T_VOICE_H

#include "HMI/a_global.h"

typedef enum{
    S_HIGH=3,
    S_MIDDLE=2,
    S_LOW=1
}SOUND_PRIORITY;//音频优先级

typedef struct{
    QString filename;//文件名
    SOUND_PRIORITY priority=S_LOW;//优先级
    QSound* sound=nullptr;//音频
    qint8 roundNum=0;//总次数
    qint8 surplusRoundNum=0;//剩余次数
    bool isStop=false;//是否停止播放
    bool isAdvert=false;//是否是广告语
    quint8 advertNum=0;//广告的数量
    quint8 advertIndex=0;//广告的下标
}SOUND_INFO;

class Voice:public QObject{
    Q_OBJECT
public:
    explicit Voice(QWidget* parent=nullptr);
    void playRoundAdvert(qint8 roundNum=1);//播放广告语，循环一定次数再停止,负一不停止一直循环
    void playRoundSound(SOUND_PRIORITY priority,QString filename,qint8 roundNum=1);//播放单个文件，循环一定次数再停止，负一不停止一直循环
    void stop();//停止循环播放单个文件
    void stop(QString filename);
public slots:
    void timer100TimeoutSlot();//用于检查是否播放完
private:
    SOUND_INFO _currentSoundInfo;
};

#endif // T_VOICE_H
