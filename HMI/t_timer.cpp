#include "t_timer.h"

Timer::Timer(QObject *parent):QObject(parent){}
void Timer::setTimerParent(QWidget* widget){
    _timer100 = new QTimer(widget);
    _timer100->start(100);
    connect(_timer100,SIGNAL(timeout()),this,SLOT(timer100TimeOutSlot()));
}
void Timer::timer100TimeOutSlot(){
    emit timer100TimeoutSignal();
    _count+=1;
    _count=_count%20;
    if(_count==0){
        emit timer1000TimeoutSignal();
    }
}
