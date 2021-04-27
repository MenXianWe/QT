#include "interport.h"
#include "ui_interport.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //添加数据显示区
    dataArea=new DataArea;
    ui->stackedWidget->addWidget(dataArea);
    //添加图片显示区
    imgArea=new AlarmProtectionImg;
    ui->stackedWidget->addWidget(imgArea);
    //添加数据库区
    sqldata=new SqliteData;
    ui->stackedWidget->addWidget(sqldata);
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    //绑定数据区和图片显示区
    connect(dataArea,SIGNAL(toImgAre(QPixmap)),imgArea,SLOT(showImg(QPixmap)));
    connect(dataArea,SIGNAL(toAlarmImgAre(QPixmap)),this,SLOT(showAlarmImg(QPixmap)));
    connect(dataArea,SIGNAL(toSqlArea(QSqlDatabase)),sqldata,SLOT(showSql(QSqlDatabase)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showAlarmImg(QPixmap pix)
{

    QMessageBox customMsgBox;
    customMsgBox.setWindowTitle(tr("报警图片"));
    QPushButton *yesBtn=customMsgBox.addButton(tr("Yes"),QMessageBox::ActionRole);
    customMsgBox.setText(tr("详细图片"));
    customMsgBox.setIconPixmap(pix);
    customMsgBox.exec();//显示此消息框
}

