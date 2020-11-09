#ifndef DATABASE_H
#define DATABASE_H


#include <QSqlDatabase>
#include <QDebug>
#include <QString>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QObject>
#include <QSqlQuery>
#include <QDir>
#include <QDateTime>
#include <QCoreApplication>
#include <QWidget>
#include <cstring>
#include <fstream>
#include <vector>


class database: public QObject
{
public:
    database();
    void CloseDB();
    QStringList Connections();
    void addTask(QDateTime number, int priority, int duration, int tray, int itemCount, QDateTime deadline, QString title);
    void deleteTask(QDateTime id);
    void addTarget(QDateTime number, QString title, bool state, QDateTime parentTask);
    void deleteTarget(QDateTime id);
    void updateTask(QDateTime number, int priority, int duration, int tray, int itemCount, QDateTime deadline, QString title);
    void updateTarget(QDateTime number, bool state);
    void addAllocation(QDateTime task, QDateTime day, int value);
    void deleteAllocation(QDateTime task, QDateTime day, int value);
    QSqlDatabase db;

private:

    QString Readconfig(std::string paramName);
    void Model();
};

#endif // DATABASE_H
