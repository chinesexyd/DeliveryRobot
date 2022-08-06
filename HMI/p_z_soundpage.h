#ifndef MSOUNDPAGE_H
#define MSOUNDPAGE_H

#include "HMI/a_logic.h"

/*@@@@@@@@@@播放声音@@@@@@@@@@@*/
class SoundThread:public QThread{
    Q_OBJECT
public:
    explicit SoundThread(QString filepath,QWidget* parent);
protected:
    void run() override;
private:
    QSound* _sound;
    QString _filename;
};
/*@@@@@@@@@@调节声音@@@@@@@@@@@*/
class MSoundPage : public QFrame{
    Q_OBJECT
public:
    explicit MSoundPage(QWidget *parent = nullptr);
private:
    void initVoiceText();
    void initVoiceInfo();
public slots:
    void spdValueChangedSlot(quint8);
    void pitValueChangedSlot(quint8);
    void volValueChangedSlot(quint8);
    void perValueChangedSlot(QString);
    void testButtonClickedSlot();
    void receiveSoundSlot(QString);
    void confirmButtonClickedSlot();
    void createAdvertVideoSlot();
    void soundIsFinishedSlot(QString);
private:
    QVBoxLayout* _layout0;
    QGridLayout* _layout;
    SlideLineBar* _spdLine;
    SlideLineBar* _pitLine;
    SlideLineBar* _volLine;
    ChooseCommbo* _perCombo;
    QTextEdit* _testText;
    CommonButton* _testButton;
    CommonButton* _confirmButton;
    QSound* _testSound;
    //
    HMI::VOICE_INFO _voiceInfo;
    HMI::VOICE_INFO _voiceInfoTag;
    bool _isVoiceInfoChanged=false;
    //
    QStringList __voiceStringList;
    quint8 __voiceCount;
    QJsonObject __voiceTextObj;
    //
    QStringList __adverStringList;
    quint8 __adverCount;
};

#endif // MSOUNDPAGE_H
