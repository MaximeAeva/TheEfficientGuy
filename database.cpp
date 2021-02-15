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
                   "archive INT DEFAULT 0, "
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

    QSqlQuery *query4 = new QSqlQuery(db);
    query4->exec("CREATE TABLE IF NOT EXISTS miscellaneous"
                   "("
                   "id DATE PRIMARY KEY, "
                   "avgTask0 REAL DEFAULT 0.0, "
                   "avgTask1 REAL DEFAULT 0.0, "
                   "avgTask2 REAL DEFAULT 0.0, "
                   "avgTask3 REAL DEFAULT 0.0, "
                   "avgTask4 REAL DEFAULT 0.0, "
                   "avgTask5 REAL DEFAULT 0.0, "
                   "activeTask INT DEFAULT 0, "
                   "activeTask0 INT DEFAULT 0, "
                   "activeTask1 INT DEFAULT 0, "
                   "activeTask2 INT DEFAULT 0, "
                   "activeTask3 INT DEFAULT 0, "
                   "activeTask4 INT DEFAULT 0, "
                   "activeTask5 INT DEFAULT 0, "
                   "estimedTime0 INT DEFAULT 0, "
                   "estimedTime1 INT DEFAULT 0, "
                   "estimedTime2 INT DEFAULT 0, "
                   "estimedTime3 INT DEFAULT 0, "
                   "estimedTime4 INT DEFAULT 0, "
                   "estimedTime5 INT DEFAULT 0, "
                   "allocated0 INT DEFAULT 0, "
                   "allocated1 INT DEFAULT 0, "
                   "allocated2 INT DEFAULT 0, "
                   "allocated3 INT DEFAULT 0, "
                   "allocated4 INT DEFAULT 0, "
                   "allocated5 INT DEFAULT 0, "
                   "spentTime INT DEFAULT 0"
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
    if(db.isOpen()){
    int archive = task.daysTo(QDateTime::currentDateTime());
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("UPDATE task "
                   "SET  active=:active, archive=:archive "
                   "WHERE number=:number");
    query->bindValue(":active", active);
    query->bindValue(":archive", archive);
    query->bindValue(":number", task.toString("yyyyMMddhhmmssz"));
    query->exec();
    }
}

void database::updateMiscellaneous(int spentTime, float avgTask, int activeTask)
{
    if(db.isOpen()){
        QSqlQuery *cnt = new QSqlQuery(db);
        QString str1 = "SELECT COUNT(*) FROM miscellaneous";
        cnt->exec(str1);
        cnt->first();
        if(!cnt->value(0).toInt())//add the first line
        {
            QSqlQuery *query1 = new QSqlQuery(db);
            query1->prepare("INSERT INTO miscellaneous(id, spentTime) "
                           "VALUES (:id, :spentTime)");
            query1->bindValue(":id", QDate::currentDate());
            query1->bindValue(":spentTime", spentTime);
            query1->exec();
        }
        else
        {
            QSqlQuery *query = new QSqlQuery(db);
            QString str = "SELECT id FROM miscellaneous ORDER BY id DESC LIMIT 1";
            query->exec(str);
            query->first();

            if(QDate::currentDate().daysTo(query->value(0).toDate()))//Add a new line
            {
                QSqlQuery *query1 = new QSqlQuery(db);
                query1->prepare("INSERT INTO miscellaneous(id, spentTime) "
                               "VALUES (:id, :spentTime)");
                query1->bindValue(":id", QDate::currentDate());
                query1->bindValue(":spentTime", spentTime);
                query1->exec();
            } else//Update a new line
            {
                QSqlQuery *t = new QSqlQuery(db);
                QString str = "SELECT spentTime FROM miscellaneous ORDER BY id DESC LIMIT 1";
                t->exec(str);
                t->first();
                spentTime += t->value(0).toInt();
                QSqlQuery *query1 = new QSqlQuery(db);
                query1->prepare("UPDATE miscellaneous "
                               "SET  spentTime=:spentTime "
                               "WHERE id=:id");
                query1->bindValue(":spentTime", spentTime);
                query1->bindValue(":id", QDate::currentDate());
                query1->exec();
            }
        }
        getStatInfo();
    }
}

