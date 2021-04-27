#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dataarea.h"
#include "alarmprotectionimg.h"
#include "sqlitedata.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //数据显示
    DataArea *dataArea;
    //图片显示
    AlarmProtectionImg *imgArea;
    //数据库
    SqliteData *sqldata;

private slots:
    void showAlarmImg(QPixmap pix);
};
#endif // MAINWINDOW_H
