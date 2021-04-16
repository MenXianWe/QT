#include "thread.h"

Thread::Thread()
{

}

void Thread::run()
{
    qDebug()<<__FUNCTION__<<QThread::currentThreadId();
    emit send(QString("run thread"));
}
