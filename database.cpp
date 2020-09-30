#include "database.h"

database::database()
{
        db->addDatabase("QSQLITE");
        db->setHostName(Readconfig("HOST"));
        db->setDatabaseName(Readconfig("NAME"));
        db->setUserName(Readconfig("USER"));
        db->setPassword(Readconfig("PASSWORD"));
        db->open();
        if(!db->isOpen())
        {
            qDebug() << db->lastError();
            qDebug() << QSqlDatabase::drivers();
         }

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
    if(db->isOpen()) db->close();
}

QStringList database::Connections()
{
    return db->connectionNames();
}

void database::Model()
{

}
