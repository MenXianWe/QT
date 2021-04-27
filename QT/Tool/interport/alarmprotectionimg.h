#ifndef ALARMPROTECTIONIMG_H
#define ALARMPROTECTIONIMG_H

#include <QWidget>
#include <QGraphicsScene>
#include <QMessageBox>
namespace Ui {
class AlarmProtectionImg;
}

class AlarmProtectionImg : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmProtectionImg(QWidget *parent = nullptr);
    ~AlarmProtectionImg();
private slots:
    void showImg(QPixmap pix);
private:
    Ui::AlarmProtectionImg *ui;
};

#endif // ALARMPROTECTIONIMG_H
