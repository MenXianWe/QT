#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    socket=new QTcpSocket(this);
    setWindowTitle(tr("Client"));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::answer()
{
    ui->readTextEdit->append(tr("连接成功"));
}

void Widget::readInfor()
{
    QByteArray read=socket->readAll();
    ui->readTextEdit->append(read);
}

void Widget::on_connectPushButton_clicked()
{
    QString ipStr=ui->ipLineEdit->text();
    qint16 port=ui->portLineEdit->text().toInt();
    socket->connectToHost((QHostAddress)ipStr,port);
    connect(socket,SIGNAL(connected()),this,SLOT(answer()));
}

void Widget::on_sendPushButton_clicked()
{
    QString sendText=ui->sendTextEdit->toPlainText();
    socket->write(sendText.toUtf8().data());
    connect(socket,SIGNAL(readyRead()),this,SLOT(readInfor()));
}

void Widget::on_disConnectPushButton_clicked()
{
    socket->disconnectFromHost();
}
