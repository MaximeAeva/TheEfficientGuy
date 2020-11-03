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

#include "database.h"
#include "delegate.h"



//####################### ##############################

class gantt : public QObject
{
    Q_OBJECT
public:
    gantt(database *db);
    void build(QStringList lst, int col, int dayLength[7]);
    QTableView *tableView = new QTableView;
    QTableWidget *table = new QTableWidget;

public slots:
    void getter(const QModelIndex &);

private:
    database *db;
};

#endif // GANTT_H
