#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <iostream>
#include "task.h"
#include "tray.h"

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
    tray *t1 = new tray("Ideas");
    tray *t2 = new tray("ToDo");
    tray *t3 = new tray("InProgress");
    tray *t4 = new tray("StandBy");
    QPushButton *adder = new QPushButton;
};
#endif // MAINWINDOW_H
