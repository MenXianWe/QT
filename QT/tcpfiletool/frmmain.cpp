#include "frmmain.h"
#include "ui_frmmain.h"
#include "head.h"
#include "app.h"

frmMain::frmMain(QWidget *parent) : QWidget(parent), ui(new Ui::frmMain)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::initForm()
{
    sendBytes = 0;
    sendBlockNumber = 0;
    sendMaxBytes = 0;

    receiveBytes = 0;
    receiveBlockNumber = 0;
    receiveMaxBytes = 0;

    ui->pbarSendData->setRange(0, 100);
    ui->pbarSendData->setValue(0);
    ui->pbarReceiveData->setRange(0, 100);
    ui->pbarReceiveData->setValue(0);

    receive = new ReceiveFileServer(this);
    connect(receive, SIGNAL(finished()), this, SLOT(receiveFinsh()));
    connect(receive, SIGNAL(message(QString)), this, SLOT(updateReceiveStatus(QString)));
    connect(receive, SIGNAL(receiveFileName(QString)), this, SLOT(receiveFileName(QString)));
    connect(receive, SIGNAL(receiveFileSize(qint64)), this, SLOT(setReceivePBar(qint64)));
    connect(receive, SIGNAL(receiveData(qint64)), this, SLOT(updateReceivePBar(qint64)));

    connect(ui->pbarSendData, SIGNAL(valueChanged(int)), this, SLOT(sendValueChanged()));
    connect(ui->pbarReceiveData, SIGNAL(valueChanged(int)), this, SLOT(receiveValueChanged()));

    firstCheck = true;
    lastTime = QDateTime::currentDateTime();
    timerCheck = new QTimer(this);
    connect(timerCheck, SIGNAL(timeout()), this, SLOT(checkFile()));
    timerCheck->start(1000);
}

void frmMain::sendValueChanged()
{
    int value = ui->pbarSendData->value();
    int max = ui->pbarSendData->maximum();
    int percent = ((double)value / max) * 100;

    QString qss;
    if (percent < 50) {
        qss = "QProgressBar{color:#000000;}QProgressBar::chunk{background:rgb(60, 140, 220);}";
    } else {
        qss = "QProgressBar{color:#FFFFFF;}QProgressBar::chunk{background:rgb(60, 140, 220);}";
    }

    ui->pbarSendData->setStyleSheet(qss);
}

void frmMain::receiveValueChanged()
{
    int value = ui->pbarReceiveData->value();
    int max = ui->pbarReceiveData->maximum();
    int percent = ((double)value / max) * 100;

    QString qss;
    if (percent < 50) {
        qss = "QProgressBar{color:#000000;}QProgressBar::chunk{background:rgb(60, 140, 220);}";
    } else {
        qss = "QProgressBar{color:#FFFFFF;}QProgressBar::chunk{background:rgb(60, 140, 220);}";
    }

    ui->pbarReceiveData->setStyleSheet(qss);
}

void frmMain::checkFile()
{
    if (!App::AutoListen) {
        return;
    }

    QFile file(App::SendFile);
    if (file.size() == 0) {
        return;
    }

    QFileInfo fileInfo(file);
    QDateTime now = fileInfo.lastModified();
    if (now != lastTime) {
        if (!firstCheck) {
            QTimer::singleShot(1000, this, SLOT(on_btnSend_clicked()));
        }

        firstCheck = false;
        lastTime = now;
    }
}

