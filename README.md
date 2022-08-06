# 众多模块
### 1.相机模块
专门负责调用ROS进行拍照并返回照片路径，用于控制录制视频。
### 2.配置模块
配合JSON配置文件，对配置文件进行读写操作，并对旧的配置文件进行更新。
### 3.信号模块
准备了一些信号，用于在逻辑部分通过信号来控制界面部分。或者界面部分通过信号将数据传递到逻辑部分（软键盘）
### 4.http模块
将与服务器通过HTTP沟通的方法全部封装在该模块中，在逻辑部分进行调用，服务器返回消息通过信号的方式进行传输到程序中
### 5.mqServer模块
建立一个TCP Server端，监听客户端的连接，且与专门用于监听RABBITMQ消息的客户端进行数据交互。
通过调用系统命令控制客户端程序进程的开始和结束，在刚启动程序，服务端开始监听时，启动客户端，若客户端连续20s无数据返回，判定为连接MQ失败，将界面的M图标背景色设置为红色。60s仍旧没有数据返回，则杀死客户端进行，再重新启动客户端进程，直至客户端一直持续稳定的发送数据至服务端。
### 6.日志模块
用于记录程序的一些交互信息，主要是与服务器通过HTTP方式交互，以及通过RABBITMQ交互，和与ROS交互的信息，有一小部分为程序自身的日志信息。
每次写入日志时检查日期是否变更，若日期变更，则生成新的日志文件，删除一个月之前的旧日志文件。
自动补入消息的日期。格式：yyyy-MM-dd hh:mm:ss:zzz [消息来源] (类型) : 内容\n
### 7.解析省市级联JSON文件的模块
封装了从省市级联JSON文件中获取省市县信息的方法
### 8.ROS模块
通过TCP Client连接到ROS提供的TCP Server端，与ROS进行数据交互，封装了一系列与ROS交互的方法，并将ROS的返回信息封装为信号供程序使用。
### 9.定时器模块
只有一个100ms的定时器，通过计数的方式，制造出1s的定时器信号，发射100ms和1s的定时信号。
### 10.tts模块
通过http方式调用百度文字转语音，并将语音保存为wav格式的音频文件。
### 11.声音模块
用于控制播放音频，封装了串联起来多个广告语音频文件进行播放的功能和播放单个文件的功能，可以控制播放的循环次数。
# UI
### 管理员设置界面
##### 广告设置页面 
更新广告语，广告语试听，生成广告语，应用广告语。
##### 小区配置页面
设置小区所在省市，选定仓储名称，显示已绑定小区和仓库，显示本机的唯一识别码（MAC地址）
##### 相机设置页面
拍照测试相机是否可用，设置本机保存的图片数量，保存的视频数量，每一小段视频录制的时长
##### 订单页面
显示需要处理的订单信息界面，以及正在送货的订单信息。
##### 机器人信息页面
显示机器人的各种状态信息，数据来源于ROS
##### 声音设置页面
设置声音的人物，语速，语调，音量，并提供试听功能。试听之后觉得可以，点击应用即可生成所有需要的音频文件。
##### 系统设置页面
初始化、退出界面、重启中位机、关机。是否开启低电量提示音，控制充电，控制开启关闭车门。显示上位机、中位机、下位机的软件服务器端和本机端的版本信息，并均提供升级按钮。
### 用户可见页面
##### 常驻眨眼页面
管理员登录按钮，威信小程序入口二维码，简单的眨眼动效
##### 管理员登录页面
输入管理员密码，即可进入管理员设置界面部分
##### 开机显示页面
显示连接RABBITMQ，网络连通，与ROS的连接状态，三者都连接到则可以初始化。管理员按钮可以进入管理员登录页面
##### 显示订单页面
当接收到订单消息时切换为此页面，显示订单中的商品信息，价格信息等，有开始送货，放弃订单，打开车门，关闭车门按钮
##### 用户输入验证码界面
用户输入通过短信或通过电话得知的取货码，自动打开车门并进入到取货界面。
##### 用户取货界面
显示订单的商品信息，价格信息等，确认收获按钮，客服电话。点击确认收货立即关闭车门返航，如果未点击确认收获，则2分钟后自动确认收货，关闭车门返航。
##### 状态栏
显示 公司简称（吉立达），小车当前的状态信息，警告信息，HTTP，RABBITMQ，ROS的连接状态图标，时钟，电池电量信息。
### 自定义控件
对QT原始控件的简单改造，以适用于界面UI，不做过多介绍。
# 程序系统逻辑
root将程序几乎所有用到的库到导入进来，不用在每个文件中都做导入，提供基础的功能，导入日志模块，配置模块，界面信号模块，以便程序全局调用;有些模块导入root头文件，逻辑部分导入部分模块并实例化，界面部分导入逻辑部分进行调用。这是自下而上的顺序，方便导入实例化调用。
自上而下则通过界面信号模块进行信号通知，作出响应。
# 文件结构
+ DeliveryRobot
    + DeliveryRobot_Debug
        + DeliveryRobot
        + ...
    + DeliveryRobot_Release
        + DeliveryRobot
        + ...
    + DeliveryRobot
        + Camera
            + Image
                + ...
            + Video
                + ...
        + Config
            + address_data.json
            + isTest.txt
            + robot.json
        + Download
            + LowerSoft
            + MiddleSoft
            + UpperSoft
        + HMI
            + a_const.h
            + a_global.cpp
            + a_global.h
            + a_logic.cpp
            + a_logic.h
            + a_root.cpp
            + a_root.h
            + j_button.cpp
            + j_button.h
            + j_checkbox.cpp
            + j_checkbox.h
            + j_combobox.cpp
            + j_combobox.h
            + j_frame.cpp
            + j_frame.h
            + j_label.cpp
            + j_label.h
            + j_line.cpp
            + j_line.h
            + j_slider.cpp
            + j_slider.h
            + main.cpp
            + p_blinkpage.cpp
            + p_blinkpage.h
            + p_mainwindow.cpp
            + p_mainwindow.h
            + p_managerloginpage.cpp
            + p_managerloginpage.h
            + p_managerpage.cpp
            + p_managerpage.h
            + p_poweronpage.cpp
            + p_poweronpage.h
            + p_showorderpage.cpp
            + p_showorderpage.h
            + p_statusbar.cpp
            + p_statusbar.h
            + p_userloginpage.cpp
            + p_userloginpage.h
            + p_userreceivepage.cpp
            + p_userreceivepage.h
            + p_z_advertpage.cpp
            + p_z_advertpage.h
            + p_z_areapage.cpp
            + p_z_areapage.h
            + p_z_camerapage.cpp
            + p_z_camerapage.h
            + p_z_orderpage02.cpp
            + p_z_orderpage02.h
            + p_z_orderpage03.cpp
            + p_z_orderpage03.h
            + p_z_orderpage04.cpp
            + p_z_orderpage04.h
            + p_z_robotinfopage.cpp
            + p_z_robotinfopage.h
            + p_z_soundpage.cpp
            + p_z_soundpage.h
            + p_z_systempage.cpp
            + p_z_systempage.h
            + t_camera.cpp
            + t_camera.h
            + t_config.cpp
            + t_config.h
            + t_hmi.cpp
            + t_hmi.h
            + t_http.cpp
            + t_http.h
            + t_mqserver.cpp
            + t_mqserver.h
            + t_notewriter.cpp
            + t_noterwriter.h
            + t_parsejson.cpp
            + t_parsejson.h
            + t_ros.cpp
            + t_ros.h
            + t_timer.cpp
            + t_timer.h
            + t_tts.cpp
            + t_tts.hh
            + t_voice.cpp
            + t_voice.h
        + Image
            + Image.qrc
            + background.png
            + ...
        + Include
            + qtsingleapplication
                + ...
        + Music
            + BaiduVoice
                + 0.wav
                + 1.wav
                + 2.wav
                + ...
                + codeFalse.wav
                + codeTrue.wav
                + dontTouchMe.wav
                + elevatorGiveWay.wav
                + elevatorGiveWayIn.wav
                + elevatorGiveWayOut.wav
                + giveWay.wav
                + init.wav
                + noPower.wav
                + receivedGoods.wav
                + retentionGoods.wav
                + returnGoods.wav
                + test.wav
                + xxx.wav
            + button.wav
            + change.wav
            + check.wav
            + number.wav
            + slider.wav
            + Music.qrc
        + Note
            + 2022-02-02.txt
            + 2022-02-03.txt
            + 2022-02-04.txt
            + ...
        + Qss
            + Qss.qrc
            + style.qss
    + DeliveryRobot.pro
    + DeliveryRobot.pro.user 
    + README.md 
# DeliveryRobot
