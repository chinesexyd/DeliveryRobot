#include "a_root.h"

/*=============全局=============*/
HMI* hmi=new HMI();
Config* config=new Config();
/*==========================*/
QString HOME_PATH=QDir::homePath();
QJsonObject pathObject=config->getValue("path").toObject();
QString NOTE_PATH=HOME_PATH+pathObject.value("notePath").toString();
QString PROVINCES_PATH=HOME_PATH+pathObject.value("provincesPath").toString();
QString MAP_PATH=HOME_PATH+pathObject.value("mapPath").toString();
QString BAIDUVOICE_PATH=HOME_PATH+pathObject.value("baiduVoicePath").toString();
QString VIDEO_PATH=HOME_PATH+pathObject.value("videoPath").toString();
QString IMAGE_PATH=HOME_PATH+pathObject.value("imagePath").toString();
QString ROS_LOG_PATH=HOME_PATH+"/jld_profile/log";
QString UPPERSOFT_PATH=HOME_PATH+pathObject.value("upperSoftPath").toString();
QString MIDDLESOFT_PATH=HOME_PATH+pathObject.value("middleSoftPath").toString();
QString LOWERSOFT_PATH=HOME_PATH+pathObject.value("lowerSoftPath").toString();
QString ROBOTINFO_PATH=HOME_PATH+pathObject.value("robotInfoPath").toString();
/*============颜色渐变器==============*/
ColorChange::ColorChange(){}
ColorChange::~ColorChange(){}
void ColorChange::setStartColor(QString startColor){
    std::string hexCode = startColor.toStdString();
    if(hexCode.at(0)=='#'){
        hexCode = hexCode.erase(0,1);
    }
    std::istringstream(hexCode.substr(0,2)) >> std::hex >> _startColor.r;
    std::istringstream(hexCode.substr(2,2)) >> std::hex >> _startColor.g;
    std::istringstream(hexCode.substr(4,2)) >> std::hex >> _startColor.b;
}
void ColorChange::setStartColor(int r,int g,int b){
    _startColor.r = r;
    _startColor.g = g;
    _startColor.b = b;
}
void ColorChange::setStartColor(RGB_COLOR startColor){
    _startColor = startColor;
}
void ColorChange::setEndColor(QString endColor){
    std::string hexCode = endColor.toStdString();
    if(hexCode.at(0)=='#'){
        hexCode = hexCode.erase(0,1);
    }
    std::istringstream(hexCode.substr(0,2)) >> std::hex >> _endColor.r;
    std::istringstream(hexCode.substr(2,2)) >> std::hex >> _endColor.g;
    std::istringstream(hexCode.substr(4,2)) >> std::hex >> _endColor.b;
}
void ColorChange::setEndColor(int r,int g,int b){
    _endColor.r = r;
    _endColor.g = g;
    _endColor.b = b;
}
void ColorChange::setEndColor(RGB_COLOR endColor){
    _endColor = endColor;
}
void ColorChange::setMinNum(int min){
    _min = min;
}
void ColorChange::setMaxNum(int max){
    _max = max;
}
void ColorChange::setRange(int min,int max){
    _min = min;
    _max = max;
}
RGB_COLOR ColorChange::getColor(int currentNum){
    int r = (currentNum-_min)*(_endColor.r-_startColor.r)/(_max-_min)+_startColor.r;
    int g = (currentNum-_min)*(_endColor.g-_startColor.g)/(_max-_min)+_startColor.g;
    int b = (currentNum-_min)*(_endColor.b-_startColor.b)/(_max-_min)+_startColor.b;
    RGB_COLOR color;
    color.r=r;
    color.g=g;
    color.b=b;
    return color;
}


