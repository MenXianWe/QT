#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    current_date_time =QDateTime::currentDateTime();
    current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    readAllData="";
    readAllDataSever="";
    connectStatus=false;
    readTimer=new QTimer;
    m_qudpSocket = new QUdpSocket();
    socket=new QTcpSocket;
    data="IALARM:0000000000000<?xml version=\"1.0\" encoding=\"UTF-8\"?><Server><SearchDevice /></Server>";
    //QString strIP = "224.0.0.17";
    m_qudpSocket->bind(QHostAddress::AnyIPv4,6905,QUdpSocket::ShareAddress);//绑定所有ip来接收组播组信息
    m_qudpSocket->joinMulticastGroup(QHostAddress("224.0.0.17"));//加入组播组ip：224.0.0.17
    m_qudpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,1024*1024*8);//设置缓冲区
    m_qudpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);//禁止接收本机内容
    connect(m_qudpSocket,SIGNAL(readyRead()),this,SLOT(RecvData()));//连接接收信号槽
    sever=new QTcpServer(this);
    severSocket=new QTcpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::severAnswer()
{
    severSocket=sever->nextPendingConnection();
    QString ipStr=severSocket->peerAddress().toString();
    qint16 port=severSocket->peerPort();
    ui->sendTextEdit->append(QString("[%1:%2]连接成功").arg(ipStr).arg(port));
    listenTimer=new QTimer;
    listenTimer->setInterval(1000);
    connect(severSocket,SIGNAL(readyRead()),this,SLOT(severShowData()));
}

void MainWindow::sendData(QString data)
{
    int len=data.size();
    QString str=QString("%1").arg(len,13,10,QLatin1Char('0'));
    QString datah="IALARM:"+str;
    QString dataAll=datah+data;
    ui->sendTextEdit->append(dataAll);
    socket->write(dataAll.toUtf8());
    connect(socket,SIGNAL(readyRead()),this,SLOT(readInfor()));
}




void MainWindow::on_sendPushButton_clicked()
{
    m_qudpSocket->joinMulticastGroup(QHostAddress("224.0.0.17")); //加入组播ip：224.0.0.17
    ui->sendTextEdit->append("发送数据>>"+data);
    m_qudpSocket->writeDatagram(data.toUtf8(),QHostAddress("224.0.0.17"),6905);//ui->sendTextEdit->toPlainText().toUtf8()
}

void MainWindow::RecvData()
{
    QByteArray baRecv;
    while(m_qudpSocket->hasPendingDatagrams())
    {
        baRecv.resize(m_qudpSocket->pendingDatagramSize());
        m_qudpSocket->readDatagram(baRecv.data(),baRecv.size());
        QString strRecvData = baRecv;
        ui->receiveTextEdit->setPlainText(strRecvData);
        //qDebug()<<baRecv.data();

        QString xmlStr=strRecvData.mid(20);

        QDomDocument dom;
        QString errorMsg;
        int errorLine, errorColumn;
        if(!dom.setContent(xmlStr, &errorMsg, &errorLine, &errorColumn)) {
            qDebug() << "Parse error at line " +  errorMsg;
            continue;
        }

        QDomElement root=dom.documentElement();

            QDomElement e=root.toElement();
            DeviceIP=e.attribute("DeviceIP");

            ui->ipComboBox->addItem(DeviceIP);

    }

}
//如果TCP连接成功会调用这个槽函数
void MainWindow::answer()
{
    ui->sendTextEdit->append(tr("连接成功"));
    connectStatus=true;//将连接状态设置为true
}

void MainWindow::readInfor()
{
    QByteArray read=socket->readAll();//收的图片数据
    readAllData+=read;
    QString lenStr=readAllData.mid(7,13);//获取包头中包体的长度
    int Datalen=lenStr.toInt();//转换成整数
    qDebug()<<"长度："<<Datalen;
    qDebug()<<"readALLData长度："<<readAllData.length();
    if(readAllData.length()-20>=Datalen)
    {
        qDebug()<<"判断";
        checkData(readAllData,Datalen);
         readAllData="";
    }
    ui->receiveTextEdit->append(read);//可以完整显示数据
}

void MainWindow::checkData(QByteArray data,int len)
{
        data=data.mid(20,len);
        QDomDocument dom;
        QString errorMsg;
        int errorLine, errorColumn;
        if(!dom.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
            qDebug() << "CHeck Parse error at line " +  errorMsg;
        }
        QDomElement root=dom.documentElement();
        QDomElement firstChild=root.firstChildElement();
        QString  imgDataStr=firstChild.text();
        QPixmap image;
        QSize pixSize(341,231);
        image.loadFromData(QByteArray::fromBase64(imgDataStr.toLocal8Bit()));
        QPixmap scaledPixmap = image.scaled(pixSize, Qt::KeepAspectRatio);
        ui->imgLabel->setPixmap(scaledPixmap);
        ui->imgLabel->setAlignment(Qt::AlignCenter);
}

void MainWindow::severShowData()
{
    qDebug()<<"ssssssssssssssss";
    QByteArray read=severSocket->readAll();//收的图片数据
    qDebug()<<"read:"<<read;
    readAllDataSever+=read;
    QString lenStr=readAllDataSever.mid(7,13);//获取包头中包体的长度
    int Datalen=lenStr.toInt();//转换成整数
    qDebug()<<"长度："<<Datalen;
    qDebug()<<"readAllDataSever长度："<<readAllDataSever.length();
    if(readAllDataSever.length()-20>=Datalen)
    {
        qDebug()<<"判断";
        checkData(readAllDataSever,Datalen);
         readAllDataSever="";
    }
    ui->receiveTextEdit->append(read);//可以完整显示数据
}


void MainWindow::showImage()
{
    QString imgDataw=QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><Server TargetIP=\"%1\" NowTime=\"%2\"><GetBasicPic /></Server>").arg(DeviceIP).arg(current_date);
    sendData(imgDataw);
}

void MainWindow::on_showPixPushButton_clicked()
{
    //send

    readTimer->setInterval(1000);
    readTimer->start();
    connect(readTimer,SIGNAL(timeout()),this,SLOT(showImage()));
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->sendTextEdit->clear();
    ui->receiveTextEdit->clear();
}

void MainWindow::on_connectPushButton_clicked()
{

    //connect
    QString ipStr=ui->ipComboBox->currentText();

    qint16 port=ui->portComboBox->currentText().toInt();
    socket->connectToHost((QHostAddress)ipStr,port);
    connect(socket,SIGNAL(connected()),this,SLOT(answer()));
}

void MainWindow::on_disconnectPushButton_clicked()
{
    //disconnect
    connectStatus=false;
    readTimer->stop();
    socket->disconnectFromHost();
}

void MainWindow::on_openAlarmPushButton_clicked()
{
    //启动服务
    QString startServiceDataw=QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><Server TargetIP=\"%1\" NowTime=\"%2\"><StartServices /></Server>").arg(DeviceIP).arg(current_date);
    sendData(startServiceDataw);


}

void MainWindow::on_closeSerivePushButton_clicked()
{
    QString closeSeriveData=QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><Server TargetIP=\"%1\" NowTime=\"%2\"><StopServices /></Server>").arg(DeviceIP).arg(current_date);
    sendData(closeSeriveData);
}

void MainWindow::on_alarmPushButton_clicked()
{
    //监听6901
    sever->listen(QHostAddress::Any,6901);
    connect(sever,SIGNAL(newConnection()),this,SLOT(severAnswer()));
}
