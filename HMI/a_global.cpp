#include "a_global.h"

/*===================== 全局 =====================*/
Note* note=new Note();
Timer* timer=new Timer();
/*===================== 全局函数 =====================*/
QString GetMacAddress(){
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++){
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack)){
            strMacAddr = nets[i].hardwareAddress();
            strMacAddr.replace(":","");
            break;
        }
    }
    return strMacAddr;
}
XYA_POINT GetXYAPointFromXYZWPoint(XYZW_POINT point){
    double yaw = atan2(2 * (point.w * point.z), 1 - 2 * (point.z * point.z));
    double angle = yaw/M_PI*180;
    XYA_POINT xyaPoint;
    xyaPoint.x = point.x;
    xyaPoint.y = point.y;
    xyaPoint.angle = angle;
    return xyaPoint;
}
XYZW_POINT GetXYZWPointFromXYAPoint(XYA_POINT point){
    double angle = point.angle;
    double yaw = angle/180.0*M_PI;
    double cy = cos(yaw*0.5);
    double sy = sin(yaw*0.5);
    double cp = 1;
    double sp = 0;
    double cr = 1;
    double sr = 0;
    XYZW_POINT xyzwPoint;
    xyzwPoint.w = cy*cp*cr+sy*sp*sr;
    xyzwPoint.z = sy*cp*cr-cy*sp*sr;
    xyzwPoint.x = point.x;
    xyzwPoint.y = point.y;
    return xyzwPoint;
}
void Sleep(quint16 msec){
    QTime reachTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<reachTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}
QString GetTimeFromSeconds(quint16 seconds){
    quint8 minutes = floor(seconds/60.0);
    quint8 second = seconds%60;
    return QString("%1:%2").arg(minutes,2,10,QLatin1Char('0')).arg(second,2,10,QLatin1Char('0'));
}
const QString BuildDateTime(){
    QString dateTime;
    dateTime += __DATE__;
    dateTime += " ";
    dateTime += __TIME__;
    return dateTime;
}
