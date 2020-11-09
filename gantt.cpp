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
}

void gantt::build(QStringList lst, QStringList lstNumb, int col, int dayLength[7])
{
    int row = lst.length();
    QStringList headerH;
    QStringList headerV;
    this->table->setRowCount(row);
    this->table->setColumnCount(col);
    int day;
    for(int i = 0; i<row; i++)
    {
        std::string s = lst.at(i).toStdString();
        const char *a = s.c_str();
        headerV << a;
    }
    for(int i = 0; i<col; i++)
    {
        std::string s = QDate::currentDate().addDays(i).toString("dd-MM-yyyy").toStdString();
        std::string k;
        day = QDate::currentDate().addDays(i).dayOfWeek();
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

        da = QDate::currentDate().addDays(i).dayOfWeek();
        if(this->table->width()/(col+1)<(20*dayLength[da-1])) w = 20*dayLength[da-1];
        if (dayLength[da-1] == 0) w = 20;
        this->table->setItemDelegate(new StarDelegate);

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
                        " AND day="+QDateTime::currentDateTime().addDays(i).toString("yyyyMMdd");

                ct->setQuery(st, db->db);
                QSqlQueryModel *mod = new QSqlQueryModel;
                QString str = "SELECT value as val FROM allocation WHERE parentTask="
                        +QDateTime::fromString(lstNumb[j],"yyyyMMddhhmmssz").toString("yyyyMMddhhmmssz")+
                        " AND day="+QDateTime::currentDateTime().addDays(i).toString("yyyyMMdd");

                mod->setQuery(str, db->db);
                QColor starColor = QColor(colo->record(0).value("cl").toString());
                qDebug() << starColor;
                if(ct->record(0).value("ct").toInt())
                    for(int l = 0; l<ct->record(0).value("ct").toInt(); l++)
                        if(mod->record(l).value("val").toInt()<dayLength[da-1])
                            vect[mod->record(l).value("val").toInt()]++;
                QTableWidgetItem *item = new QTableWidgetItem;
                StarRating sR(vect, dayLength[da-1],
                        QDateTime::fromString(lstNumb[j],"yyyyMMddhhmmssz") , QDateTime::currentDateTime().addDays(i), starColor);
                sR.setDB(this->db);
                item->setData(0, QVariant::fromValue(sR));
                table->setItem(j, i, item);
            }
        }
        this->table->setColumnWidth(i, w);
        this->table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    }
}

bool gantt::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    QPoint p = mouseEvent->pos();
    this->table->openPersistentEditor(this->table->itemAt(p));
    QModelIndex ind = this->table->indexAt(mouseEvent->pos());
    if(this->table->indexAt(mouseEvent->pos()) != ind)
    {
        this->table->closePersistentEditor(this->table->itemAt(p));
    }
  }
  return false;
}


void gantt::getter(const QModelIndex &index)
{
    int col = this->table->currentIndex().column();
    int row = this->table->currentIndex().row();

    std::cout << col << ", " << row << ": " << index.column() << std::endl;
}


