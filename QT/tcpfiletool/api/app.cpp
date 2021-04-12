#include "app.h"
#include "head.h"

QString App::ConfigFile = "config.ini";

QString App::ServerIP = "127.0.0.1";
int App::ServerPort = 6005;
QString App::SendFile = "qt_app.tar.gz";
int App::ListenPort = 6005;
bool App::AutoListen = true;

void App::readConfig()
{
    if (!checkConfig()) {
        return;
    }

    QSettings set(App::ConfigFile, QSettings::IniFormat);

    set.beginGroup("BaseConfig");
    App::ServerIP = set.value("ServerIP").toString();
    App::ServerPort = set.value("ServerPort").toInt();
    App::SendFile = set.value("SendFile").toString();
    App::ListenPort = set.value("ListenPort").toInt();
    App::AutoListen = set.value("AutoListen").toBool();
    set.endGroup();
}

void App::writeConfig()
{
    QSettings set(App::ConfigFile, QSettings::IniFormat);

    set.beginGroup("BaseConfig");
    set.setValue("ServerIP", App::ServerIP);
    set.setValue("ServerPort", App::ServerPort);
    set.setValue("SendFile", App::SendFile);
    set.setValue("ListenPort", App::ListenPort);
    set.setValue("AutoListen", App::AutoListen);
    set.endGroup();
}

void App::newConfig()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#endif
    writeConfig();
}

bool App::checkConfig()
{
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(App::ConfigFile);
    if (file.size() == 0) {
        newConfig();
        return false;
    }

    //如果配置文件不完整,则以初始值继续运行,并生成配置文件
    if (file.open(QFile::ReadOnly)) {
        bool ok = true;

        while (!file.atEnd()) {
            QString line = file.readLine();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            QStringList list = line.split("=");

            if (list.count() == 2) {
                if (list.at(1) == "") {
                    ok = false;
                    break;
                }
            }
        }

        if (!ok) {
            newConfig();
            return false;
        }
    } else {
        newConfig();
        return false;
    }

    return true;
}
