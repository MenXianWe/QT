#ifndef DATAAREA_H
#define DATAAREA_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include "udpsocket.h"
#include "tcpsocket.h"
#include "daelalldata.h"
namespace Ui {
class DataArea;
}

class DataArea : public QWidget
{
    Q_OBJECT

public:
    explicit DataArea(QWidget *parent = nullptr);
    ~DataArea();
    enum SockeType{RadioType,TcpSocketType};
signals:
    void sendUdpDataToDeal(QString data,int type);//将广播命令发送去处理
    void sendTcpDataToDeal(QString data,int type);//发送Tcp命令到处理
    void toImgAre(QPixmap);//将图片信息传出去
    void toAlarmImgAre(QPixmap);//将报警图片信息传出去
private:
    void init();
    void orderInit();//初始化命令
    void getCurrentTime();
private slots:
    //udp
    void showSendData(QString sendDataShow);
    void udpDataToShow(QString dataShow);
    void setDeviceIPToComboBox(QString ipStr,int num);
    void on_udpRadioPushButton_clicked();
    //tcp
    void connectStatus(QString str);
    void on_connectPushButton_clicked();
    void showOpenArlamData(QString opendata);
    void sendImgOrder();
    //button
    void on_clearPushButton_clicked();
    void on_openAlarmPushButton_clicked();
    void on_closeSerivePushButton_clicked();
    //test
    //void showImg(QPixmap);
private:
    Ui::DataArea *ui;
    //时间
    QDateTime current_date_time;
    QString current_date;
    QTimer *imgTimer;
    // 命令
    QString udpRadioData;//广播命令
    QString openServeData;//开启服务命令
    QString closeServeData;//关闭服务命令
    QString getImgData;//前端图像命令
    //udp
    UdpSocket *udpRadio;
    QString comboBoxIpStr="";

    //数据处理
    DaelAllData *dealData;

    //TCP
    TcpSocket *tcpSocket;
};

#endif // DATAAREA_H
