#include "serialport.h"
#include "ui_serialport.h"

SerialPort::SerialPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPort)
{
    ui->setupUi(this);
    timerRead = new QTimer(this);
    timerRead->setInterval(100);
    connect(timerRead, SIGNAL(timeout()), this, SLOT(readMyCom()));
    resultState=0;
    ui->sendDataBtn->setEnabled(false);
    ui->volPushButton->setEnabled(false);
    ui->hStatePushButton->setEnabled(false);
    ui->dePushButton->setEnabled(false);
}

SerialPort::~SerialPort()
{
    delete ui;
}
