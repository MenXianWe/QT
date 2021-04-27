#ifndef DAELALLDATA_H
#define DAELALLDATA_H

#include <QObject>
#include <QDomDocument>
#include <QDebug>
#include <QPixmap>
#include <QSize>
#include <QPainter>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

class DaelAllData : public QObject
{
    Q_OBJECT
public:
    explicit DaelAllData(QObject *parent = nullptr);

signals:
    void dealSendDataToTcp(QString sendDataTcp);//传给Tcp处理
    void dealReadDataImgToUi(QPixmap imgPix);//将图片传给Ui
    void dealReadDataAlarmImgToUi(QPixmap imgPix);//将报警图片传给Ui
    void dealSendDataToUdp(QString sendDataUdp);
    void dealReadDataToUi(QString udpData);
    void TcpReadDataToUi(QString tcpData);//原始数据
    //将要发送的数据加上包头再返回ui
    void dealSendDataToUi(QString data);
    void deviceIP(QString ipStr,int num);
    void toSqlArea(QSqlDatabase);
private slots:
    void dealSendData(QString sendData,int type);//处理发送数据
    void dealReadData(QString readData,int len,int type);//将收到数据处理成图片
    void dealReadUdpData(QString readUdpData);
private:
    QString DeviceIP;//设备IP
    QString ipTempStr="";//临时存储ip
    int ipCount=0;//ip个数
    QSqlDatabase db; //数据库，用于打开一个数据库
    QSqlQuery *sql_q = NULL;        //SQL数据库操作类，用于操作数据库

};

#endif // DAELALLDATA_H
