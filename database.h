#ifndef DATABASE_H
#define DATABASE_H


#include <QSqlDatabase>
#include <QDebug>
#include <QString>
#include <QSqlError>
#include <cstring>
#include <fstream>
#include <vector>

class database
{
public:
    database();
    void CloseDB();
    QStringList Connections();

private:
    QSqlDatabase *db = new QSqlDatabase;
    QString Readconfig(std::string paramName);
    void Model();
};

#endif // DATABASE_H
