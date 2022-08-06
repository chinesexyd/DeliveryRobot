#ifndef GLOBAL_H
#define GLOBAL_H

#ifndef M_PI
#define M_PI = 3.14159265358979323846
#endif

#include "HMI/c_commonwidget.h"
#include "HMI/t_timer.h"
#include "HMI/t_note.h"
/*===================== 全局 =====================*/
extern Note* note;
extern Timer* timer;
/*===================== 全局函数 =====================*/
extern QString GetMacAddress();
extern XYA_POINT GetXYAPointFromXYZWPoint(XYZW_POINT point);
extern XYZW_POINT GetXYZWPointFromXYAPoint(XYA_POINT point);
extern void Sleep(quint16 msec);
extern QString GetTimeFromSeconds(quint16 seconds);
extern const QString BuildDateTime();
//开机获取macAddress;
const QString macAddress = GetMacAddress();
#define buildDatatime BuildDateTime()

#endif // GLOBAL_H
