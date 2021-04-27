#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QTcpServer>
class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket();
    void socketConnect(QString ipStr,qint16 port);

signals:
    void msg(QString msgString);
    void tcpReadDataToDeal(QString dataStr,int len,int type);
private:
    QString dataTempStr="";//临时存储读到的数据
    QString readAllDataSever="";
    QTcpSocket *socket;//开启/关闭服务、时事图片，端口6902
    QTcpSocket *serverSocket;//获得监听图片，端口6901
    QTcpServer *server;//监听6901端口
public slots:
    void TcpSendData(QString dataStr);//发送数据
    void listenPort();
    void closeListen();
private slots:
    void connectStatus();//连接状态
    void readTcpData();//读取报警主机传来的数据
    void serverAnswer();
    void MSGError(QAbstractSocket::SocketError);
    void serverShowData();
};

#endif // TCPSOCKET_H
