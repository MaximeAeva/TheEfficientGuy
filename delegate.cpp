#include "delegate.h"

tableDelegate::tableDelegate(int size, int w, QObject *parent)
    : QItemDelegate(parent)
{
    this->size = size;
    this->w = w;
}


QWidget *tableDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QStandardItemModel *model = new QStandardItemModel(1, size);
    QTableView *editor = new QTableView(parent);
    QHeaderView *header = editor->horizontalHeader();
    header->hide();
    QHeaderView *header2 = editor->verticalHeader();
    header2->hide();
    editor->setModel(model);
    for (int column = 0; column < size; ++column) {
        editor->setColumnWidth(column, w/(size));
    }
    if(!size)
        editor->setStyleSheet("QTableView { background-color: grey; selection-color: black; }");
    else
        editor->setStyleSheet("QTableView { background-color: #222326; selection-color: black; }");
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

