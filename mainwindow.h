#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <iostream>
#include <iostream>
#include "task.h"
#include "tray.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void createTask();

private:
    Ui::MainWindow *ui;
    void designPage();
    database *db = new database();
    tray *t1 = new tray("Ideas", 0);
    tray *t2 = new tray("ToDo", 1);
    tray *t3 = new tray("InProgress", 2);
    tray *t4 = new tray("StandBy", 3);
    QPushButton *adder = new QPushButton;
};
#endif // MAINWINDOW_H
