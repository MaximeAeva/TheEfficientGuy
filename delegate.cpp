#include "delegate.h"

tableDelegate::tableDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *tableDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QStandardItemModel *model = new QStandardItemModel(1, 5);
    QTableView *editor = new QTableView(parent);
    QHeaderView *header = editor->horizontalHeader();
    header->hide();
    QHeaderView *header2 = editor->verticalHeader();
    header2->hide();
    editor->setModel(model);
    for (int column = 0; column < 5; ++column) {
        QModelIndex index = model->index(0, column, QModelIndex());
        model->setData(index, QVariant((column+1)));
        editor->setColumnWidth(column, 20);
    }
    editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    return editor;
}

void tableDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QTableView *subTable = static_cast<QTableView*>(editor);
}

void tableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QTableView *subTable = static_cast<QTableView*>(editor);

    //model->setData(index, 1, Qt::EditRole);
}

void tableDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

