#include "sqlitedata.h"
#include "ui_sqlitedata.h"

SqliteData::SqliteData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SqliteData)
{
    ui->setupUi(this);
}

SqliteData::~SqliteData()
{
    delete ui;
}
void SqliteData::showSql(QSqlDatabase db)
{
    model = new QSqlRelationalTableModel(NULL,db);
    model->setTable("AlarmData");
    model->select();
    ui->tableView->setModel(model);
}
