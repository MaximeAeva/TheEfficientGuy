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
#include <QMouseEvent>
#include <QCoreApplication>
#include <vector>

#include <iostream>

#include "database.h"
#include "delegate.h"
#include "stardelegate.h"
#include "starrating.h"



//####################### ##############################

class gantt : public QObject
{
    Q_OBJECT
public:
    gantt(database *db);
    void build(QStringList lst, QStringList lstNumb, int col, int dayLength[7], QDate displayFrom);
    QTableWidget *table = new QTableWidget;
    inline void changeDb(database *db){this->db = db;};

private:
    QTableWidgetItem* targetItem;
    database *db;
};

#endif // GANTT_H
