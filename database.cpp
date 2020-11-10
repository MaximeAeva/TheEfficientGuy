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
                   "color VARCHAR(10), "
                   "deadline DATETIME, "
                   "title VARCHAR(255)"
                   ")");

    QSqlQuery *query1 = new QSqlQuery(db);
    query1->exec("CREATE TABLE IF NOT EXISTS target"
                   "("
                   "number DATETIME PRIMARY KEY NOT NULL, "
                   "title VARCHAR(255), "
                   "state BOOL, "
                   "parentTask DATETIME, "
                   "FOREIGN KEY(parentTask) REFERENCES task(number)"
                   ")");

    QSqlQuery *query2 = new QSqlQuery(db);
    query2->exec("CREATE TABLE IF NOT EXISTS parms"
                   "("
                   "monday INT, "
                   "tuesday INT, "
                   "wednesday INT, "
                   "thursday INT, "
                   "friday INT, "
                   "saturday INT, "
                   "sunday INT "
                   ")");

    QSqlQuery *query3 = new QSqlQuery(db);
    query3->exec("CREATE TABLE IF NOT EXISTS allocation"
                   "("
                   "number INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "value INTEGER, "
                   "day DATETIME, "
                   "parentTask DATETIME, "
                   "FOREIGN KEY(parentTask) REFERENCES task(number)"
                   ")");

    query->exec("SELECT COUNT(*) FROM parms");
    query->first();
    if(!query->value(0).toInt())
        query->exec("INSERT INTO parms(monday, tuesday, wednesday, thursday, friday, saturday, sunday) VALUES (0, 0, 0, 0, 0, 0, 0)");
}

void database::addTask(QDateTime number, int priority, int duration, int tray, int itemCount, QDateTime deadline, QString title, QString color)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("INSERT INTO task(number, priority, duration, tray, itemCount, color, deadline, title) "
                   "VALUES (:number, :priority, :duration, :tray, :itemCount, :color, :deadline, :title)");
    query->bindValue(":number", number.toString("yyyyMMddhhmmssz"));
    query->bindValue(":priority", priority);
    query->bindValue(":duration", duration);
    query->bindValue(":tray", tray);
    query->bindValue(":itemCount", itemCount);
    query->bindValue(":color", color);
    query->bindValue(":deadline", deadline);
    query->bindValue(":title", title);
    query->exec();
}

void database::deleteTask(QDateTime id)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("DELETE FROM task WHERE number="+id.toString("yyyyMMddhhmmssz"));
    deleteTarget(id);
    deleteAllocation(id);
}

void database::deleteAllocation(QDateTime id)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("DELETE FROM allocation WHERE parentTask="+id.toString("yyyyMMddhhmmssz"));
}

void database::addTarget(QDateTime number, QString title, bool state, QDateTime parentTask)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("INSERT INTO target(number, title, state, parentTask) "
                   "VALUES (:number, :title, :state, :parentTask)");
    query->bindValue(":number", number.toString("yyyyMMddhhmmssz"));
    query->bindValue(":title", title);
    query->bindValue(":state", state);
    query->bindValue(":parentTask", parentTask.toString("yyyyMMddhhmmssz"));
    query->exec();
}

void database::deleteTarget(QDateTime id)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("DELETE FROM target WHERE number="+id.toString("yyyyMMddhhmmssz"));
}

void database::updateTask(QDateTime number, int priority, int duration, int tray, int itemCount, QDateTime deadline, QString title, QString color)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("UPDATE task "
                   "SET priority=:priority, duration=:duration, tray=:tray, itemCount=:itemCount, color=:color, deadline=:deadline, title=:title "
                   "WHERE number=:number");
    query->bindValue(":priority", priority);
    query->bindValue(":duration", duration);
    query->bindValue(":tray", tray);
    query->bindValue(":itemCount", itemCount);
    query->bindValue(":color", color);
    query->bindValue(":deadline", deadline);
    query->bindValue(":title", title);
    query->bindValue(":number", number.toString("yyyyMMddhhmmssz"));
    query->exec();
}

void database::updateTarget(QDateTime number, bool state)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("UPDATE target "
                   "SET state=:state "
                   "WHERE number=:number");
    query->bindValue(":state", state);
    query->bindValue(":number", number.toString("yyyyMMddhhmmssz"));
    query->exec();
}

void database::addAllocation(QDateTime task, QDateTime day, int value)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("INSERT INTO allocation(value, day, parentTask) "
                   "VALUES (:value, :day, :parentTask)");
    query->bindValue(":value", value);
    query->bindValue(":day", day.toString("yyyyMMdd"));
    query->bindValue(":parentTask", task.toString("yyyyMMddhhmmssz"));
    query->exec();
}

void database::deleteAllocation(QDateTime task, QDateTime day, int value)
{
    QSqlQuery *query = new QSqlQuery(db);
    QString str = "DELETE FROM allocation WHERE parentTask="+task.toString("yyyyMMddhhmmssz")+
            " AND day="+day.toString("yyyyMMdd")+" AND value="+QString::fromStdString(std::to_string(value));
    query->exec(str);
}

bool database::isAllocated(QDateTime day, int value)
{
    QSqlQuery *query = new QSqlQuery(db);
    QString str = "SELECT COUNT(*) FROM allocation WHERE day="+day.toString("yyyyMMdd")+" AND value="+QString::fromStdString(std::to_string(value));
    query->exec(str);
    query->first();
    return query->value(0).toBool();
}

int database::getAlloc(QDateTime task)
{
    QSqlQuery *query = new QSqlQuery(db);
    QString str = "SELECT COUNT(*) FROM allocation WHERE parentTask="+task.toString("yyyyMMddhhmmssz");
    query->exec(str);
    query->first();
    return query->value(0).toInt();
}



