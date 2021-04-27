#ifndef SQLITEDATA_H
#define SQLITEDATA_H

#include <QWidget>
#include <QSqlRelationalTableModel>
namespace Ui {
class SqliteData;
}

class SqliteData : public QWidget
{
    Q_OBJECT

public:
    explicit SqliteData(QWidget *parent = nullptr);
    ~SqliteData();

private:
    Ui::SqliteData *ui;
    QSqlRelationalTableModel* model;
private slots:
    void showSql(QSqlDatabase db);
};

#endif // SQLITEDATA_H
