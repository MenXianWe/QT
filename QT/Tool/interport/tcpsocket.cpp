#include "tcpsocket.h"

TcpSocket::TcpSocket()
{
    socket=new QTcpSocket;
    connect(socket,SIGNAL(readyRead()),this,SLOT(readTcpData()));
    serverSocket=new QTcpSocket;
    server=new QTcpServer;
}

//TCP连接
void TcpSocket::socketConnect(QString ipStr, qint16 port)
{
    socket->connectToHost((QHostAddress)ipStr,port);
    connect(socket,SIGNAL(connected()),this,SLOT(connectStatus()));
}

//发送数据，传进来的数据是经过处理的
void TcpSocket::TcpSendData(QString dataStr)
{
    socket->write(dataStr.toUtf8());
}


//监听6901
void TcpSocket::listenPort()
{
    server->listen(QHostAddress::Any,6901);
    connect(server,SIGNAL(newConnection()),this,SLOT(serverAnswer()));
}

void TcpSocket::closeListen()
{
    server->close();
}

//连接状态
void TcpSocket::connectStatus()
{
    //将数据发送到数据数据处理地方
    emit msg("连接成功！");
}

//读取数据，此处不做处理，传给数据处理类
void TcpSocket::readTcpData()
{
    QByteArray data=socket->readAll();
    dataTempStr+=data;
    QString lenStr=dataTempStr.mid(7,13);//获取包头中包体的长度
    int Datalen=lenStr.toInt();//转换成整数
    if(dataTempStr.length()-20>=Datalen)
    {
        emit tcpReadDataToDeal(dataTempStr,Datalen,0);//0表示时事图片，1表示报警图片
        dataTempStr="";
    }
}

void TcpSocket::serverAnswer()
{
    serverSocket=server->nextPendingConnection();
    QString ipStr=serverSocket->peerAddress().toString();
    qint16 port=serverSocket->peerPort();
    qDebug()<<"连接："<<"ip:"<<ipStr<<" "<<"port:"<<port;
    connect(serverSocket,SIGNAL(readyRead()),this,SLOT(serverShowData()));
    connect(serverSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(MSGError(QAbstractSocket::SocketError)));
}

void TcpSocket::MSGError(QAbstractSocket::SocketError)
{
    int error = serverSocket->error();
    switch(error)
    {
        case QAbstractSocket::RemoteHostClosedError://客户端断开
        {
            QString hostAddress=serverSocket->QAbstractSocket::peerAddress().toString();
            qDebug()<<QString("客户端[%1]断开连接\r\n").arg(hostAddress);
            break;
        }
        default:
            break;
    }
}

void TcpSocket::serverShowData()
{
    QByteArray read=serverSocket->readAll();//收的图片数据
    readAllDataSever+=read;
    QString lenStr=readAllDataSever.mid(7,13);//获取包头中包体的长度
    int Datalen=lenStr.toInt();//转换成整数
    if(readAllDataSever.length()-20>=Datalen)
    {
        emit tcpReadDataToDeal(readAllDataSever,Datalen,1);
         readAllDataSever="";
    }
}
