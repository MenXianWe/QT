#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QtNetwork>
#include <QDomDocument>
#include <QTimer>
#include <QSize>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void severAnswer();
//    void severReadInfor();
    void sendData(QString data);
    void showImage();
    void RecvData();
    void answer();
    void readInfor();
    void checkData(QByteArray data,int len);
    void severShowData();
    void on_sendPushButton_clicked();
    void on_showPixPushButton_clicked();
    void on_connectPushButton_clicked();
    void on_disconnectPushButton_clicked();
    void on_pushButton_2_clicked();

    void on_openAlarmPushButton_clicked();

    void on_closeSerivePushButton_clicked();

    void on_alarmPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QByteArray readAllDataSever;
    QDateTime current_date_time;
    QString current_date;
    QUdpSocket *m_qudpSocket;
    QTcpSocket *socket;
    QString  data;
    QString DeviceIP;
    QByteArray readAllData;
    QTimer *readTimer,*listenTimer;
    bool connectStatus;
    QTcpServer *sever;
    QTcpSocket *severSocket;
};
#endif // MAINWINDOW_H
