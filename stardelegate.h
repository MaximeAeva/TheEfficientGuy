#ifndef STARDELEGATE_H
#define STARDELEGATE_H

#include <QStyledItemDelegate>
#include <QtGui>
#include "starrating.h"
#include "stareditor.h"

 class StarDelegate : public QStyledItemDelegate
 {
     Q_OBJECT

 public:
     StarDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

     void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
     QSize sizeHint(const QStyleOptionViewItem &option,
                    const QModelIndex &index) const;
     QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;
     void setEditorData(QWidget *editor, const QModelIndex &index) const;
     void setModelData(QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const;
     inline void setAllocInt(int a){this->allocDisp=a;};

 private slots:
     void commitAndCloseEditor();

 private:
     int allocDisp;
 };

 #endif
