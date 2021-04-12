#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("Sever"));
    sever=new QTcpServer(this);
    sever->listen(QHostAddress::Any,8888);
    connect(sever,SIGNAL(newConnection()),this,SLOT(answer()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::answer()
{
    socket=sever->nextPendingConnection();
    QString ipStr=socket->peerAddress().toString();
    qint16 port=socket->peerPort();
    ui->readTextEdit->append(QString("[%1:%2]连接成功").arg(ipStr).arg(port));
}

void Widget::readInfor()
{
    QByteArray readByte=socket->readAll();
    ui->readTextEdit->append(readByte);
}


void Widget::on_sendPushButton_clicked()
{
    QString sendText=ui->sendTextEdit->toPlainText();
    socket->write(sendText.toUtf8().data());
    connect(socket,SIGNAL(readyRead()),this,SLOT(readInfor()));
}

void Widget::on_disconnectPushButton_clicked()
{
    socket->disconnectFromHost();
    socket->close();
    socket=NULL;
}
