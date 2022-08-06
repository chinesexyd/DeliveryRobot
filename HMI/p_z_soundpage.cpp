#include "p_z_soundpage.h"

/*@@@@@@@@@@播放声音@@@@@@@@@@@*/
SoundThread::SoundThread(QString filepath,QWidget* parent):QThread(parent){
    _sound=new QSound(filepath,this);
    connect(this,&QThread::finished,[this](){deleteLater();});
    _filename=filepath.split("/").last();
}
void SoundThread::run(){
    _sound->play();
    while (!_sound->isFinished()) {
        Sleep(100);
    }
    _sound->deleteLater();
    emit hmi->soundIsFinishedSignal(_filename);
}

/*@@@@@@@@@@调节声音@@@@@@@@@@@*/
MSoundPage::MSoundPage(QWidget* parent):QFrame(parent){
    _spdLine=new SlideLineBar(this);
    _spdLine->setRange(0,15);
    _spdLine->setName("语速");
    _spdLine->setPixmap(":/Image/VoiceSpeed.png");
    _pitLine=new SlideLineBar(this);
    _pitLine->setRange(0,15);
    _pitLine->setName("语调");
    _pitLine->setPixmap(":/Image/VoicePit.png");
    _volLine=new SlideLineBar(this);
    _volLine->setRange(0,15);
    _volLine->setName("音量");
    _volLine->setPixmap(":/Image/VoiceVol.png");
    _perCombo = new ChooseCommbo(this);
    _perCombo->setName("人物");
    _perCombo->setPixmap(":/Image/VoicePerson.png");
    QStringList perList;
    perList<<"小宇"<<"小美"<<"逍遥"<<"丫丫";
    _perCombo->setStringList(perList);
    _layout0=new QVBoxLayout();
    _layout0->setContentsMargins(0,0,0,0);
    _layout0->addWidget(_spdLine);
    _layout0->addWidget(_pitLine);
    _layout0->addWidget(_volLine);
    _layout0->addWidget(_perCombo);
    _testText=new QTextEdit(this);
    _testText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _testText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _testText->setReadOnly(true);
    _testText->setStyleSheet("background-color:transparent;color:white;border:none;font:13pt;");
    _testButton=new CommonButton(this);
    _testButton->setText("试听");
    _testButton->setPixmap(":/Image/ButtonVoiceTest.png");
    _confirmButton=new CommonButton(this);
    _confirmButton->setText("生成语音");
    _confirmButton->setPixmap(":/Image/ButtonVoiceConfirm.png");
    _layout=new QGridLayout(this);
    _layout->setContentsMargins(30,30,30,30);
    _layout->setHorizontalSpacing(60);
    _layout->setHorizontalSpacing(60);
    _layout->addLayout(_layout0,0,0,3,1);
    _layout->addWidget(_confirmButton,3,0,1,1,Qt::AlignCenter);
    _layout->addWidget(_testText,0,1,1,1);
    _layout->addWidget(_testButton,3,1,1,1,Qt::AlignCenter);
    _layout->setColumnStretch(0,7);
    _layout->setColumnStretch(1,3);
    setLayout(_layout);
    initVoiceInfo();
    initVoiceText();
    connect(_spdLine,SIGNAL(changedSignal(quint8)),this,SLOT(spdValueChangedSlot(quint8)));
    connect(_pitLine,SIGNAL(changedSignal(quint8)),this,SLOT(pitValueChangedSlot(quint8)));
    connect(_volLine,SIGNAL(changedSignal(quint8)),this,SLOT(volValueChangedSlot(quint8)));
    connect(_perCombo,SIGNAL(changedSignal(QString)),this,SLOT(perValueChangedSlot(QString)));
    connect(_testButton,SIGNAL(clicked()),this,SLOT(testButtonClickedSlot()));
    connect(_confirmButton,SIGNAL(clicked()),this,SLOT(confirmButtonClickedSlot()));
    connect(tts,SIGNAL(createSoundSuccess(QString)),this,SLOT(receiveSoundSlot(QString)));
    connect(hmi,SIGNAL(createAdvertVideoSignal()),this,SLOT(createAdvertVideoSlot()));
    connect(hmi,SIGNAL(soundIsFinishedSignal(QString)),this,SLOT(soundIsFinishedSlot(QString)));
}
void MSoundPage::spdValueChangedSlot(quint8 value){
    _voiceInfo.spd = value;
    _isVoiceInfoChanged=true;
}
void MSoundPage::pitValueChangedSlot(quint8 value){
    _voiceInfo.pit = value;
    _isVoiceInfoChanged=true;
}
void MSoundPage::volValueChangedSlot(quint8 value){
    _voiceInfo.vol = value;
    _isVoiceInfoChanged=true;
}
void MSoundPage::perValueChangedSlot(QString value){
    if(value=="小宇"){
        _voiceInfo.per = HMI::DUXIAOYU;
    }else if(value=="小美"){
        _voiceInfo.per = HMI::DUXIAOMEI;
    }else if(value=="逍遥"){
        _voiceInfo.per = HMI::DUXIAOYAO;
    }else if(value=="丫丫"){
        _voiceInfo.per =HMI:: DUYAYA;
    }else{}
    _isVoiceInfoChanged=true;
}
void MSoundPage::initVoiceInfo(){
    HMI::VOICE_INFO voiceInfo;
    QJsonObject personVoiceObj = config->getValue("voicePerson").toObject();
    voiceInfo.spd = personVoiceObj.value("spd").toInt();
    voiceInfo.pit = personVoiceObj.value("pit").toInt();
    voiceInfo.vol = personVoiceObj.value("vol").toInt();
    voiceInfo.per = (HMI::VOICE_PERSON)personVoiceObj.value("per").toInt();
    _spdLine->setValue(voiceInfo.spd);
    _pitLine->setValue(voiceInfo.pit);
    _volLine->setValue(voiceInfo.vol);
    switch (voiceInfo.per) {
    case HMI::DUXIAOYU:
        _perCombo->setChoose("小宇");
        break;
    case HMI::DUXIAOMEI:
        _perCombo->setChoose("小美");
        break;
    case HMI::DUXIAOYAO:
        _perCombo->setChoose("逍遥");
        break;
    case HMI::DUYAYA:
        _perCombo->setChoose("丫丫");
        break;
    default:
        break;
    }
    _voiceInfo=_voiceInfoTag= voiceInfo;
}
void MSoundPage::testButtonClickedSlot(){
    _testButton->setEnabled(false);
    QString testText = _testText->toPlainText();
    if(_isVoiceInfoChanged){
        hmi->sendLogicTextSignal("试听语音正在生成及播放");
        tts->setVoiceInfo(_voiceInfo);
        tts->createSpeechFromText(testText,"test.wav");
        _isVoiceInfoChanged=false;
    }else{
        QString filePath = BAIDUVOICE_PATH+"/test.wav";
        QFile file(filePath);
        if(file.exists()){
            hmi->sendLogicTextSignal("试听语音正在生成及播放");
            SoundThread* sound=new SoundThread(filePath,this);
            sound->start();
        }else{
            hmi->sendLogicTextSignal("试听语音正在生成及播放");
            tts->setVoiceInfo(_voiceInfo);
            tts->createSpeechFromText(testText,"test.wav");
        }
    }
}
void MSoundPage::receiveSoundSlot(QString filename){
    if(filename=="test.wav"){
        QString filePath = BAIDUVOICE_PATH+"/test.wav";
        SoundThread* sound=new SoundThread(filePath,this);
        sound->start();
    }else{
        if(__voiceCount>0){
            __voiceCount--;
            hmi->sendLogicTextSignal("正在生成自定义语音中");
            tts->createSpeechFromText(__voiceTextObj.value(__voiceStringList.at(__voiceCount)).toString(),QString("%1.wav").arg(__voiceStringList.at(__voiceCount)));
        }else{
            hmi->sendLogicTextSignal("自定义语音文件已经生成完毕");
            if(__adverCount>0){
                __adverCount--;
                hmi->sendLogicTextSignal("正在生成广告语音中");
                tts->createSpeechFromText(__adverStringList.at(__adverCount),QString("%1.wav").arg(__adverCount));
            }else{
                hmi->sendLogicTextSignal("广告语音文件已经生成完毕");
            }
        }
    }
}
void MSoundPage::confirmButtonClickedSlot(){
    if(_voiceInfo.spd!=_voiceInfoTag.spd||_voiceInfo.pit!=_voiceInfoTag.pit||_voiceInfo.vol!=_voiceInfoTag.vol||_voiceInfo.per!=_voiceInfoTag.per){
        tts->saveVoiceInfo(_voiceInfo);//声音设置保存到本地
        _voiceInfoTag = _voiceInfo;
        //
        __voiceTextObj = config->getValue("voiceText").toObject();
        __voiceStringList = __voiceTextObj.keys();
        __voiceStringList.removeAt(__voiceStringList.indexOf("test"));
        __voiceCount = __voiceStringList.length();
        QStringList fakeAdverStringList = config->getValue("advertWordList").toVariant().toStringList();
        if(fakeAdverStringList.length()>0){
            QString advertString=fakeAdverStringList.at(0);
            if(advertString.length()>60){
                __adverStringList =advertString.split("，");
                __adverCount = __adverStringList.length();
            }else{
                QStringList __adverStringList;
                __adverStringList.append(advertString);
                __adverCount = 1;
            }
            tts->setVoiceInfo(_voiceInfo);
            tts->createSpeechFromText("这是一个触发器，没有实际意义。","xxx.wav");
        }else{
            //广告语为空
        }
    }else{
        hmi->sendLogicTextSignal("相同音色的语音文件已存在");
    }
}
void MSoundPage::createAdvertVideoSlot(){
    __voiceCount =0;
    QStringList fakeAdverStringList = config->getValue("advertWordList").toVariant().toStringList();
    if(fakeAdverStringList.length()>0){
        QString advertString=fakeAdverStringList.at(0);
        if(advertString.length()>60){
            __adverStringList =advertString.split("，");
            __adverCount = __adverStringList.length();
        }else{
            QStringList __adverStringList;
            __adverStringList.append(advertString);
            __adverCount = 1;
        }
        tts->setVoiceInfo(_voiceInfo);
        tts->createSpeechFromText("这是一个触发器，没有实际意义。","xxx.wav");
    }else{
        //广告语为空
    }
}
void MSoundPage::initVoiceText(){
    QString testText = config->getValue("voiceText").toObject().value("test").toString();
    _testText->setText(testText);
}
void MSoundPage::soundIsFinishedSlot(QString filename){
    if(filename=="test.wav"){
        Sleep(1000);
        _testButton->setEnabled(true);
    }
}
