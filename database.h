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
    database(QString name);
    void CloseDB();
    QStringList Connections();
    void addTask(QDateTime number, int priority, int duration, int tray, int itemCount, QDateTime deadline, QString title, QString color);
    void deleteTask(QDateTime id);
    void deleteAllocation(QDateTime id);
    void addTarget(QDateTime number, QString title, bool state, QDateTime parentTask);
    void deleteTarget(QDateTime id);
    void updateTask(QDateTime number, int priority, int duration, int tray,
                    int itemCount, QDateTime deadline, QString title, QString color);
    void updateTarget(QDateTime number, bool state);
    void addAllocation(QDateTime task, QDateTime day, int value);
    void deleteAllocation(QDateTime task, QDateTime day, int value);
    void archive(QDateTime task, bool active);
    int isAllocated(QDateTime day, int value);
    int getAlloc(QDateTime task);
    int dayOccupation(QDateTime day);
    int isDead(QDateTime task, QDateTime day);
    int isOverkilled(QDateTime task);
    void updateMiscellaneous(int spentTime);
    int addNote(int x, int y, QString text);
    void updateNote(int id, int x, int y, QString text);
    void deleteNote(int id);
    QStringList getDbNames();
    QSqlDatabase db;
    QString nextDb();
    QString prevDb();

private:

    QStringList dbNames;
    void Model();
    void getStatInfo();
};

#endif // DATABASE_H
