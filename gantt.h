#ifndef GANTT_H
#define GANTT_H

#include <QDateTime>
#include <QTableWidget>
#include <QHeaderView>
#include <QItemDelegate>
#include <QStandardItemModel>

#include "database.h"

class hourTable : public QItemDelegate
{
    Q_OBJECT
public:
    explicit hourTable(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index);
    void setEditorData(QWidget *editor, const QModelIndex &index);
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index);
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,  const QModelIndex &index);
};

class gantt
{
public:
    gantt(database *db);
    void setSize(int row, int col);
    void adaptDisplay();
    QTableWidget *table = new QTableWidget;

private:
    database *db;
};

#endif // GANTT_H
