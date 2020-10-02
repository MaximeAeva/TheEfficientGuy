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
    void addTarget(QDateTime number, bool state, int parentTask);
    void deleteTarget(QDateTime id);

private:
    QSqlDatabase db;
    QString Readconfig(std::string paramName);
    void Model();
};

#endif // DATABASE_H
