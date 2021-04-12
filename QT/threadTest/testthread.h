#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QThread>
#include <QDebug>
class TestThread : public QObject
{
    Q_OBJECT
public:
    explicit TestThread(QObject *parent = nullptr);
//    ~TestThread();
    void myTimeout();
    void setFlag(bool flag=true);
signals:
    void mySignal();
private:
    bool isStop;
};

#endif // TESTTHREAD_H
