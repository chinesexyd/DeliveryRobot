#ifndef MSYSTEMPAGE_H
#define MSYSTEMPAGE_H

#include "HMI/a_logic.h"

/*@@@@@@@@@@@@@@@@@@@@@*/
class MAdvertPage : public QFrame{
    Q_OBJECT
public:
    explicit MAdvertPage(QWidget *parent = nullptr);
private:
    void initAdvertList();
public slots:
    void advertListSlot(QStringList);
    void elevatorSwitchChangedSlot(bool);
    void testAdSwitchChangedSlot(bool);
private:
    QGridLayout* _layout;
    SlideFrame* _checkBoxSlide;
    CommonButton* _updateADButton;
    SwitchItemFrame* _elevatorSwitchItem;
    SwitchItemFrame* _testAdSwitchItem;
    QTextEdit* _advertText;
    bool _isUpdateAdButtonClick=false;
    QStringList __adverStringList;
    quint8 __adverCount;
};

#endif // MSYSTEMPAGE_H
