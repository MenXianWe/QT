#include "receivefilethread.h"
#include "head.h"

ReceiveFileThread::ReceiveFileThread(int socketDescriptor, QObject *parent) : QThread(parent)
{
    blockSize = 0;
    blockNumber  = 0;
    s = new QTcpSocket(this);
    s->setSocketDescriptor(socketDescriptor);
    connect(s, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(s, SIGNAL(disconnected()), this, SLOT(disConnect()));
    connect(s, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
}

ReceiveFileThread::~ReceiveFileThread()
{

}

void ReceiveFileThread::proccessData(QByteArray &array)
{
    QDataStream in(&array, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_7);

    int key;
    QByteArray data;
    in >> key >> data;
    blockNumber ++;

    emit message(QString("已接收数据包:%1个").arg( blockNumber));
    emit message(QString("收到标识符:%1 当前数据包大小:%2字节").arg(key).arg(data.size()));

    switch(key) {
    case 0x01:
        fileName = fileName.fromUtf8(data.data(), data.size());
        file.setFileName(qApp->applicationDirPath() + "/" + fileName);
        emit receiveFileName(file.fileName());
        if(file.exists()) {
            file.remove();
        }
        if(!file.open(QIODevice::WriteOnly)) {
            emit message("不能打开文件进行写入");
            break;
        }
        break;
    case 0x02: {
        QString size = QString::fromUtf8(data.data(), data.size());
        emit receiveFileSize(size.toUInt());
        break;
    }
    case 0x03:
        file.write(data.data(), data.size());
        file.flush();
        break;
    case 0x04:
        file.close();

#ifdef Q_OS_LINUX        
        //根据拓展名判断是否要进行解压        
        if (fileName.endsWith(".tar.gz")) {
            qApp->processEvents();
            emit message("正在解压文件");
            int result = system(QString("tar zxvf %1 -C /").arg(fileName).toLatin1());
            emit message(QString("解压文件完成 结果 : %1").arg(result));
        }
#endif
        s->disconnectFromHost();
        break;
    }
}

void ReceiveFileThread::run()
{
    emit message(QString("收到新连接请求 IP:%1[%2]")
                 .arg(s->peerAddress().toString())
                 .arg(s->peerPort()));
    exec();
}

void ReceiveFileThread::disConnect()
{
    exit(0);
}

void ReceiveFileThread::receiveData()
{
    while(s->bytesAvailable() >= sizeof(quint64)) {
        if(blockSize == 0) {
            if(s->bytesAvailable() < sizeof(qint64)) {
                return;
            }
            s->read((char *)&blockSize, sizeof(qint64));
        }
        if(s->bytesAvailable() < blockSize) {
            return;
        }
        emit receiveData(blockSize + sizeof(qint64));
        QByteArray data = s->read(blockSize);
        proccessData(data);
        blockSize = 0;
    }
}

void ReceiveFileThread::error()
{
    emit message(QString("接收文件遇到错误:%1").arg(s->errorString()));
    emit message(QString("正在移除文件:%1").arg(file.fileName()));
    if(file.isOpen()) {
        file.close();
    } else {
        return;
    }
    if(!file.fileName().isEmpty()) {
        file.remove(fileName);
    } else {
        return;
    }
}

