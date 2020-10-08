#include "gantt.h"

hourTable::hourTable(QObject *parent)
: QItemDelegate(parent)
{
}

QWidget* hourTable::createEditor(QWidget *parent, const   QStyleOptionViewItem &option, const QModelIndex &index)
{
QTableWidget *Ht = new QTableWidget(parent);
Ht->setRowCount(1);
Ht->setColumnCount(8);
Ht->show();
return Ht;
}

void hourTable::setEditorData(QWidget *editor, const QModelIndex &index)
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QTableWidget *spinBox = static_cast<QTableWidget*>(editor);
}

void hourTable::setModelData(QWidget *editor, QAbstractItemModel *model,   const QModelIndex &index)
{
    QTableWidget *spinBox = static_cast<QTableWidget*>(editor);
    int value = index.model()->data(index, Qt::EditRole).toInt();
         model->setData(index, value, Qt::EditRole);
}

void hourTable::updateEditorGeometry(QWidget *editor, const     QStyleOptionViewItem &option, const QModelIndex &index)
{
    editor->setGeometry(option.rect);
}


gantt::gantt(database *db)
{
    this->db = db;
    QStandardItemModel model(1, 1);
         QTableView tableView;
         tableView.setModel(&model);

         hourTable delegate;
         tableView.setItemDelegate(&delegate);
    table->show();
    hourTable t;
    table->setItemDelegate(&t);
}

void gantt::setSize(int row, int col)
{
    this->table->setRowCount(row);
    this->table->setColumnCount(col);
}

void gantt::adaptDisplay()
{
    for(int i = 0; i<this->table->columnCount(); i++)
    {
        this->table->setColumnWidth(i, std::round(this->table->columnCount()/this->table->width()));
    }
}
