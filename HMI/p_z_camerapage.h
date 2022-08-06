#ifndef P_CAMERAPAGE_H
#define P_CAMERAPAGE_H

#include "HMI/a_logic.h"

/*@@@@@@@@@@@@@@@@@@@@@*/
class MCameraPage:public QFrame{
    Q_OBJECT
public:
    explicit MCameraPage(QWidget *parent = nullptr);
private:
    void init();
private slots:
    void takePictureSlot();
    void confirmButtonClickedSlot();
private:
    PixmapLabel* _cameraLabel;
    CommonButton* _takePictureButton;
    ChooseCommbo* _pictureNumberCombo;
    ChooseCommbo* _videoNumberCombo;
    ChooseCommbo* _videoIntervalCombo;
    CommonButton* _confirmButton;
    QVBoxLayout* _layout0;
    QGridLayout* _layout;
};

#endif // P_CAMERAPAGE_H
