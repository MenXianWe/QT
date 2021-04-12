#ifndef SENDFILECLIENT_H
#define SENDFILECLIENT_H

#include <QTcpSocket>

class SendFileClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit SendFileClient(QObject *parent = 0);
    ~SendFileClient();

    void sendFile(QString fileName, QString serverIP, int serverPort);

signals:
    void fileSize(qint64 size);
    void message(QString msg);

private slots:
    void sendData();
    void error();

private:
    QString fileName;

};

#endif // SENDFILECLIENT_H
