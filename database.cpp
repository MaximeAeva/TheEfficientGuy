#include "database.h"

database::database()
{
        db->addDatabase("QSQLITE", "SQLITE3");
        db->setDatabaseName("C:/Users/maxime.bellot/Documents/build-TheEfficientGuy-Desktop_Qt_5_15_0_MSVC2019_64bit-Debug/debug/a.db.sqlite");
        db->open();
        if(!db->isOpen())
        {
            qDebug() << db->lastError();
            qDebug() << QSqlDatabase::drivers();
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
    if(db->isOpen()) db->close();
}

QStringList database::Connections()
{
    return db->connectionNames();
}

void database::Model()
{

}
