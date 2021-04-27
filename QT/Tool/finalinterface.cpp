#include "finalinterface.h"
#include "ui_finalinterface.h"

FinalInterFace::FinalInterFace(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FinalInterFace)
{
    ui->setupUi(this);
    setWindowTitle(tr("myTool"));
    //将串口界面插入到tabWidget中
    port=new SerialPort;
    ui->tabWidget->insertTab(0,port,tr("串口通信"));
    interport=new MainWindow;
    ui->tabWidget->insertTab(1,interport,tr("网络通信"));
}

FinalInterFace::~FinalInterFace()
{
    delete ui;
}

