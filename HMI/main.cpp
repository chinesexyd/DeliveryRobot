#include "p_mainwindow.h"

int main(int argc, char *argv[]){
    QtSingleApplication a("DeliveryRobot",argc, argv);
    if(a.isRunning()){
        note->writeLine(Note::M_HMI,"打开错误","已经开启了一个软件，当前为重复开启");
        a.sendMessage("raise_window_noop",2000);
        return EXIT_SUCCESS;
    }
    MainWindow w;
    a.setActivationWindow(&w,1);
    w.setFixedSize(1280,480);
    w.showFullScreen();
    return a.exec();
}


