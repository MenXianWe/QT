#include "dataarea.h"
#include "ui_dataarea.h"

DataArea::DataArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataArea)
{
    ui->setupUi(this);
    init();//初始化
    //orderInit();//初始化命令
}

DataArea::~DataArea()
{
    delete ui;
}

void DataArea::init()
{
    udpRadio=new UdpSocket;
    dealData=new DaelAllData;
    tcpSocket=new TcpSocket;
    //udp广播
    connect(this,SIGNAL(sendUdpDataToDeal(QString,int)),dealData,SLOT(dealSendData(QString,int)));
    connect(dealData,SIGNAL(dealSendDataToUdp(QString)),this,SLOT(showSendData(QString)));
    connect(dealData,SIGNAL(dealSendDataToUdp(QString)),udpRadio,SLOT(UdpSendData(QString)));
    connect(udpRadio,SIGNAL(udpReadDataToDeal(QString)),dealData,SLOT(dealReadUdpData(QString)));
    connect(dealData,SIGNAL(dealReadDataToUi(QString)),this,SLOT(udpDataToShow(QString)));
    connect(dealData,SIGNAL(deviceIP(QString,int)),this,SLOT(setDeviceIPToComboBox(QString,int)));
    //TCP，开启服务，关闭服务，获得图片数据
    connect(tcpSocket,SIGNAL(msg(QString)),this,SLOT(connectStatus(QString)));
    connect(this,SIGNAL(sendTcpDataToDeal(QString,int)),dealData,SLOT(dealSendData(QString,int)));
    connect(dealData,SIGNAL(dealSendDataToTcp(QString)),this,SLOT(showSendData(QString)));//发送数据显示在桌面
    connect(dealData,SIGNAL(dealSendDataToTcp(QString)),tcpSocket,SLOT(TcpSendData(QString)));
    connect(tcpSocket,SIGNAL(tcpReadDataToDeal(QString,int,int)),dealData,SLOT(dealReadData(QString,int,int)));
    connect(dealData,SIGNAL(TcpReadDataToUi(QString)),this,SLOT(showOpenArlamData(QString)));
    //将图片发送到图片区
    connect(dealData,SIGNAL(dealReadDataImgToUi(QPixmap)),this,SIGNAL(toImgAre(QPixmap)));
    connect(dealData,SIGNAL(dealReadDataAlarmImgToUi(QPixmap)),this,SIGNAL(toAlarmImgAre(QPixmap)));
}

//初始化命令
void DataArea::orderInit()
{
    //广播命令
     udpRadioData="<?xml version=\"1.0\" encoding=\"UTF-8\"?><Server><SearchDevice /></Server>";
    //开启服务命令
     openServeData=QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><Server TargetIP=\"%1\" NowTime=\"%2\"><StartServices /></Server>").arg(comboBoxIpStr).arg(current_date);
    //关闭服务命令
     closeServeData=QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><Server TargetIP=\"%1\" NowTime=\"%2\"><StopServices /></Server>").arg(comboBoxIpStr).arg(current_date);
     //获得前端命令
     getImgData=QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><Server TargetIP=\"%1\" NowTime=\"%2\"><GetBasicPic /></Server>").arg(comboBoxIpStr).arg(current_date);
}
//获取当前时间
void DataArea::getCurrentTime()
{
    //时间
     current_date_time =QDateTime::currentDateTime();
     current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
}

//显示发送数据
void DataArea::showSendData(QString sendDataShow)
{
    ui->showDataTextEdit->append(QString(tr("发送数据>> ")+sendDataShow));
}

//显示接收数据
void DataArea::udpDataToShow(QString dataShow)
{
    ui->showDataTextEdit->append(QString(tr("数据接收<< "))+dataShow);
}

void DataArea::showOpenArlamData(QString opendata)
{
    ui->showDataTextEdit->append(QString(tr("数据接收<< "))+opendata);
}

//将ip设置到ComboBox中
void DataArea::setDeviceIPToComboBox(QString ipStr,int num)
{
    ui->ipComboBox->addItem(ipStr,num);
}

//UDP广播
void DataArea::on_udpRadioPushButton_clicked()
{
    orderInit();//初始化命令
    emit sendUdpDataToDeal(udpRadioData,RadioType);
}

//TCP连接
void DataArea::on_connectPushButton_clicked()
{
    if(ui->connectPushButton->text()=="连接"){
        comboBoxIpStr=ui->ipComboBox->currentText(); //ip
        qint16 port=ui->portComboBox->currentText().toInt();//port
        tcpSocket->socketConnect(comboBoxIpStr,port);//连接tcp
        ui->connectPushButton->setText("关闭");
    }else if(ui->connectPushButton->text()=="关闭"){
        tcpSocket->disconnectFromHost();
        ui->connectPushButton->setText("连接");
        //imgTimer->stop();
        ui->showDataTextEdit->append("连接已断开");
    }

}


//连接状态
void DataArea::connectStatus(QString str)
{
    ui->showDataTextEdit->append(str);
}

//清除
void DataArea::on_clearPushButton_clicked()
{
    ui->showDataTextEdit->clear();
}

//开启服务，tcp连接，端口6002
//发送现实图片
void DataArea::on_openAlarmPushButton_clicked()
{
    if(ui->connectPushButton->text()=="连接"){
        QMessageBox::warning(NULL,"warning","未连接", QMessageBox::Yes);
        return;
    }
    //发送开启服务命令
    getCurrentTime();//更新时间
    orderInit();//初始化命令
    emit sendTcpDataToDeal(openServeData,TcpSocketType);
    //开启监听
    tcpSocket->listenPort();
    //每秒获取图片
    imgTimer=new QTimer;
    imgTimer->setInterval(1000);
    imgTimer->start();
    connect(imgTimer,SIGNAL(timeout()),this,SLOT(sendImgOrder()));
}
//发送图片命令
void DataArea::sendImgOrder()
{
    emit sendTcpDataToDeal(getImgData,TcpSocketType);
}

//关闭服务
void DataArea::on_closeSerivePushButton_clicked()
{
    getCurrentTime();//更新时间
    orderInit();//初始化命令
    emit sendTcpDataToDeal(closeServeData,TcpSocketType);
    imgTimer->stop();
    //tcpSocket->closeListen();
}

