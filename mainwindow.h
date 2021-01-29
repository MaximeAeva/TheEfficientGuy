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
#include <QtCharts>
#include <QLineSeries>
#include <QGraphicsView>
#include <QTableWidget>
#include <QFile>
#include <iostream>
#include <fstream>
#include <cstring>

#include "task.h"
#include "tray.h"
#include "database.h"
#include "target.h"
#include "gantt.h"

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
    void loadHomePage();
    void reloadPage();
    inline void setMon(int val){QSqlQuery *query = new QSqlQuery(db->db); query->prepare("UPDATE parms SET monday=:val"); query->bindValue(":val", val); query->exec();};
    inline void setTue(int val){QSqlQuery *query = new QSqlQuery(db->db); query->prepare("UPDATE parms SET tuesday=:val"); query->bindValue(":val", val); query->exec();};
    inline void setWed(int val){QSqlQuery *query = new QSqlQuery(db->db); query->prepare("UPDATE parms SET wednesday=:val"); query->bindValue(":val", val); query->exec();};
    inline void setThu(int val){QSqlQuery *query = new QSqlQuery(db->db); query->prepare("UPDATE parms SET thursday=:val"); query->bindValue(":val", val); query->exec();};
    inline void setFri(int val){QSqlQuery *query = new QSqlQuery(db->db); query->prepare("UPDATE parms SET friday=:val"); query->bindValue(":val", val); query->exec();};
    inline void setSat(int val){QSqlQuery *query = new QSqlQuery(db->db); query->prepare("UPDATE parms SET saturday=:val"); query->bindValue(":val", val); query->exec();};
    inline void setSun(int val){QSqlQuery *query = new QSqlQuery(db->db); query->prepare("UPDATE parms SET sunday=:val"); query->bindValue(":val", val); query->exec();};
    void refreshSelector(int);
    void designPage();
    void loadArchive();
    void rngGantt();
    void loadPage();
    void prevDb();
    void nextDb();
    void addDb();
    void dlDb();

private:
    Ui::MainWindow *ui;

    void designArchive();
    void designParms();
    void designGanttPage();
    void designConnections();
    void designChargePage();
    void designHomePage();
    void load(tray *t);
    void kill();
    void resizeEvent(QResizeEvent*);

    QLabel *crtDb = new QLabel;
    QPushButton *bef = new QPushButton;
    QPushButton *aft = new QPushButton;
    QPushButton *adder = new QPushButton;
    gantt *g = new gantt(db);
    tray *t1 = new tray("Ideas", 0, db);
    tray *t2 = new tray("ToDo", 1, db);
    tray *t3 = new tray("InProgress", 2, db);
    tray *t4 = new tray("StandBy", 3, db);
    tray *t5 = new tray("Archive", 4, db);

    QVBoxLayout *ganttDisp = new QVBoxLayout;
    QHBoxLayout *b = new QHBoxLayout;
    QVBoxLayout *pageL = new QVBoxLayout;
    QHBoxLayout *col = new QHBoxLayout;
};
#endif // MAINWINDOW_H
