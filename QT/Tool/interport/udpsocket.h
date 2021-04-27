#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QUdpSocket>

class UdpSocket : public QUdpSocket
{
    Q_OBJECT
public:
    UdpSocket();
signals:
    void udpReadDataToDeal(QString udpreadData);
private slots:
    void UdpReadData();
    void UdpSendData(QString UdpSendDataStr);
};

#endif // UDPSOCKET_H
