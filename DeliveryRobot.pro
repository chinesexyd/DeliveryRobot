# 作者：谢亚东
QT += core gui network websockets gui-private multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

#软件图标
RC_ICONS +=DeliveryRobot.ico

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    HMI/a_global.cpp \
    HMI/a_logic.cpp \
    HMI/a_root.cpp \
    HMI/c_commonwidget.cpp \
    HMI/main.cpp \
    HMI/p_blinkpage.cpp \
    HMI/p_mainwindow.cpp \
    HMI/p_managerloginpage.cpp \
    HMI/p_managerpage.cpp \
    HMI/p_poweronpage.cpp \
    HMI/p_showorderpage.cpp \
    HMI/p_statusbar.cpp \
    HMI/p_userloginpage.cpp \
    HMI/p_userreceivepage.cpp \
    HMI/p_z_advertpage.cpp \
    HMI/p_z_camerapage.cpp \
    HMI/p_z_orderemptypage.cpp \
    HMI/p_z_orderinfopage.cpp \
    HMI/p_z_orderretentionpage.cpp \
    HMI/p_z_robotinfopage.cpp \
    HMI/p_z_soundpage.cpp \
    HMI/p_z_systempage.cpp \
    HMI/t_camera.cpp \
    HMI/t_config.cpp \
    HMI/t_hmi.cpp \
    HMI/t_http.cpp \
    HMI/t_mqserver.cpp \
    HMI/t_note.cpp \
    HMI/t_obs.cpp \
    HMI/t_ros.cpp \
    HMI/t_timer.cpp \
    HMI/t_tts.cpp \
    HMI/t_voice.cpp

HEADERS += \
    HMI/a_const.h \
    HMI/a_global.h \
    HMI/a_logic.h \
    HMI/a_root.h \
    HMI/c_commonwidget.h \
    HMI/p_blinkpage.h \
    HMI/p_mainwindow.h \
    HMI/p_managerloginpage.h \
    HMI/p_managerpage.h \
    HMI/p_poweronpage.h \
    HMI/p_showorderpage.h \
    HMI/p_statusbar.h \
    HMI/p_userloginpage.h \
    HMI/p_userreceivepage.h \
    HMI/p_z_advertpage.h \
    HMI/p_z_camerapage.h \
    HMI/p_z_orderemptypage.h \
    HMI/p_z_orderinfopage.h \
    HMI/p_z_orderretentionpage.h \
    HMI/p_z_robotinfopage.h \
    HMI/p_z_soundpage.h \
    HMI/p_z_systempage.h \
    HMI/t_camera.h \
    HMI/t_config.h \
    HMI/t_hmi.h \
    HMI/t_http.h \
    HMI/t_mqserver.h \
    HMI/t_note.h \
    HMI/t_obs.h \
    HMI/t_ros.h \
    HMI/t_timer.h \
    HMI/t_tts.h \
    HMI/t_voice.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
# 导入第三方库
LIBS+= -lyaml-cpp -lev -lqrencode
# 资源文件
RESOURCES += \
    Image/Image.qrc \
    Music/Music.qrc
# 单例
include(./Include/qtsingleapplication/qtsingleapplication.pri)
# 软件名
TARGET=DeliveryRobot
