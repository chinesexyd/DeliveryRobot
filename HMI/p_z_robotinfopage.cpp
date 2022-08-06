#include "p_z_robotinfopage.h"
/*@@@@@@@@@@@设置图片@@@@@@@@@*/
MRobotInfoPage::MRobotInfoPage(QWidget* parent):QFrame(parent){
    _robot=new PixmapLabel(this);
    _robot->setPixmap(":/Image/Robot.png");
    _robot->setGeometry(275,0,693,455);
    _macLabel = new RobotInfoLabel(this);
    _buildTimeLabel = new RobotInfoLabel(this);
    _collisionLabel = new RobotInfoLabel(this);
    _curLabel = new RobotInfoLabel(this);
    _imuLabel = new RobotInfoLabel(this);
    _mapFileLabel = new RobotInfoLabel(this);
    _milsLabel = new RobotInfoLabel(this);
    _macLabel->setName("MAC地址");
    _buildTimeLabel->setName("编译日期");
    _collisionLabel->setName("碰撞");
    _curLabel->setName("电流");
    _imuLabel->setName("陀螺仪");
    _mapFileLabel->setName("地图");
    _milsLabel->setName("里程");
    _macLabel->setPixmap(":/Image/RobotInfoMac.png");
    _buildTimeLabel->setPixmap(":/Image/RobotInfoBuildTime.png");
    _collisionLabel->setPixmap(":/Image/RobotInfoCollision.png");
    _curLabel->setPixmap(":/Image/RobotInfoCur.png");
    _imuLabel->setPixmap(":/Image/RobotInfoImu.png");
    _mapFileLabel->setPixmap(":/Image/RobotInfoMapPath.png");
    _milsLabel->setPixmap(":/Image/RobotInfoMils.png");
    _macLabel->setDirect(RobotInfoLabel::LEFT);
    _buildTimeLabel->setDirect(RobotInfoLabel::LEFT);
    _collisionLabel->setDirect(RobotInfoLabel::LEFT);
    _curLabel->setDirect(RobotInfoLabel::LEFT);
    _imuLabel->setDirect(RobotInfoLabel::LEFT);
    _mapFileLabel->setDirect(RobotInfoLabel::LEFT);
    _milsLabel->setDirect(RobotInfoLabel::LEFT);
    _relayLabel = new RobotInfoLabel(this);
    _skipLabel = new RobotInfoLabel(this);
    _sonarLabel = new RobotInfoLabel(this);
    _volLabel = new RobotInfoLabel(this);
    _wheelLabel = new RobotInfoLabel(this);
    _pointLabel = new RobotInfoLabel(this);
    _relayLabel->setName("继电器");
    _skipLabel->setName("环境匹配度");
    _sonarLabel->setName("声纳");
    _volLabel->setName("电压");
    _wheelLabel->setName("轮子");
    _pointLabel->setName("坐标");
    _relayLabel->setPixmap(":/Image/RobotInfoRelay.png");
    _skipLabel->setPixmap(":/Image/RobotInfoSkip.png");
    _sonarLabel->setPixmap(":/Image/RobotInfoSonar.png");
    _volLabel->setPixmap(":/Image/RobotInfoVol.png");
    _wheelLabel->setPixmap(":/Image/RobotInfoWheel.png");
    _pointLabel->setPixmap(":/Image/RobotInfoPoint.png");
    _relayLabel->setDirect(RobotInfoLabel::RIGHT);
    _skipLabel->setDirect(RobotInfoLabel::RIGHT);
    _sonarLabel->setDirect(RobotInfoLabel::RIGHT);
    _volLabel->setDirect(RobotInfoLabel::RIGHT);
    _wheelLabel->setDirect(RobotInfoLabel::RIGHT);
    _pointLabel->setDirect(RobotInfoLabel::RIGHT);
    _layout00=new QVBoxLayout();
    _layout00->setContentsMargins(0,0,0,0);
    _layout00->setSpacing(0);
    _layout00->addWidget(_macLabel);
    _layout00->addWidget(_buildTimeLabel);
    _layout00->addWidget(_collisionLabel);
    _layout00->addWidget(_curLabel);
    _layout00->addWidget(_imuLabel);
    _layout00->addWidget(_mapFileLabel);
    _layout00->addWidget(_milsLabel);
    _layout01=new QVBoxLayout();
    _layout01->setContentsMargins(0,0,0,0);
    _layout01->setSpacing(0);
    _layout01->addWidget(_relayLabel);
    _layout01->addWidget(_skipLabel);
    _layout01->addWidget(_sonarLabel);
    _layout01->addWidget(_volLabel);
    _layout01->addWidget(_wheelLabel);
    _layout01->addWidget(_pointLabel);
    _layout=new QHBoxLayout(this);
    _layout->setContentsMargins(40,30,30,30);
    _layout->setSpacing(400);
    _layout->addLayout(_layout00);
    _layout->addLayout(_layout01);
    setLayout(_layout);
    connect(ros,SIGNAL(robotStateChangedSignal()),this,SLOT(changedSlot()));
}
void MRobotInfoPage::changedSlot(){
    _macLabel->setValueColor(_color.common);
    _macLabel->setValue(macAddress);
    _buildTimeLabel->setValueColor(_color.common);
    _buildTimeLabel->setValue(buildDatatime);
    if(ros->robotStateValue.collision){
        _collisionLabel->setValueColor(_color.warn);
        _collisionLabel->setValue("异常");
    }else{
        _collisionLabel->setValueColor(_color.health);
        _collisionLabel->setValue("正常");
    }
    _curLabel->setValueColor(_color.common);
    _curLabel->setValue(QString::number(ros->robotStateValue.cur,'f',3));
    if(ros->robotStateValue.imu){
        _imuLabel->setValueColor(_color.warn);
        _imuLabel->setValue("异常");
    }else{
        _imuLabel->setValueColor(_color.health);
        _imuLabel->setValue("正常");
    }
    _mapFileLabel->setValueColor(_color.common);
    _mapFileLabel->setValue(ros->robotStateValue.mapFile);
    _milsLabel->setValueColor(_color.common);
    _milsLabel->setValue(QString::number(ros->robotStateValue.mils));
    _relayLabel->setValueColor(_color.common);
    _relayLabel->setValue(QString::number(ros->robotStateValue.relay));
    _skipLabel->setValueColor(_color.common);
    _skipLabel->setValue(QString::number(ros->robotStateValue.skip));
    if(ros->robotStateValue.sonar){
        _sonarLabel->setValueColor(_color.warn);
        _sonarLabel->setValue("异常");
    }else{
        _sonarLabel->setValueColor(_color.health);
        _sonarLabel->setValue("正常");
    }
    _volLabel->setValueColor(_color.common);
    _volLabel->setValue(QString::number(ros->robotStateValue.vol,'f',3));
    if(ros->robotStateValue.wheel){
        _wheelLabel->setValueColor(_color.warn);
        _wheelLabel->setValue("异常");
    }else{
        _wheelLabel->setValueColor(_color.health);
        _wheelLabel->setValue("正常");
    }
    QString x=QString::number(ros->robotStateValue.point.x,'f',3);
    QString y=QString::number(ros->robotStateValue.point.y,'f',3);
    QString a=QString::number(ros->robotStateValue.point.angle,'f',3);
    _pointLabel->setValueColor(_color.common);
    _pointLabel->setValue(QString("[%1,%2,%3]").arg(x).arg(y).arg(a));
}
