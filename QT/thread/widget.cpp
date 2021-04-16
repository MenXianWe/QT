#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    button=new QPushButton("开始");
    QHBoxLayout *mainLayout=new QHBoxLayout(this);
    mainLayout->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(buttonF()));
    connect(&thread, SIGNAL(send(QString)), this, SLOT(accept(QString)));
}

Widget::~Widget()
{
}

void Widget::accept(QString msg)
{
    qDebug()<<"accept"<<msg;
}

void Widget::buttonF()
{
    thread.start();
}

