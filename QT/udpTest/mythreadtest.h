#ifndef MYTHREADTEST_H
#define MYTHREADTEST_H

#include <QThread>

class myThreadTest : public QThread
{
    Q_OBJECT
public:
    myThreadTest();
};

#endif // MYTHREADTEST_H
