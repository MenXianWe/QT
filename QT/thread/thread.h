#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QDebug>
class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();
    void stop();
protected:
    void run();
signals:
    void send(QString msg);
};

#endif // THREAD_H
