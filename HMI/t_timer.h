#ifndef TIMER_H
#define TIMER_H

#include "HMI/a_root.h"

class Timer:public QObject {
    Q_OBJECT
public:
  explicit Timer(QObject *parent = nullptr);
  void setTimerParent(QWidget* widget);
private:
   QTimer* _timer100;
   quint8 _count=0;
public slots:
   void timer100TimeOutSlot();
signals:
   void timer100TimeoutSignal();
   void timer1000TimeoutSignal();
};

#endif // TIMER_H
