#include "t_voice.h"

Voice::Voice(QWidget* parent):QObject(parent){
    connect(timer,SIGNAL(timer100TimeoutSignal()),this,SLOT(timer100TimeoutSlot()));
}
void Voice::playRoundAdvert(qint8 roundNum){
    if(_currentSoundInfo.sound!=nullptr){
        if(!_currentSoundInfo.sound->isFinished()){
            _currentSoundInfo.sound->stop();
        }
        _currentSoundInfo.sound->deleteLater();
        _currentSoundInfo.sound=nullptr; 
    }
    quint8 advertLength=config->getValue("advertWordList").toVariant().toStringList().length();
    if(advertLength>0){
        QString filepath=QString("%1/%2").arg(BAIDUVOICE_PATH).arg("0.wav");
        QFile file(filepath);
        if(file.exists()){
            _currentSoundInfo.priority=S_HIGH;
            _currentSoundInfo.filename="0.wav";
            _currentSoundInfo.isStop=false;
            _currentSoundInfo.roundNum=_currentSoundInfo.surplusRoundNum=roundNum;
            _currentSoundInfo.isAdvert=true;
            _currentSoundInfo.advertNum=config->getValue("advertWordList").toVariant().toStringList().at(0).split("，").length();
            _currentSoundInfo.advertIndex=0;
            _currentSoundInfo.sound=new QSound(filepath,this);
            _currentSoundInfo.sound->play();
        }else{
            note->writeLine(Note::M_HMI_ERROR,"播放音频",QString("音频文件不存在：%1").arg(filepath));
        }
    }else{}
}
void Voice::playRoundSound(SOUND_PRIORITY priority,QString filename,qint8 roundNum){
    if(priority>=_currentSoundInfo.priority){
        if(_currentSoundInfo.sound!=nullptr){
            if(!_currentSoundInfo.sound->isFinished()){
                _currentSoundInfo.sound->stop();
            }
            _currentSoundInfo.sound->deleteLater();
            _currentSoundInfo.sound=nullptr;
        }
        QString filepath=QString("%1/%2").arg(BAIDUVOICE_PATH).arg(filename);
        QFile file(filepath);
        if(file.exists()){
            _currentSoundInfo.priority=priority;
            _currentSoundInfo.filename=filename;
            _currentSoundInfo.isStop=false;
            _currentSoundInfo.roundNum=_currentSoundInfo.surplusRoundNum=roundNum;
            _currentSoundInfo.sound=new QSound(filepath,this);
            _currentSoundInfo.isAdvert=false;
            _currentSoundInfo.sound->play();
            _currentSoundInfo.surplusRoundNum--;
        }else{
            note->writeLine(Note::M_HMI_ERROR,"播放音频",QString("音频文件不存在：%1").arg(filepath));
        }
    }else{}
}
void Voice::stop(){
    _currentSoundInfo.isStop=true;
}
void Voice::stop(QString filename){
    if(filename==_currentSoundInfo.filename){
        _currentSoundInfo.isStop=true;
    }else{}
}
void Voice::timer100TimeoutSlot(){
    if(_currentSoundInfo.sound!=nullptr){
        if(_currentSoundInfo.sound->isFinished()){
            if(_currentSoundInfo.isStop){
                _currentSoundInfo.sound->stop();
                _currentSoundInfo.roundNum=0;
                _currentSoundInfo.surplusRoundNum=0;
                _currentSoundInfo.sound->deleteLater();
                _currentSoundInfo.sound=nullptr;
            }else{
                if(_currentSoundInfo.isAdvert){
                    if(_currentSoundInfo.surplusRoundNum==-1){
                        _currentSoundInfo.advertIndex++;
                        _currentSoundInfo.advertIndex=_currentSoundInfo.advertIndex%_currentSoundInfo.advertNum;
                        _currentSoundInfo.sound->deleteLater();
                        _currentSoundInfo.sound=nullptr;
                        QString filepath=QString("%1/%2.wav").arg(BAIDUVOICE_PATH).arg(_currentSoundInfo.advertIndex);
                        QFile file(filepath);
                        if(file.exists()){
                            Sleep(50);
                            _currentSoundInfo.sound=new QSound(filepath,this);
                            _currentSoundInfo.sound->play();
                        }else{
                            note->writeLine(Note::M_HMI_ERROR,"播放音频",QString("音频文件不存在：%1").arg(filepath));
                        }
                    }else if(_currentSoundInfo.surplusRoundNum>0){
                        _currentSoundInfo.advertIndex++;
                        _currentSoundInfo.advertIndex=_currentSoundInfo.advertIndex%_currentSoundInfo.advertNum;
                        _currentSoundInfo.sound->deleteLater();
                        _currentSoundInfo.sound=nullptr;
                        QString filepath=QString("%1/%2.wav").arg(BAIDUVOICE_PATH).arg(_currentSoundInfo.advertIndex);
                        QFile file(filepath);
                        if(file.exists()){
                            Sleep(50);
                            _currentSoundInfo.sound=new QSound(filepath,this);
                            _currentSoundInfo.sound->play();
                        }else{
                            note->writeLine(Note::M_HMI_ERROR,"播放音频",QString("音频文件不存在：%1").arg(filepath));
                        }
                        if(_currentSoundInfo.advertIndex==_currentSoundInfo.advertNum-1){
                            _currentSoundInfo.surplusRoundNum--;
                        }
                    }else{
                        _currentSoundInfo.priority=S_LOW;
                    }
                }
                //如果不是广告语
                else{
                    if(_currentSoundInfo.surplusRoundNum==-1){
                        _currentSoundInfo.sound->play();
                    }else if(_currentSoundInfo.surplusRoundNum>0){
                        _currentSoundInfo.sound->play();
                        _currentSoundInfo.surplusRoundNum--;
                    }else{
                        _currentSoundInfo.priority=S_LOW;
                    }
                }
            }
        }else{
            if(_currentSoundInfo.isStop){
                _currentSoundInfo.sound->stop();
                _currentSoundInfo.roundNum=0;
                _currentSoundInfo.sound->deleteLater();
                _currentSoundInfo.sound=nullptr;
                _currentSoundInfo.priority=S_LOW;    
            }else{}
        }
    }else{}
}
