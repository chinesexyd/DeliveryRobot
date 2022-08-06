#include "t_camera.h"

/*@@@@@@@@@@@全局ROS@@@@@@@@@@@*/
ROS* ros=new ROS();

/*@@@@@@@@@@@@@@@@@@@@@@*/
Camera::Camera(QObject* parent):QObject(parent){}
QString Camera::takePicture(){
    QDir pictureDir(IMAGE_PATH);
    pictureDir.setFilter(QDir::Files);
    pictureDir.setSorting(QDir::Time);
    QFileInfoList fileInfoList=pictureDir.entryInfoList();
    int max=config->getValue("pictureNumber").toInt();
    int num=fileInfoList.length();
    if(num>max){
        for(int i=max;i<num;i++){
            QFileInfo info=fileInfoList[i];
            QFile file(info.fileName());
            file.remove();
        }
    }
    QString newFileName=QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz.jpg");
    QString newFIlePath=IMAGE_PATH+"/"+newFileName;
    ros->takePicture(newFIlePath);
    return newFileName;
}
void Camera::startRecorder(QString orderInfo){
    _timerCount=0;
    QDir videoRootDir(VIDEO_PATH);
    videoRootDir.setFilter(QDir::Dirs|QDir::NoDot|QDir::NoDotDot);
    videoRootDir.setSorting(QDir::Time);
    QFileInfoList videoDirInfoList=videoRootDir.entryInfoList();
    int max=config->getValue("videoNumber").toInt();
    int num=videoDirInfoList.length();
    if(num>max){
        for(int i=max;i<num;i++){
            QFileInfo info=videoDirInfoList[i];
            QDir dir(info.filePath());
            dir.removeRecursively();
        }
    }
    QString newDirName=QDateTime::currentDateTime().toString("yyyy-MM--dd-hh-mm")+"--"+orderInfo;
    _videoDirPath=VIDEO_PATH+"/"+newDirName;
    QDir dir;
    dir.mkpath(_videoDirPath);
    connect(timer,SIGNAL(timer1000TimeoutSignal()),this,SLOT(timer1000TimeOutSlot()));
}
void Camera::stopRecorder(){
    ros->stopRecorder();
    disconnect(timer,SIGNAL(timer1000TimeoutSignal()),this,SLOT(timer1000TimeOutSlot()));
}
void Camera::timer1000TimeOutSlot(){
    _timerCount++;
    _timerCount=_timerCount%(config->getValue("videoSinceTime").toInt()*60);
    if(_timerCount==1){
        createVideo();
    }
}
void Camera::createVideo(){
    ros->stopRecorder();
    QString videoName=QDateTime::currentDateTime().toString("hh-mm-ss")+".mp4";
    QString videoPath=_videoDirPath+"/"+videoName;
    ros->startRecorder(videoPath);
}
