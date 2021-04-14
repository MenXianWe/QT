#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QDomDocument>
#include <QTimer>
#include <QMutexLocker>
#include <QMutex>
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
    void RecvData();
    void answer();
    void readInfor();
    void dealReceData();
    void out();
    void on_sendPushButton_clicked();
    void on_showPixPushButton_clicked();

    void on_pushButton_2_clicked();

    void on_connectPushButton_clicked();

    void on_disconnectPushButton_clicked();
    //void out();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *m_qudpSocket;
    QTcpSocket *socket;
    QString  data;
    QString DeviceIP;
    QByteArray readAll;

        qint64 totalBytes = 0;//发送数据的总大小
        qint64 receivedBytes = 0;//已收到的
        QByteArray infoBlock;//帧体内容
        QByteArray inBlock;//临时存储的数据

        QByteArray s_frameHead;//帧头
        QByteArray s_frameContent;//帧体
        QByteArray s_frameTail;//帧位

QMutex mutex;
};
#endif // MAINWINDOW_H
