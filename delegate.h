#ifndef DELEGATE_H
#define DELEGATE_H

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

class tableDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    tableDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
#endif // DELEGATE_H
