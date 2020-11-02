#ifndef GANTT_H
#define GANTT_H

#include <QDateTime>
#include <QTableWidget>
#include <QHeaderView>
#include <QString>
#include <QObject>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QItemDelegate>

#include <iostream>
#include <QSpinBox>

#include "database.h"
#include "delegate.h"



//####################### ##############################

class gantt
{
public:
    gantt(database *db);
    void build(int row, int col, int dayLength[7]);
    QTableView *tableView = new QTableView;
    QTableWidget *table = new QTableWidget;

private:
    database *db;
};

#endif // GANTT_H
