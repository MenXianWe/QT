#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("SerialPortTest");
    timerRead = new QTimer(this);
    timerRead->setInterval(100);
    connect(timerRead, SIGNAL(timeout()), this, SLOT(readMyCom()));
    resultState=0;
    ui->sendDataBtn->setEnabled(false);
    ui->volPushButton->setEnabled(false);
    ui->hStatePushButton->setEnabled(false);
    ui->dePushButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}



