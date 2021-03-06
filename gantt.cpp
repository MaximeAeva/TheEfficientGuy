#include "gantt.h"

/**
 * @brief Create a Gantt object
 * @param db
 */
gantt::gantt(database *db)
{
    this->db = db;
    this->table->setAlternatingRowColors(true);
    this->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->table->verticalHeader ()->setDefaultSectionSize(20);
    QCoreApplication::instance()->installEventFilter(this);
    this->targetItem = this->table->item(1, 0);
    this->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->table->setStyleSheet("QHeaderView::section{background-color: rgb(24, 26, 31); "
                               "color : rgb(214,216,218);} "
                               "QTableWidget {gridline-color: rgb(170,170,170); "
                               "alternate-background-color: rgb(47, 51, 61);background-color: rgb(24, 26, 31);}");
}

/**
* @brief gantt::~gantt
*/
gantt::~gantt()
{
    this->db = NULL;
    delete db;
    this->table = NULL;
    this->targetItem = NULL;
    delete this->table;
    delete this->targetItem;
}

/**
 * @brief Build Gantt (item delegate painting)
 * @param lst
 * @param lstNumb
 * @param col
 * @param dayLength
 * @param displayFrom
 */
void gantt::build(QStringList lst, QStringList lstNumb, int col, int dayLength[7], QDate displayFrom, bool editMode)
{
    int row = lst.length();
    QSqlQuery *optt = new QSqlQuery(this->db->db);
    optt->exec("SELECT opt1 FROM parms");
    optt->first();
    int opt = optt->value("opt1").toInt();
    QStringList headerH;
    QStringList headerV;
    this->table->setRowCount(row+1);
    this->table->setColumnCount(col+1);
    int day;
    headerV << "Availability";
    for(int i = 0; i<row; i++)
    {
        std::string s = lst.at(i).toStdString();
        const char *a = s.c_str();
        headerV << a;
    }
    headerH << "Completion";
    for(int i = 0; i<col; i++)
    {
        std::string s = displayFrom.addDays(i).toString("dd-MM-yyyy").toStdString();
        std::string k;
        day = displayFrom.addDays(i).dayOfWeek();
        switch (day) {
        case 1:
            k = "Mon "+s;
            break;
        case 2:
            k = "Tue "+s;
            break;
        case 3:
            k = "Wed "+s;
            break;
        case 4:
            k = "Thu "+s;
            break;
        case 5:
            k = "Fri "+s;
            break;
        case 6:
            k = "Sat "+s;
            break;
        case 7:
            k = "Sun "+s;
            break;
        }
        const char *a = k.c_str();
        headerH << a;
    }

    this->table->setHorizontalHeaderLabels(headerH);
    this->table->setVerticalHeaderLabels(headerV);


    int da;
    int w = this->table->width()/(col+1);
    this->table->setColumnWidth(0, 1*20);
    //Completion
    for(int j = 0; j<row; j++)
    {
        QSqlQueryModel *q = new QSqlQueryModel;
        QString stcol = "SELECT duration as d FROM task WHERE number="
                +QDateTime::fromString(lstNumb[j],"yyyyMMddhhmmssz").toString("yyyyMMddhhmmssz");

        q->setQuery(stcol, db->db);
        float ratio = float(db->getAlloc(QDateTime::fromString(lstNumb[j],"yyyyMMddhhmmssz")))/float(q->record(0).value("d").toInt());
        int ratio1 = floor(5*ratio);
        if(ratio1>5) ratio1 = 5;
        std::vector<int> vect = {};
        vect.push_back(ratio1);
        QTableWidgetItem *item = new QTableWidgetItem;
        StarRating sR(vect, 1,
                 QDateTime::currentDateTime(), QDateTime::currentDateTime(), Qt::red, false);
        sR.setDB(this->db);
        item->setData(0, QVariant::fromValue(sR));
        table->setItem(j+1, 0, item);
    }
    for(int i = 0; i<col; i++)
    {

        da = displayFrom.addDays(i).dayOfWeek();
        if(this->table->width()/(col+1)<(20*dayLength[da-1])) w = 20*dayLength[da-1];
        if (dayLength[da-1] == 0) w = 20;
        this->table->setItemDelegate(new StarDelegate);
        StarDelegate *sD = static_cast<StarDelegate *>(this->table->itemDelegate());
        sD->setAllocInt(opt);
        std::vector<int> vect = {};
        //Allocation count
        if(opt)
        {
            for(int k = 0; k < dayLength[da-1]; k++)
            {
                int val = floor(5*db->isAllocated(QDateTime(displayFrom.addDays(i)), k)/opt);
                if(val>5) val = 5;
                vect.push_back(val);
            }
        }
        else
        {
            for(int k = 0; k < dayLength[da-1]; k++)
            {
                if(db->isAllocated(QDateTime(displayFrom.addDays(i)), k))
                    vect.push_back(2);
                else
                    vect.push_back(0);
            }
        }
        QTableWidgetItem *item = new QTableWidgetItem;
        StarRating sR(vect, dayLength[da-1],
                 QDateTime::currentDateTime(), QDateTime::currentDateTime(), Qt::transparent, false);
        sR.setDB(this->db);
        item->setData(0, QVariant::fromValue(sR));
        table->setItem(0, i+1, item);
        for(int j = 0; j<row; j++)
        {
            if(dayLength[da-1])
            {
                std::vector<int> vect = {};
                for(int k = 0; k < dayLength[da-1]; k++) vect.push_back(0);
                QSqlQueryModel *colo = new QSqlQueryModel;
                QString stcol = "SELECT color as cl FROM task WHERE number="
                        +QDateTime::fromString(lstNumb[j],"yyyyMMddhhmmssz").toString("yyyyMMddhhmmssz");

                colo->setQuery(stcol, db->db);
                QSqlQueryModel *ct = new QSqlQueryModel;
                QString st = "SELECT COUNT(*) as ct FROM allocation WHERE parentTask="
                        +QDateTime::fromString(lstNumb[j],"yyyyMMddhhmmssz").toString("yyyyMMddhhmmssz")+
                        " AND day="+displayFrom.addDays(i).toString("yyyyMMdd");

                ct->setQuery(st, db->db);
                QSqlQueryModel *mod = new QSqlQueryModel;
                QString str = "SELECT value as val FROM allocation WHERE parentTask="
                        +QDateTime::fromString(lstNumb[j],"yyyyMMddhhmmssz").toString("yyyyMMddhhmmssz")+
                        " AND day="+displayFrom.addDays(i).toString("yyyyMMdd");

                mod->setQuery(str, db->db);
                QColor starColor = QColor(colo->record(0).value("cl").toString());
                if(ct->record(0).value("ct").toInt())
                    for(int l = 0; l<ct->record(0).value("ct").toInt(); l++)
                        if(mod->record(l).value("val").toInt()<dayLength[da-1])
                            vect[mod->record(l).value("val").toInt()]++;
                QTableWidgetItem *item = new QTableWidgetItem;
                StarRating sR(vect, dayLength[da-1],
                        QDateTime::fromString(lstNumb[j],"yyyyMMddhhmmssz") , QDateTime(displayFrom.addDays(i)), starColor);
                sR.setDB(this->db);
                item->setData(0, QVariant::fromValue(sR));
                table->setItem(j+1, i+1, item);
                this->targetItem = this->table->item(j+1, i+1);
                if(editMode)
                    this->table->openPersistentEditor(targetItem);
                else
                    this->table->closePersistentEditor(targetItem);

            }
        }
        this->table->setColumnWidth(i+1, w);
        this->table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    }
    this->table->update();
}

