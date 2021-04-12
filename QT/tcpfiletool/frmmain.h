#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>
#include <QDateTime>
#include "sendfileclient.h"
#include "receivefileserver.h"

namespace Ui
{
class frmMain;
}

class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

private:
    Ui::frmMain *ui;

    int sendBytes;
    qint64 sendBlockNumber;
    qint64 sendMaxBytes;

    int receiveBytes;
    qint64 receiveBlockNumber;
    qint64 receiveMaxBytes;

    ReceiveFileServer *receive;

    bool firstCheck;
    QDateTime lastTime;
    QTimer *timerCheck;

private slots:    
    void initForm();
    void initConfig();
    void saveConfig();

    void sendValueChanged();
    void receiveValueChanged();

    void checkFile();

private slots:    
    void on_btnSend_clicked();
    void on_btnSelect_clicked();
    void on_btnListen_clicked();

    void updateSendPBar(qint64 size);
    void updateSendStatus(QString msg);
    void setSendPBar(qint64 size);
    void sendFinsh();

    void updateReceivePBar(qint64 size);
    void updateReceiveStatus(QString msg);
    void setReceivePBar(qint64 size);
    void receiveFinsh();
    void receiveFileName(QString name);

};

#endif // FRMMAIN_H
