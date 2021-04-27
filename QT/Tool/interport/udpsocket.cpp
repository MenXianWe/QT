#include "udpsocket.h"

UdpSocket::UdpSocket()
{
    //绑定所有ip来接收组播组信息
    this->bind(QHostAddress::AnyIPv4,6905,QUdpSocket::ShareAddress);
    //禁止接收本机内容
    this->setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
    //加入组播组ip：224.0.0.17
    this->joinMulticastGroup(QHostAddress("224.0.0.17"));
    //设置缓冲区
   // this->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,1024*1024*8);
    connect(this,SIGNAL(readyRead()),this,SLOT(UdpReadData()));//连接接收信号槽
}

//发送组波命令
void UdpSocket::UdpSendData(QString UdpSendDataStr)
{
    //this->joinMulticastGroup(QHostAddress("224.0.0.17")); //加入组播ip：224.0.0.17
    this->writeDatagram(UdpSendDataStr.toUtf8(),QHostAddress("224.0.0.17"),6905);
}

void UdpSocket::UdpReadData()
{
    QByteArray baRecv;
    QString strRecvData;
    while(this->hasPendingDatagrams())
    {
        baRecv.resize(this->pendingDatagramSize());
        this->readDatagram(baRecv.data(),baRecv.size());
        //读到的数据
        strRecvData = baRecv;
        //将数据传出去
        emit udpReadDataToDeal(strRecvData);
    }

}

