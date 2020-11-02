#include "gantt.h"

gantt::gantt(database *db)
{
    this->db = db;
}

void gantt::build(int row, int col, int dayLength[7])
{
    QStandardItemModel *model = new QStandardItemModel(row, col);
    int day;
    for(int i = 0; i<col; i++)
    {
        day = QDate::currentDate().addDays(i).dayOfWeek();
        switch (day) {
        case 1:
            model->setHeaderData(i, Qt::Horizontal, QObject::tr("Mon"));
            break;
        case 2:
            model->setHeaderData(i, Qt::Horizontal, QObject::tr("Tue"));
            break;
        case 3:
            model->setHeaderData(i, Qt::Horizontal, QObject::tr("Wed"));
            break;
        case 4:
            model->setHeaderData(i, Qt::Horizontal, QObject::tr("Thu"));
            break;
        case 5:
            model->setHeaderData(i, Qt::Horizontal, QObject::tr("Fri"));
            break;
        case 6:
            model->setHeaderData(i, Qt::Horizontal, QObject::tr("Sat"));
            break;
        case 7:
            model->setHeaderData(i, Qt::Horizontal, QObject::tr("Sun"));
            break;
        }

    }
    tableDelegate delegate;
    this->tableView->setItemDelegate(&delegate);
    this->tableView->setModel(model);
    for (int r = 0; r < row; ++r) {
            for (int column = 0; column < col; ++column) {
                QModelIndex index = model->index(r, column, QModelIndex());
                model->setData(index, QVariant((r+1) * (column+1)));
            }
        }
}

