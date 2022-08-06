#ifndef COMMONWIDGET_H
#define COMMONWIDGET_H

#include "a_root.h"
/*@@@@@@@@@@@常规背景@@@@@@@@@*/
class BackgroundFrame:public QWidget{
    Q_OBJECT
public:
    explicit BackgroundFrame(QWidget* parent=nullptr);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QPixmap _pixmap;
};
/*@@@@@@@@@@@管理员背景@@@@@@@@@*/
class ManagerBackgroundFrame:public QWidget{
    Q_OBJECT
public:
    explicit ManagerBackgroundFrame(QWidget* parent=nullptr);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QPixmap _pixmap;
    QPixmap _leftPixmap;
    QPixmap _lightPixmap;
};
/*@@@@@@@@@@@电池图标@@@@@@@@@*/
class PowerFrame:public QWidget{
    Q_OBJECT
public:
    explicit PowerFrame(QWidget* parent=nullptr);
    ~PowerFrame();
    void setPower(int power);
    void setCharge(bool isCharging);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    ColorChange* colorChange;
    QString _startColor="#FF0000";
    QString _endColor="#00FF00";
    QString _borderColor = "#818181";
    QString _charingColor = "#FFFFFF";
    int _min=0;
    int _max=100;
    int _currentNum;
    RGB_COLOR _currentColor;
    bool _isCharging = false;
};
/*@@@@@@@@@@@信号图标@@@@@@@@@*/
class SignalFrame:public QWidget{
    Q_OBJECT
public:
    explicit SignalFrame(QWidget* parent=nullptr);
    void setSignal(quint8 CSQ);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    quint8 _signalValue;
};
/*@@@@@@@@@@@设置图片@@@@@@@@@*/
class PixmapLabel:public QWidget{
    Q_OBJECT
public:
    explicit PixmapLabel(QWidget* parent=nullptr);
    void setPixmap(QString path);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QPixmap _pixmap;
};
/*@@@@@@@@@@@商品条目@@@@@@@@@*/
class GoodsItemFrame:public QWidget{
    Q_OBJECT
public:
    explicit GoodsItemFrame(QWidget* parent=nullptr);
    void setGoodsInfo(HMI::GOODS_INFO goodsInfo);
protected:
    void paintEvent(QPaintEvent*);
public slots:
    void finishedSlot(QNetworkReply*);
private:
    QNetworkAccessManager* _manager;
    HMI::GOODS_INFO _goodsInfo;
    QPixmap _pixmap;
    QColor _backgroundColor;
};
/*@@@@@@@@@@@开关条目@@@@@@@@@*/
class SwitchItemFrame:public QWidget{
    Q_OBJECT
public:
    explicit SwitchItemFrame(QWidget* parent=nullptr);
    void setName(QString name);
    void setTip(QString tip);
    void setPixmap(QString path);
    void setBool(bool isTrue);
protected:
    void resizeEvent(QResizeEvent *event)override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
signals:
    void changedSignal(bool);
private:
    QPixmap _pixmap;
    QColor _backgroundColor;
    QString _name;
    QString _tip;
    bool _isTrue;
    QRect _switchRect;
    QRect _ellipseRect;
    QColor _switchColor;
    QSound* _sound;
};
/*@@@@@@@@@@@滑动窗口@@@@@@@@@*/
class SlideFrame:public QWidget{
    Q_OBJECT
public:
    explicit SlideFrame(QWidget* parent=nullptr);
    void addItem(QWidget* item);
    void clearItems();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    void keep(double y);
private:
    QWidget* _centralFrame;
    QList<QWidget*> _itemList;
    QVBoxLayout* _centralLayout;
    quint32 _centralHeight=0;
    QPoint _startPos;
    double _centralY;
};
/*@@@@@@@@@@@键盘按键@@@@@@@@@*/
class KeyboardFrame;
class KeyboardButton:public QPushButton{
    Q_OBJECT
public:
    explicit KeyboardButton(QWidget* parent=nullptr);
    void setNumber(quint8 number);
protected:
    void mousePressEvent(QMouseEvent*)override;
    void mouseReleaseEvent(QMouseEvent*)override;
    void paintEvent(QPaintEvent *) override;
public slots:
    void clickedSlot();
private:
    QSound* _sound;
    QString _number;
    bool _isDrawPixmap=false;
    QPixmap _pixmap;
    QColor _backgroundColor;
    KeyboardFrame* _parent;
};
/*@@@@@@@@@@@键盘@@@@@@@@@*/
class KeyboardFrame:public QWidget{
    Q_OBJECT
public:
    explicit KeyboardFrame(QWidget* parent=nullptr);
signals:
    void sendKeySignal(QString key);
private:
    QGridLayout* _layout;
    KeyboardButton* _key0;
    KeyboardButton* _key1;
    KeyboardButton* _key2;
    KeyboardButton* _key3;
    KeyboardButton* _key4;
    KeyboardButton* _key5;
    KeyboardButton* _key6;
    KeyboardButton* _key7;
    KeyboardButton* _key8;
    KeyboardButton* _key9;
    KeyboardButton* _key10;
    KeyboardButton* _key11;
};
/*@@@@@@@@@@@键盘输入框@@@@@@@@@@@*/
class KeyboardLine:public QWidget{
    Q_OBJECT
public:
    explicit KeyboardLine(QWidget* parent=nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
public slots:
    void receiveKeySlot(QString key);
signals:
    void sendLineText(QString);
private:
    QLineEdit* _line;
    QHBoxLayout* _layout;
    QString _text;
};
/*@@@@@@@@@@@菜单按钮@@@@@@@@@*/
class MenuButton:public QPushButton{
    Q_OBJECT
public:
    explicit MenuButton(QWidget* parent=nullptr);
    void setUncheckedPixmap(QString path);
    void setCheckedPixmap(QString path);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QPixmap _uncheckedPixmap;
    QPixmap _checkedPixmap;
    QSound* _sound;
};
/*@@@@@@@@@@@动画标签@@@@@@@@@@@*/
class AnimationLabel:public QLabel{
    Q_OBJECT
public:
    explicit AnimationLabel(QWidget *parent = nullptr);
    ~AnimationLabel();
    void setPixmapPath(QString path);
protected:
    void paintEvent(QPaintEvent *)override;
protected slots:
    void animationValueChangedSlot(QVariant);
private:
    QList<QPixmap> _pixAninormalList;
    QPixmap _pixNormal;
    QPropertyAnimation *_aninormal;
    int _pixmapIndex;
};
/*@@@@@@@@@@@返回按钮@@@@@@@@@@@*/
class ReturnButton : public QPushButton{
    Q_OBJECT
public:
    explicit ReturnButton(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent*);
private:
    QPixmap _pixmap;
    QSound* _sound;
};
/*@@@@@@@@@@@管理员按钮@@@@@@@@@@@*/
class ManagerButton:public QPushButton{
    Q_OBJECT
public:
    explicit ManagerButton(QWidget* parent=nullptr);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QPixmap _pixmap;
    QSound* _sound;
};
/*@@@@@@@@@@@大按钮@@@@@@@@@@@*/
class PowerButton:public QPushButton{
    Q_OBJECT
public:
    explicit PowerButton(QWidget* parent=nullptr);
    void setText(QString text);
    void setPixmap(QString path);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QString _text;
    QPixmap _pixmap;
    QSound* _sound;
};
/*@@@@@@@@@@@通用按钮@@@@@@@@@@@*/
class CommonButton:public QPushButton{
    Q_OBJECT
public:
    explicit CommonButton(QWidget* parent=nullptr);
    void setText(QString text);
    void setPixmap(QString path);
protected:
    void mousePressEvent(QMouseEvent*)override;
    void mouseReleaseEvent(QMouseEvent*)override;
    void paintEvent(QPaintEvent *) override;
private:
    QString _text;
    QPixmap _pixmap;
    QSound* _sound;
    QColor _backgroundColor;
};
/*@@@@@@@@@@@@二维码@@@@@@@@@@*/
class CodeFrame:public QWidget{
    Q_OBJECT
public:
    explicit CodeFrame(QWidget* parent=nullptr);
    ~CodeFrame();
    void setCodeText(QString codeText);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QRcode* _qrcode;
    bool _isSetCodeText=false;
};
/*@@@@@@@@@@@@提示栏@@@@@@@@@@@@@*/
class TipLabel:public QLabel{
    Q_OBJECT
public:
    enum TIP_TYPE{
        TT_ERROR,
        TT_NORMAL
    };
    struct TIP_INFO{
        TIP_TYPE type;
        int code;
        QString detail;
    };
public:
    explicit TipLabel(QWidget* parent=nullptr);
    void showTip(TIP_INFO info);
};
/*@@@@@@@@@@@订单金额@@@@@@@@@@@*/
class OrderMoneyFrame:public QWidget{
    Q_OBJECT
public:
    explicit OrderMoneyFrame(QWidget* parent=nullptr);
    void setAmount(double amount);
    void setConcessionary(double concessionary);
    void setPayment(double payment);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QFormLayout* _layout;
    QLabel* _amountLabel;
    QLabel* _amountLine;
    QLabel* _concessionaryLabel;
    QLabel* _concessionaryLine;
    QLabel* _paymentLabel;
    QLabel* _paymentLine;
};
/*@@@@@@@@@@@订单用户信息@@@@@@@@@@@*/
class OrderCustomerInfoFrame:public QWidget{
    Q_OBJECT
public:
    explicit OrderCustomerInfoFrame(QWidget* parent=nullptr);
    void setCustomerName(QString name);
    void setCustomerPhone(QString phone);
    void setCustomerAddress(QString building,QString unit,QString floor,QString room);
    void setOrderTime(QString time);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QFormLayout* _layout00;
    QFormLayout* _layout01;
    QHBoxLayout* _layout;
    PixmapLabel* _customerNameLabel;
    QLabel* _customerNameLine;
    PixmapLabel* _customerTelLabel;
    QLabel* _customerTelLine;
    PixmapLabel* _customerAddressLabel;
    QLabel* _customerAddressLine;
    PixmapLabel* _orderTimeLabel;
    QLabel* _orderTimeLine;
};
/*@@@@@@@@@@@滑动条@@@@@@@@@@@*/
class SlideLineBar:public QWidget{
    Q_OBJECT
public:
    explicit SlideLineBar(QWidget* parent=nullptr);
    void setRange(quint8 min,quint8 max);
    void setPixmap(QString path);
    void setName(QString name);
    void setValue(quint8 value);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    void xToRect(qint32 x);
    void valueToRect(quint8 value);
signals:
    void changedSignal(quint8 value);
private:
    QPixmap _pixmap;
    quint8 _min,_max,_value;
    QString _name;
    QRectF _valueRect;
    QSound* _sound;
    int _unit;
};
/*@@@@@@@@@@@下拉框@@@@@@@@@@@*/
class ChooseCommbo:public QWidget{
    Q_OBJECT
public:
    explicit ChooseCommbo(QWidget* parent=nullptr);
    void setStringList(QStringList list);
    void setPixmap(QString path);
    void setName(QString name);
    void setChoose(QString choose);
    QString getChoose();
protected:
    void paintEvent(QPaintEvent *event) override;
signals:
    void changedSignal(QString);
private:
    QPixmap _pixmap;
    QString _name;
    QMap<QString,QPushButton*> _buttonmap;
    QHBoxLayout* _layout;
    QButtonGroup* _buttonGroup;
    QSound* _sound;
};
/*@@@@@@@@@@@软件更新框@@@@@@@@@*/
class SoftwareLine:public QWidget{
    Q_OBJECT
public:
    struct SOFTWARELINE_INFO{
        QPixmap pixmap;
        QString title;
        QString version;
        QString detail;
    };
public:
    explicit SoftwareLine(QWidget* parent=nullptr);
    void setInfo(SOFTWARELINE_INFO info);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    SOFTWARELINE_INFO _softwareInfo;
};
class SoftwareFrame:public QWidget{
    Q_OBJECT
public:
    struct CLOUD{
        QString versionName;
        qint16 versionCode;
        QString url;
        QString remark;
        qint16 enforced;
        QString savePath;
    };
    enum STATUS_TYPE{
        SW_ERROR,
        SW_DOWNLOADED,
        SW_DOWNLOADING,
        SW_HASNEW
    };
public:
    explicit SoftwareFrame(QWidget* parent=nullptr);
    ~SoftwareFrame();
    void setTip(QString tip);
    void setTitle(QString title);
    void setCloud(CLOUD cloud);
    void setLocalVersion(QString localVersion);
protected:
    void mousePressEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;
private:
    void downloadIsEnable();
public slots:
    void downloadProcessSlot(qint64,qint64);
signals:
    void finishedSignal(QString);
private:
    QLabel* _titleLine;
    SoftwareLine* _cloudLine;
    SoftwareLine* _localLine;
    QLabel* _tipLine;
    PixmapLabel* _statusLabel;
    QHBoxLayout* _layout;
    QVBoxLayout* _layout0;
    QSound* _sound;
    QNetworkAccessManager* _manager;
    QNetworkReply* _reply;
private:
    CLOUD _cloud;
    QString _localVersion;
    double _downloadProcess=0;
    SoftwareLine::SOFTWARELINE_INFO _cloudInfo;
    SoftwareLine::SOFTWARELINE_INFO _localInfo;
    STATUS_TYPE _status=SW_ERROR;
};
/*@@@@@@@@@@@机器信息栏@@@@@@@@@*/
class RobotInfoLabel:public QWidget{
    Q_OBJECT
public:
    enum DIRECT{
        LEFT,
        RIGHT
    };
public:
    explicit RobotInfoLabel(QWidget* parent=nullptr);
    void setDirect(DIRECT direct);
    void setPixmap(QString path);
    void setName(QString name);
    void setValue(QString value);
    void setValueColor(QColor color);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    DIRECT _direct;
    QPixmap _pixmap;
    QString _path,_name,_value;
    QColor _color;
};

#endif // COMMONWIDGET_H
