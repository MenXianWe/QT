#ifndef APP_H
#define APP_H

#include <QStringList>

class App
{
public:
    static QString ConfigFile;      //配置文件文件路径及名称

    static QString ServerIP;        //服务器IP
    static int ServerPort;          //服务器端口
    static QString SendFile;        //发送文件
    static int ListenPort;          //监听端口
    static bool AutoListen;         //自动监听

    static void readConfig();       //读取配置文件,在main函数最开始加载程序载入
    static void writeConfig();      //写入配置文件,在更改配置文件程序关闭时调用
    static void newConfig();        //以初始值新建配置文件
    static bool checkConfig();      //校验配置文件

};

#endif // APP_H
