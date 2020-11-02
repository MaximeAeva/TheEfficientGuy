#include "gantt.h"

gantt::gantt(database *db)
{
    this->db = db;
}

void gantt::build(int row, int col, int dayLength[7])
{
    QStandardItemModel *model = new QStandardItemModel(row, col);
    std::string day;
    for(int i = 0; i<col; i++)
    {
        day = QDate::currentDate().addDays(i).toString("ddd").toStdString();

        if(day == "lun.") model->setHeaderData(i, Qt::Horizontal, QObject::tr("Mon"));
        else if(day == "mar.") model->setHeaderData(i, Qt::Horizontal, QObject::tr("Tue"));
        else if(day == "mer.") model->setHeaderData(i, Qt::Horizontal, QObject::tr("Wed"));
        else if(day == "jeu.") model->setHeaderData(i, Qt::Horizontal, QObject::tr("Thu"));
        else if(day == "ven.") model->setHeaderData(i, Qt::Horizontal, QObject::tr("Fri"));
        else if(day == "sam.") model->setHeaderData(i, Qt::Horizontal, QObject::tr("Sat"));
        else if(day == "dim.") model->setHeaderData(i, Qt::Horizontal, QObject::tr("Sun"));
    }
    this->tableView->setModel(model);
}