void frmMain::initConfig()
{
    ui->txtServerIP->setText(App::ServerIP);
    connect(ui->txtServerIP, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtServerPort->setText(QString::number(App::ServerPort));
    connect(ui->txtServerPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtSendFile->setText(App::SendFile);
    connect(ui->txtSendFile, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtListenPort->setText(QString::number(App::ListenPort));
    connect(ui->txtListenPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->ckAutoListen->setChecked(App::AutoListen);
    connect(ui->ckAutoListen, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));
}

void frmMain::saveConfig()
{
    App::ServerIP = ui->txtServerIP->text();
    App::ServerPort = ui->txtServerPort->text().toInt();
    App::SendFile = ui->txtSendFile->text();
    App::ListenPort = ui->txtListenPort->text().toInt();
    App::AutoListen = ui->ckAutoListen->isChecked();
    App::writeConfig();
}

void frmMain::on_btnSend_clicked()
{
    sendBytes = 0;
    sendBlockNumber = 0;
    sendMaxBytes = 0;

    if (App::ServerIP == "" || App::ServerPort == 0) {
        QMessageBox::information(this, "提示", "服务器IP地址和端口不能为空,请重新填写!");
        return;
    }

    if (App::SendFile == "" ) {
        QMessageBox::critical(this, "提示", "发送文件不能为空,请选择要发送的文件!");
        return;
    }

    SendFileClient *send = new SendFileClient(this);
    connect(send, SIGNAL(disconnected()), this, SLOT(sendFinsh()));
    connect(send, SIGNAL(disconnected()), send, SLOT(deleteLater()));
    connect(send, SIGNAL(fileSize(qint64)), this, SLOT(setSendPBar(qint64)));
    connect(send, SIGNAL(bytesWritten(qint64)), this, SLOT(updateSendPBar(qint64)));
    connect(send, SIGNAL(message(QString)), this, SLOT(updateSendStatus(QString)));

    send->sendFile(App::SendFile, App::ServerIP, App::ServerPort);
}

void frmMain::updateSendPBar(qint64 size)
{
    sendBlockNumber ++;
    sendBytes += size;
    ui->pbarSendData->setValue(sendBytes);
    QString msg = QString("已发数据包:%1个 当前数据包大小:%2字节 已发字节:%3 总共字节:%4")
                  .arg(sendBlockNumber).arg(size).arg(sendBytes).arg(sendMaxBytes);
    ui->txtSendStatus->setText(msg);
    qApp->processEvents();
}

void frmMain::updateSendStatus(QString msg)
{
    //ui->txtSendStatus->setText(msg);
    qDebug() << "发送文件客户端:" << msg;
}

void frmMain::setSendPBar(qint64 size)
{
    sendMaxBytes = size;
    ui->pbarSendData->setRange(0, size - 1);
    ui->pbarSendData->setValue(0);
}

void frmMain::sendFinsh()
{
    ui->pbarSendData->setRange(0, 100);
    ui->pbarSendData->setValue(100);
    ui->txtSendStatus->setText("发送文件成功");
}

void frmMain::on_btnSelect_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择文件");
    if (fileName.isEmpty()) {
        return;
    }

    ui->txtSendFile->setText(fileName);
    ui->pbarSendData->setRange(0, 100);
    ui->pbarSendData->setValue(0);
    ui->pbarReceiveData->setRange(0, 100);
    ui->pbarReceiveData->setValue(0);
}

void frmMain::on_btnListen_clicked()
{
    if (App::ListenPort != 0) {
        if (ui->btnListen->text() == "开始监听") {
            bool ok = receive->startServer(App::ListenPort);
            if (ok) {
                ui->txtReceiveStatus->setText("监听成功");
                ui->btnListen->setText("停止监听");
            } else {
                ui->txtReceiveStatus->setText("监听失败");
            }
        } else {
            receive->stopServer();
            ui->txtReceiveStatus->setText("停止监听成功");
            ui->btnListen->setText("开始监听");
        }
    }

    ui->pbarReceiveData->setRange(0, 100);
    ui->pbarReceiveData->setValue(0);
}

void frmMain::updateReceivePBar(qint64 size)
{
    receiveBlockNumber ++;
    receiveBytes += size;
    ui->pbarReceiveData->setValue(receiveBytes);
    QString msg = QString("已接收数据包:%1个 当前数据包大小:%2字节 已接收字节:%3 总共字节:%4")
                  .arg(receiveBlockNumber).arg(size).arg(receiveBytes).arg(receiveMaxBytes);
    ui->txtReceiveStatus->setText(msg);
    qApp->processEvents();
}

void frmMain::updateReceiveStatus(QString msg)
{
    qDebug() << "接收文件服务端:" << msg;
}

void frmMain::setReceivePBar(qint64 size)
{
    receiveBytes = 0;
    receiveBlockNumber = 0;
    receiveMaxBytes = size;
    ui->pbarReceiveData->setRange(0, size - 1);
    ui->pbarReceiveData->setValue(0);
}

void frmMain::receiveFinsh()
{
    ui->pbarReceiveData->setRange(0, 100);
    ui->pbarReceiveData->setValue(100);
    ui->txtReceiveStatus->setText("发送文件成功");
}

void frmMain::receiveFileName(QString name)
{
    ui->pbarReceiveData->setRange(0, 100);
    ui->pbarReceiveData->setValue(100);
    ui->txtReceiveFile->setText(name);
}
