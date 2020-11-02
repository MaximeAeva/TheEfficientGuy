#include "delegate.h"

tableDelegate::tableDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *tableDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    /*QStandardItemModel *model = new QStandardItemModel(1, 5);
    QTableView *editor = new QTableView(parent);
    editor->setModel(model);
    return editor;*/
    QSpinBox *editor = new QSpinBox(parent);
        editor->setMinimum(0);
        editor->setMaximum(100);

    return editor;
}

void tableDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    /*int value = index.model()->data(index, Qt::EditRole).toInt();

    QTableView *subTable = static_cast<QTableView*>(editor);*/
    int value = index.model()->data(index, Qt::EditRole).toInt();

        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
}

void tableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    /*QTableView *subTable = static_cast<QTableView*>(editor);

    model->setData(index, 1, Qt::EditRole);*/
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();

        model->setData(index, value, Qt::EditRole);
}

void tableDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

