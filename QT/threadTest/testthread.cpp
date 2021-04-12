#include "testthread.h"

TestThread::TestThread()
{
    Count=0;
}


void TestThread::run()
{
    while(true)
        {
            Count++;
            if(Count>=1000) Count=0;
            qDebug()<<"WorkThread"<<Count;
            sleep(1);//线程 睡眠一秒一次
        }

}
