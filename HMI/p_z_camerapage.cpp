#include "p_z_camerapage.h"

MCameraPage::MCameraPage(QWidget *parent) : QFrame(parent){
    _pictureNumberCombo=new ChooseCommbo(this);
    _pictureNumberCombo->setName("保存图片数量");
    _pictureNumberCombo->setPixmap(":/Image/PictureNumber.png");
    QStringList pictureNumberList;
    pictureNumberList<<"100"<<"200"<<"500"<<"1000";
    _pictureNumberCombo->setStringList(pictureNumberList);
    _videoNumberCombo=new ChooseCommbo(this);
    _videoNumberCombo->setName("保存视频数量");
    _videoNumberCombo->setPixmap(":/Image/videoNumber.png");
    QStringList videoNumberList;
    videoNumberList<<"3"<<"5"<<"10"<<"15"<<"20";
    _videoNumberCombo->setStringList(videoNumberList);
    _videoIntervalCombo=new ChooseCommbo(this);
    _videoIntervalCombo->setName("视频录制间隔时长");
    _videoIntervalCombo->setPixmap(":/Image/VideoInterval.png");
    QStringList videoIntervalList;
    videoIntervalList<<"1分钟"<<"2分钟"<<"3分钟";
    _videoIntervalCombo->setStringList(videoIntervalList);
    _layout0=new QVBoxLayout();
    _layout0->setContentsMargins(0,0,0,0);
    _layout0->addWidget(_pictureNumberCombo);
    _layout0->addWidget(_videoNumberCombo);
    _layout0->addWidget(_videoIntervalCombo);
    _confirmButton=new CommonButton(this);
    _confirmButton->setText("应用");
    _confirmButton->setPixmap(":/Image/ButtonCameraConfirm.png");
    _cameraLabel = new PixmapLabel(this);
    _takePictureButton=new CommonButton(this);
    _takePictureButton->setText("拍照");
    _takePictureButton->setPixmap(":/Image/ButtonTakePicture.png");
    _layout=new QGridLayout(this);
    _layout->setContentsMargins(30,30,30,30);
    _layout->setHorizontalSpacing(60);
    _layout->setVerticalSpacing(80);
    _layout->addLayout(_layout0,0,0,3,1);
    _layout->addWidget(_confirmButton,3,0,1,1,Qt::AlignCenter);
    _layout->addWidget(_cameraLabel,0,1,3,1);
    _layout->addWidget(_takePictureButton,3,1,1,1,Qt::AlignCenter);
    _layout->setColumnStretch(0,7);
    _layout->setColumnStretch(1,3);
    setLayout(_layout);
    init();
    connect(_confirmButton,SIGNAL(clicked()),this,SLOT(confirmButtonClickedSlot()));
    connect(_takePictureButton,SIGNAL(clicked()),this,SLOT(takePictureSlot()));
}
void MCameraPage::init(){
    QString pictureNumber=QString::number(config->getValue("pictureNumber").toInt());
    _pictureNumberCombo->setChoose(pictureNumber);
    QString videoNumber=QString::number(config->getValue("videoNumber").toInt());
    _videoNumberCombo->setChoose(videoNumber);
    QString videoSinceTime=QString::number(config->getValue("videoSinceTime").toInt())+"分钟";
    _videoIntervalCombo->setChoose(videoSinceTime);
}
void MCameraPage::takePictureSlot(){
    QString pictureName=camera->takePicture();
    Sleep(200);//等待图片保存
    QString picturePath=IMAGE_PATH+"/"+pictureName;
    QFile pictureFile(picturePath);
    if(pictureFile.exists()){
        _cameraLabel->setPixmap(picturePath);
    }
}
void MCameraPage::confirmButtonClickedSlot(){
    quint32 pictureNumber=_pictureNumberCombo->getChoose().toInt();
    quint32 videoNumber=_videoNumberCombo->getChoose().toInt();
    quint32 videoInteral=_videoIntervalCombo->getChoose().remove("分钟").toInt();
    config->setValue("pictureNumber",pictureNumber);
    config->setValue("videoNumber",videoNumber);
    config->setValue("videoSinceTime",videoInteral);
    hmi->sendLogicTextSignal("已应用视频相关设置");
}
