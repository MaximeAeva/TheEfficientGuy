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
    void addTask(int number, int priority, int duration, int tray, int itemCount, QDateTime deadline, QString title);
    void deleteTask(int id);
    void addTarget(int number, bool state, int parentTask);
    void deleteTarget(int id);

private:
    QSqlDatabase db;
    QString Readconfig(std::string paramName);
    void Model();
};

#endif // DATABASE_H
