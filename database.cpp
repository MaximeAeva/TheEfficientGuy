#include "database.h"

database::database()
{
        db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE3");
        db.setDatabaseName(Readconfig("NAME"));
        if(!db.open())
        {
            qDebug() << db.isOpen();
            qDebug() << db.lastError().databaseText();
            qDebug() << db.lastError().driverText();
            qDebug() << QSqlDatabase::isDriverAvailable("QSQLITE");
         }
        else
        {
            qDebug() << "Connected";
            Model();
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
    if(db.isOpen()) db.close();
}

QStringList database::Connections()
{
    return db.connectionNames();
}

void database::Model()
{
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("CREATE TABLE IF NOT EXISTS task"
                   "("
                   "number DATETIME PRIMARY KEY NOT NULL, "
                   "priority INT, "
                   "duration INT, "
                   "tray INT, "
                   "itemCount INT, "
                   "deadline DATETIME, "
                   "title VARCHAR(255)"
                   ")");

    QSqlQuery *query1 = new QSqlQuery(db);
    query1->exec("CREATE TABLE IF NOT EXISTS target"
                   "("
                   "number DATETIME PRIMARY KEY NOT NULL, "
                   "title VARCHAR(255), "
                   "state BOOL, "
                   "parentTask INT, "
                   "FOREIGN KEY(parentTask) REFERENCES task(number)"
                   ")");
}

void database::addTask(QDateTime number, int priority, int duration, int tray, int itemCount, QDateTime deadline, QString title)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("INSERT INTO task(number, priority, duration, tray, itemCount, deadline, title) "
                   "VALUES (:number, :priority, :duration, :tray, :itemCount, :deadline, :title)");
    query->bindValue(":number", number);
    query->bindValue(":priority", priority);
    query->bindValue(":duration", duration);
    query->bindValue(":tray", tray);
    query->bindValue(":itemCount", itemCount);
    query->bindValue(":deadline", deadline);
    query->bindValue(":title", title);
    query->exec();
}

void database::deleteTask(QDateTime id)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("DELETE FROM task "
                   "WHERE number=:id");
    query->bindValue(":id", id);
    query->exec();
}

void database::addTarget(QDateTime number, QString title, bool state, QDateTime parentTask)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("INSERT INTO target(number, title, state, parentTask) "
                   "VALUES (:number, :title, :state, :parentTask)");
    query->bindValue(":number", number);
    query->bindValue(":title", title);
    query->bindValue(":state", state);
    query->bindValue(":parentTask", parentTask);
    query->exec();
}

void database::deleteTarget(QDateTime id)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("DELETE FROM target "
                   "WHERE number=:id");
    query->bindValue(":id", id);
    query->exec();
}

void database::updateTask(QDateTime number, int priority, int duration, int tray, int itemCount, QDateTime deadline, QString title)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("UPDATE task "
                   "SET priority=:priority, duration=:duration, tray=:tray, itemCount=:itemCount, deadline=:deadline, title=:title "
                   "WHERE number=:number");
    query->bindValue(":priority", priority);
    query->bindValue(":duration", duration);
    query->bindValue(":tray", tray);
    query->bindValue(":itemCount", itemCount);
    query->bindValue(":deadline", deadline);
    query->bindValue(":title", title);
    query->bindValue(":number", number);
    query->exec();
}

void database::updateTarget(QDateTime number, bool state)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("UPDATE target "
                   "SET state=:state "
                   "WHERE number=:number");
    query->bindValue(":state", state);
    query->bindValue(":number", number);
    query->exec();
}


