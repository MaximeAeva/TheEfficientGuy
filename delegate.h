#ifndef DELEGATE_H
#define DELEGATE_H

#include <QDateTime>
#include <QTableView>
#include <QHeaderView>
#include <QString>
#include <QObject>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QItemDelegate>

#include <iostream>

class tableDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:

    using QStyledItemDelegate::QStyledItemDelegate;

    tableDelegate(int size, int w, QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const;

public slots:
    void update(const QModelIndex &);

private:
    int size;
    int w;
};
#endif // DELEGATE_H
