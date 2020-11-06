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
    void build(QStringList lst, int col, int dayLength[7]);
    QTableWidget *table = new QTableWidget;
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void getter(const QModelIndex &);

private:
    database *db;
};

#endif // GANTT_H
