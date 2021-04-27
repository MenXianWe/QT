#include "alarmprotectionimg.h"
#include "ui_alarmprotectionimg.h"

AlarmProtectionImg::AlarmProtectionImg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlarmProtectionImg)
{
    ui->setupUi(this);
}

AlarmProtectionImg::~AlarmProtectionImg()
{
    delete ui;
}

//显示图片
void AlarmProtectionImg::showImg(QPixmap pix)
{

    //创建显示容器
    QGraphicsScene *scene = new QGraphicsScene;
    //向容器中添加文件路径为fileName（QString类型）的文件
     scene->addPixmap(pix);
    //借助graphicsView（QGraphicsView类）控件显示容器的内容
    ui->graphicsView->setScene(scene);
    //开始显示
    ui->graphicsView->show();


}

