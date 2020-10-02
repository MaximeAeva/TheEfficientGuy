#include "database.h"

database::database()
{
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(Readconfig("NAME"));
        if(!db.open())
        {
            qDebug() << db.isOpen();
            qDebug() << db.lastError().databaseText();
            qDebug() << db.lastError().driverText();
            qDebug() << QSqlDatabase::isDriverAvailable("QSQLITE");
         }
        else qDebug() << "Connected";

}

QString database::Readconfig(std::string paramName)
{
    std::ifstream cfg("config.txt");
    if(!cfg.is_open())
    {
        return 0;
    }
    std::string parm, value;
    while (cfg >> parm >> value)
    {
        if(parm== paramName){return QString::fromStdString(value);}
    }
    return 0;
}

void database::CloseDB()
{
    if(db.isOpen()) db.close();
}

QStringList database::Connections()
{
    return db.connectionNames();
}

void database::Model()
{

}
