#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readAll="";
    m_qudpSocket = new QUdpSocket();
    socket=new QTcpSocket;
    data="IALARM:0000000000000<?xml version=\"1.0\" encoding=\"UTF-8\"?><Server><SearchDevice /></Server>";
    //QString strIP = "224.0.0.17";
    m_qudpSocket->bind(QHostAddress::AnyIPv4,6905,QUdpSocket::ShareAddress);//绑定所有ip来接收组播组信息
    m_qudpSocket->joinMulticastGroup(QHostAddress("224.0.0.17"));//加入组播组ip：224.0.0.17
    m_qudpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,1024*1024*8);//设置缓冲区
    m_qudpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);//禁止接收本机内容
    connect(m_qudpSocket,SIGNAL(readyRead()),this,SLOT(RecvData()));//连接接收信号槽

}

MainWindow::~MainWindow()
{
    delete ui;
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
           // qDebug()<<root.nodeName();
            QDomElement e=root.toElement();
            DeviceIP=e.attribute("DeviceIP");
            //qDebug()<<DeviceIP;
            ui->ipComboBox->addItem(DeviceIP);

    }
    //qDebug()<<"break\n";
}

void MainWindow::answer()
{
    ui->sendTextEdit->append(tr("连接成功"));
}

void MainWindow::readInfor()
{
   // qDebug()<<"fgggggggggggggggggggg";
//    if(socket->bytesAvailable() == 0) {
//         qDebug()<<"Lgggggggggggggggggggg";
//        return;
//    }
//    QByteArray read=socket->readAll();//收的图片数据
//    QString lenStr=read.mid(7,13);//获取包头长度
//    int Datalen=lenStr.toInt();//转换成整数
//    qDebug()<<lenStr;
//    qDebug()<<Datalen;
    //readAll=read+read2;
QMutexLocker mulo(&mutex);
readAll.append(socket->readAll());



    ui->receiveTextEdit->append(read);//可以完整显示数据
    qDebug()<<"xxxxxxxx"<<read;//只能显示部分数据,收到的数据不完整
   // 测试发现数据太大，readyReady（connect(socket,SIGNAL(readyRead()),this,SLOT(readInfor()));）触发多次，请问怎么解决这样的问题

//    dealReceData();
//    ui->receiveTextEdit->setText(infoBlock);
    //readInfor();
}

void MainWindow::dealReceData()
{

    //递归边界：跳出处理
        if(socket->bytesAvailable() == 0) return;//缓存

        //处理帧头
            if(receivedBytes < sizeof (qint64) && infoBlock.length() == 0){
                 if(socket->bytesAvailable() >= sizeof (qint64) && s_frameHead.size() == 0){
                       s_frameHead = socket->read(sizeof (qint64));
    qDebug()<<"s_frameHead:"<<s_frameHead.toHex().toUpper();
                   }else{
   qDebug()<<"接收头返回";
                     return;
                 }
            }
            //持续接收帧体数据
        if(receivedBytes < totalBytes && socket->bytesAvailable() > 0){
                //QByteArray inBlock = tcpSocket->readAll();
               if(socket->bytesAvailable() >= (totalBytes - receivedBytes)){
                    inBlock = socket->read(totalBytes - receivedBytes);
               }else{
                    inBlock = socket->readAll();
               }

                infoBlock.append(inBlock);

                receivedBytes += inBlock.size();
     // qDebug()<<"inBlock:"<<byteArrayToHexStr(inBlock);
    qDebug()<<"持续接收receivedBytes:"<<receivedBytes;
                inBlock.resize(0);

            }
            //一帧数据收满开始处理
         if(receivedBytes == totalBytes){
                //处理
            }
            //如果还是有残留数据，再递归处理数据
            //此处还可以增加特判帧头里的类型字段，来判断是否要递归立即处理，还是等下次数据来触发信号后再处理
                if(socket->bytesAvailable() > 0 ){
                dealReceData();
            }



}

void MainWindow::out()
{
    readInfor();

}

void MainWindow::on_showPixPushButton_clicked()
{
    //send
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
   // qDebug()<<current_date;
    //QString imgDatah=QString("IALARM:0000000000124"
    QString imgDataw=QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><Server TargetIP=\"%1\" NowTime=\"%2\"><GetBasicPic /></Server>").arg(DeviceIP).arg(current_date);
    int len=imgDataw.size();
     QString str=QString("%1").arg(len,13,10,QLatin1Char('0'));
    QString imgDatah="IALARM:"+str;
    QString imgData=imgDatah+imgDataw;
    // QString sendText=ui->sendTextEdit->toPlainText();
    ui->sendTextEdit->append(imgData);
    socket->write(imgData.toUtf8());
    connect(socket,SIGNAL(readyRead()),this,SLOT(readInfor()));
    //qDebug()<<"111111"<<str;
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
    //qDebug()<<ipStr;
    qint16 port=ui->portComboBox->currentText().toInt();
    socket->connectToHost((QHostAddress)ipStr,port);
    connect(socket,SIGNAL(connected()),this,SLOT(answer()));
}

void MainWindow::on_disconnectPushButton_clicked()
{

    //disconnect
    socket->disconnectFromHost();
}

void MainWindow::on_pushButton_clicked()
{
    readAll=readAll.mid(20);
    qDebug()<<"readALL0000000000000"<<readAll;
    QDomDocument dom;
    QString errorMsg;
    int errorLine, errorColumn;
    if(!dom.setContent(readAll, &errorMsg, &errorLine, &errorColumn)) {
        qDebug() << "Parse error at line " +  errorMsg;
    }

    QDomElement root=dom.documentElement();
    qDebug()<<root.nodeName();
    QDomElement firstChild=root.firstChildElement();
    qDebug() << firstChild.tagName();
    QByteArray imgDataStr=firstChild.text().toUtf8();
    qDebug()<<"11111111111111111"<<imgDataStr;
}
