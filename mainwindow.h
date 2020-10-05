#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <iostream>
#include <iostream>
#include <cstring>

#include "task.h"
#include "tray.h"
#include "database.h"
#include "target.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    database *db = new database();

public slots:
    void createTask();

private:
    Ui::MainWindow *ui;
    void designPage();
    void load(tray *t);
    QPushButton *adder = new QPushButton;
    tray *t1 = new tray("Ideas", 0);
    tray *t2 = new tray("ToDo", 1);
    tray *t3 = new tray("InProgress", 2);
    tray *t4 = new tray("StandBy", 3);
};
#endif // MAINWINDOW_H
