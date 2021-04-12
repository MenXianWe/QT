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

private:
    Ui::Widget *ui;
    QTcpServer *sever;
    QTcpSocket *socket;
private slots:
    void answer();
    void readInfor();
    void on_sendPushButton_clicked();
    void on_disconnectPushButton_clicked();
};
#endif // WIDGET_H