void database::getStatInfo()
{

    //Average number of target per task per priority
    QSqlQueryModel *avgTask = new QSqlQueryModel();
    QString str = "SELECT AVG(items) as avg, priority as p "
                  "FROM(SELECT itemCount as items, priority FROM task ORDER BY ROWID DESC LIMIT 20) "
                  "GROUP BY priority ORDER BY priority ASC";
    avgTask->setQuery(str, db);

    for(int i = 0; i<avgTask->rowCount(); i++)
    {
        QSqlQuery *query1 = new QSqlQuery(db);
        switch(avgTask->record(i).value("p").toInt())
        {
            case 0:
                query1->prepare("UPDATE miscellaneous SET avgTask0=:val WHERE id=:id");
            break;
            case 1:
                query1->prepare("UPDATE miscellaneous SET avgTask1=:val WHERE id=:id");
            break;
            case 2:
                query1->prepare("UPDATE miscellaneous SET avgTask2=:val WHERE id=:id");
            break;
            case 3:
                query1->prepare("UPDATE miscellaneous SET avgTask3=:val WHERE id=:id");
            break;
            case 4:
                query1->prepare("UPDATE miscellaneous SET avgTask4=:val WHERE id=:id");
            break;
            case 5:
                query1->prepare("UPDATE miscellaneous SET avgTask5=:val WHERE id=:id");
            break;
        }
        query1->bindValue(":val", avgTask->record(i).value("avg").toReal());
        query1->bindValue(":id", QDate::currentDate());
        query1->exec();
    }

    //Number of current active task
    QSqlQuery *activeTask = new QSqlQuery(db);
    QString str1 = "SELECT COUNT(*) FROM task WHERE active=1";
    activeTask->exec(str1);
    activeTask->first();

    //Number of active task per priority
    QSqlQueryModel *activeTask1 = new QSqlQueryModel();
    QString str2 = "SELECT priority as p, COUNT(*) as cnt "
                   "FROM(SELECT number, priority FROM task WHERE active=1 ORDER BY ROWID DESC) "
                   "GROUP BY priority ORDER BY priority ASC";
    activeTask1->setQuery(str2, db);

    for(int i = 0; i<activeTask1->rowCount(); i++)
    {
        QSqlQuery *query1 = new QSqlQuery(db);
        switch(activeTask1->record(i).value("p").toInt())
        {
            case 0:
                query1->prepare("UPDATE miscellaneous SET activeTask0=:val WHERE id=:id");
            break;
            case 1:
                query1->prepare("UPDATE miscellaneous SET activeTask1=:val WHERE id=:id");
            break;
            case 2:
                query1->prepare("UPDATE miscellaneous SET activeTask2=:val WHERE id=:id");
            break;
            case 3:
                query1->prepare("UPDATE miscellaneous SET activeTask3=:val WHERE id=:id");
            break;
            case 4:
                query1->prepare("UPDATE miscellaneous SET activeTask4=:val WHERE id=:id");
            break;
            case 5:
                query1->prepare("UPDATE miscellaneous SET activeTask5=:val WHERE id=:id");
            break;
        }
        query1->bindValue(":val", activeTask1->record(i).value("cnt").toInt());
        query1->bindValue(":id", QDate::currentDate());
        query1->exec();
    }

    //Average processing time per priority
    QSqlQueryModel *estimedTime = new QSqlQueryModel();
    QString str3 = "SELECT priority as p, AVG(archiveTime) as ar "
                   "FROM(SELECT archive as archiveTime, priority FROM task WHERE active=0 ORDER BY ROWID DESC LIMIT 20) "
                   "GROUP BY priority ORDER BY priority ASC";
    estimedTime->setQuery(str3, db);

    for(int i = 0; i<estimedTime->rowCount(); i++)
    {
        QSqlQuery *query1 = new QSqlQuery(db);
        switch(estimedTime->record(i).value("p").toInt())
        {
            case 0:
                query1->prepare("UPDATE miscellaneous SET estimedTime0=:val WHERE id=:id");
            break;
            case 1:
                query1->prepare("UPDATE miscellaneous SET estimedTime1=:val WHERE id=:id");
            break;
            case 2:
                query1->prepare("UPDATE miscellaneous SET estimedTime2=:val WHERE id=:id");
            break;
            case 3:
                query1->prepare("UPDATE miscellaneous SET estimedTime3=:val WHERE id=:id");
            break;
            case 4:
                query1->prepare("UPDATE miscellaneous SET estimedTime4=:val WHERE id=:id");
            break;
            case 5:
                query1->prepare("UPDATE miscellaneous SET estimedTime5=:val WHERE id=:id");
            break;
        }
        query1->bindValue(":val", estimedTime->record(i).value("ar").toInt());
        query1->bindValue(":id", QDate::currentDate());
        query1->exec();
    }

    //Average allocation per priority
    QSqlQueryModel *allocated = new QSqlQueryModel();
    QString str4 = "SELECT AVG(ct) as avg, p as p FROM (SELECT COUNT(*) as ct, t.priority as p FROM allocation "
                   "JOIN task as t ON parentTask=t.number WHERE t.active = 0 GROUP BY parentTask LIMIT 10) GROUP BY p ORDER BY p ASC";
    allocated->setQuery(str4, db);

    for(int i = 0; i<allocated->rowCount(); i++)
    {
        QSqlQuery *query1 = new QSqlQuery(db);
        switch(allocated->record(i).value("p").toInt())
        {
            case 0:
                query1->prepare("UPDATE miscellaneous SET allocated0=:val WHERE id=:id");
            break;
            case 1:
                query1->prepare("UPDATE miscellaneous SET allocated1=:val WHERE id=:id");
            break;
            case 2:
                query1->prepare("UPDATE miscellaneous SET allocated2=:val WHERE id=:id");
            break;
            case 3:
                query1->prepare("UPDATE miscellaneous SET allocated3=:val WHERE id=:id");
            break;
            case 4:
                query1->prepare("UPDATE miscellaneous SET allocated4=:val WHERE id=:id");
            break;
            case 5:
                query1->prepare("UPDATE miscellaneous SET allocated5=:val WHERE id=:id");
            break;
        }
        query1->bindValue(":val", allocated->record(i).value("avg").toInt());
        query1->bindValue(":id", QDate::currentDate());
        query1->exec();
    }

}
