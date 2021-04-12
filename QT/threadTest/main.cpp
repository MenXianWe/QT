#include "widget.h"
#include "testthread.h"

#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();
    TestThread *task = new TestThread; // (3) 创建一个 WorkThread 对象
       qDebug()<<"线程测试开始:";
       task->start();//启动该线程 (4)
       qDebug()<<"线程测试结束:";
    return a.exec();
}
