#include "database.h"

/**
 * @brief Link first database on range
 */
database::database()
{
        if(db.isOpen()) db.close();
        this->dbNames = getDbNames();
        if(dbNames.isEmpty()) dbNames.append("Default");
        db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE3");
        db.setDatabaseName(dbNames.first());
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

/**
 * @brief Link a specified database
 * @param name
 */
database::database(QString name)
{ 
        if(db.isOpen()) db.close();
        db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE3");
        db.setDatabaseName(name);
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

/**
 * @brief Get Db names in folder
 * @return
 */
QStringList database::getDbNames()
{
    QDir directory(QDir::currentPath());
    QStringList dbNames = directory.entryList(QStringList() << "*.db" << "*.db",QDir::Files);
    return dbNames;
}

/**
 * @brief Get the next dbName
 * @return
 */
QString database::nextDb()
{
    dbNames = getDbNames();
    if(dbNames.isEmpty()) dbNames.append("Default");
    if(dbNames.length()>1)
    {
        int i = 0;
        while(i < dbNames.length())
        {
            if(dbNames.at(i)==db.databaseName()) return dbNames.at((i+1)%dbNames.length());
            i++;
        }
        return dbNames.at(0);
    }
    else return dbNames.at(0);
}

/**
 * @brief Get the previous dBName
 * @return
 */
QString database::prevDb()
{
    dbNames = getDbNames();
    if(dbNames.isEmpty()) dbNames.append("Default");
    if(dbNames.length()>1)
    {
        int i = dbNames.length()-1;
        while(i > 0)
        {
            if(dbNames.at(i)==db.databaseName()) return dbNames.at(abs((i-1)%dbNames.length()));
            i--;
        }
        return dbNames.at(dbNames.length()-1);
    }
    else return dbNames.at(0);
}

/**
 * @brief Close current dB
 */
void database::CloseDB()
{
    if(db.isOpen()) db.close();
}

/**
 * @brief Design DB on load
 */
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
                   "active BOOL DEFAULT 1, "
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

/**
 * @brief add a Task to current db
 * @param number
 * @param priority
 * @param duration
 * @param tray
 * @param itemCount
 * @param deadline
 * @param title
 * @param color
 */
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

/**
 * @brief Delete a task on the current dB
 * @param id
 */
void database::deleteTask(QDateTime id)
{
    if(db.isOpen()){
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("DELETE FROM task WHERE number="+id.toString("yyyyMMddhhmmssz"));
    deleteTarget(id);
    deleteAllocation(id);
    }
}

/**
 * @brief Remove all allocated hours from a task
 * @param id
 */
void database::deleteAllocation(QDateTime id)
{
    if(db.isOpen()){
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("DELETE FROM allocation WHERE parentTask="+id.toString("yyyyMMddhhmmssz"));
    }
}

/**
 * @brief Add a target to a task
 * @param number
 * @param title
 * @param state
 * @param parentTask
 */
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

/**
 * @brief Remove a target from a task
 * @param id
 */
void database::deleteTarget(QDateTime id)
{
    if(db.isOpen()){
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("DELETE FROM target WHERE number="+id.toString("yyyyMMddhhmmssz"));
    }
}

/**
 * @brief Update fields from a task
 * @param number
 * @param priority
 * @param duration
 * @param tray
 * @param itemCount
 * @param deadline
 * @param title
 * @param color
 */
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

/**
 * @brief Store check state
 * @param number
 * @param state
 */
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

/**
 * @brief Allocate an hour to a task
 * @param task
 * @param day
 * @param value
 */
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

/**
 * @brief delete an allocated hour from a task
 * @param task
 * @param day
 * @param value
 */
void database::deleteAllocation(QDateTime task, QDateTime day, int value)
{
    if(db.isOpen()){
    QSqlQuery *query = new QSqlQuery(db);
    QString str = "DELETE FROM allocation WHERE parentTask="+task.toString("yyyyMMddhhmmssz")+
            " AND day="+day.toString("yyyyMMdd")+" AND value="+QString::fromStdString(std::to_string(value));
    query->exec(str);
    }
}

/**
 * @brief Return true if this hour is allocated, false otherwise
 * @param day
 * @param value
 * @return
 */
bool database::isAllocated(QDateTime day, int value)
{
    QSqlQuery *query = new QSqlQuery(db);
    QString str = "SELECT COUNT(*) FROM allocation WHERE day="+day.toString("yyyyMMdd")+" AND value="+QString::fromStdString(std::to_string(value));
    query->exec(str);
    query->first();
    return query->value(0).toBool();
}

/**
 * @brief Get the number of allocated hours from a task
 * @param task
 * @return
 */
int database::getAlloc(QDateTime task)
{
    QSqlQuery *query = new QSqlQuery(db);
    QString str = "SELECT COUNT(*) FROM allocation WHERE parentTask="+task.toString("yyyyMMddhhmmssz");
    query->exec(str);
    query->first();
    return query->value(0).toInt();
}

/**
 * @brief return the daylength from the specified day
 * @param day
 * @return
 */
int database::dayOccupation(QDateTime day)
{
    QString d;
    switch(day.date().dayOfWeek())
    {
        case 1:
            d = "monday";
        break;
        case 2:
            d = "tuesday";
        break;
        case 3:
            d = "wednesday";
        break;
        case 4:
            d = "thursday";
        break;
        case 5:
            d = "friday";
        break;
        case 6:
            d = "saturday";
        break;
        case 7:
            d = "sunday";
        break;
    }

    QSqlQuery *query = new QSqlQuery(db);
    QString str = "SELECT 100*(SELECT COUNT(*) FROM allocation WHERE day="+day.date().toString("yyyyMMdd")+
            ") / (SELECT "+d+" FROM parms)";
    query->exec(str);
    query->first();
    return query->value(0).toInt();
}

/**
 * @brief Return if the deadline is over
 * @param task
 * @param day
 * @return
 */
int database::isDead(QDateTime task, QDateTime day)
{
    if(db.isOpen()){
    QSqlQuery *query = new QSqlQuery(db);
    QString str = "SELECT deadline as dl FROM task WHERE number="+task.toString("yyyyMMddhhmmssz");
    query->exec(str);
    query->first();
    QDateTime dl = query->value("dl").toDateTime();
    return day.date().daysTo(dl.date());
    }
    return 0;
}

/**
 * @brief Return if allocated time is over
 * @param task
 * @return
 */
int database::isOverkilled(QDateTime task)
{
    if(db.isOpen()){
    int al = getAlloc(task);
    QSqlQuery *query = new QSqlQuery(db);
    QString str = "SELECT duration FROM task WHERE number="+task.toString("yyyyMMddhhmmssz");
    query->exec(str);
    query->first();
    return al - query->value(0).toInt();
    }
    return 0;
}

/**
 * @brief Send a task to archive
 * @param task
 * @param active
 */
void database::archive(QDateTime task, bool active)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("UPDATE task "
                   "SET  active=:active "
                   "WHERE number=:number");
    query->bindValue(":active", active);
    query->bindValue(":number", task.toString("yyyyMMddhhmmssz"));
    query->exec();
}
