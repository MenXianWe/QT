#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void answer();
    void on_connectPushButton_clicked();
    void readInfor();
    void on_sendPushButton_clicked();

    void on_disConnectPushButton_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *socket;
};
#endif // WIDGET_H
