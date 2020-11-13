#include "gantt.h"

gantt::gantt(database *db)
{
    this->db = db;
    connect(table, SIGNAL(pressed(const QModelIndex &)), this, SLOT(getter(const QModelIndex &)));
    this->table->setMouseTracking(true);
    this->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->table->verticalHeader ()->setDefaultSectionSize(20);
    QCoreApplication::instance()->installEventFilter(this);
    QPoint p(1, 0);
    this->targetItem = this->table->itemAt(p);
}

void gantt::build(QStringList lst, QStringList lstNumb, int col, int dayLength[7], QDate displayFrom)
{
    int row = lst.length();
    QStringList headerH;
    QStringList headerV;
    this->table->setRowCount(row+1);
    this->table->setColumnCount(col);
    int day;
    headerV << "Availability";
    for(int i = 0; i<row; i++)
    {
        std::string s = lst.at(i).toStdString();
        const char *a = s.c_str();
        headerV << a;
    }
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

    for(int i = 0; i<col; i++)
    {

        da = displayFrom.addDays(i).dayOfWeek();
        if(this->table->width()/(col+1)<(20*dayLength[da-1])) w = 20*dayLength[da-1];
        if (dayLength[da-1] == 0) w = 20;
        this->table->setItemDelegate(new StarDelegate);
        std::vector<int> vect = {};
        for(int k = 0; k < dayLength[da-1]; k++)
        {
            if(db->isAllocated(QDateTime(displayFrom.addDays(i)), k)) vect.push_back(1);
            else vect.push_back(0);
        }
        QTableWidgetItem *item = new QTableWidgetItem;
        StarRating sR(vect, dayLength[da-1],
                 QDateTime::currentDateTime(), QDateTime::currentDateTime(), Qt::transparent);
        sR.setDB(this->db);
        item->setData(0, QVariant::fromValue(sR));
        table->setItem(0, i, item);
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
                table->setItem(j+1, i, item);
            }
        }
        this->table->setColumnWidth(i, w);
        this->table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        connect(this->table, SIGNAL(cellEntered(int, int)), this, SLOT(openPers(int, int)));
    }
}

void gantt::getter(const QModelIndex &index)
{
    int col = this->table->currentIndex().column();
    int row = this->table->currentIndex().row();
}

void gantt::openPers(int row, int col)
{
    this->table->closePersistentEditor(targetItem);
    QPoint p(row, col);
    this->targetItem = this->table->itemAt(p);
    this->table->openPersistentEditor(targetItem);
}
