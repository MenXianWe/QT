#ifndef FINALINTERFACE_H
#define FINALINTERFACE_H

#include <QMainWindow>
#include "serialport.h"
#include "interport.h"
QT_BEGIN_NAMESPACE
namespace Ui { class FinalInterFace; }
QT_END_NAMESPACE

class FinalInterFace : public QMainWindow
{
    Q_OBJECT

public:
    FinalInterFace(QWidget *parent = nullptr);
    ~FinalInterFace();

private:
    Ui::FinalInterFace *ui;
    //串口数据
    SerialPort *port;//定义串口界面
    //网络数据
    MainWindow *interport;
};
#endif // FINALINTERFACE_H
