#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "transition.h"
#include "qextserialport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void analisis(QStringList checkDataList); //解析函数，将命令翻译成文字
    void sendData();
    void sendData(QString data);
    QString chexkSum(QString s);
    void openLightF();
    void closeLightF();
    void portSetting(QextSerialPort *portCom);
    void showDataCh(QByteArray compeletData);
private:
    Ui::MainWindow *ui;
    QextSerialPort *myCom;
    //QextSerialPort *myCom0;//ttys3
    QextSerialPort *myCom1;//总线1
    QextSerialPort *myCom2;//总线2
    QTimer *timerRead;
    QTimer *timerSend;
    QByteArray tempData;
    int resultState;
    int openLightState;//全开or开
    int closeLightState;//全关or关
private slots:
    void readMyCom();

    void on_openComBtn_clicked();
    void on_sendDataBtn_clicked();
    void on_closeComBtn_clicked();
    void on_hStatePushButton_clicked();
    void on_volPushButton_clicked();
    void on_dePushButton_clicked();
    void on_lightOpenPushButton_clicked();
    void on_lightClosePushButton_clicked();
    void on_allClosePushButton_clicked();
    void on_allOpenPushButton_clicked();
    void on_broadCastPushButton_clicked();
    void on_clearPushButton_clicked();
};
#endif // MAINWINDOW_H
