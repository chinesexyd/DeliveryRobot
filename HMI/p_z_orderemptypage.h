#ifndef MORDERPAGE04_H
#define MORDERPAGE04_H

#include "HMI/a_logic.h"

/*@@@@@@@@@@@正常@@@@@@@@@@*/
class MOrderEmptyPage : public QFrame{
    Q_OBJECT
public:
    explicit MOrderEmptyPage(QWidget *parent = nullptr);
private:
    PixmapLabel* _pixmapLabel;
    QLabel* _noOrderLabel;
};
#endif // P_ORDERPAGE03_H
