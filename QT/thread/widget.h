#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include "thread.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void text();
public slots:
    void accept(QString msg);
    void buttonF();
private:
    Thread thread;
    QPushButton *button;
};
#endif // WIDGET_H
