#include "stardelegate.h"

/**
 * @brief StarDelegate::paint
 * @param painter
 * @param option
 * @param index
 */
void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
 if ((index.data()).canConvert<StarRating>()) {
     StarRating starRating = index.data().value<StarRating>();

     if (option.state & QStyle::State_Selected)
         painter->fillRect(option.rect, option.palette.highlight());

     starRating.paint(painter, option.rect, option.palette,
                      StarRating::ReadOnly);
 } else {
     QStyledItemDelegate::paint(painter, option, index);
 }
}

/**
 * @brief StarDelegate::sizeHint
 * @param option
 * @param index
 * @return
 */
QSize StarDelegate::sizeHint(const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
 if ((index.data()).canConvert<StarRating>()) {
     StarRating starRating = index.data().value<StarRating>();
     return starRating.sizeHint();
 } else {
     return QStyledItemDelegate::sizeHint(option, index);
 }
}

/**
 * @brief StarDelegate::createEditor
 * @param parent
 * @param option
 * @param index
 * @return
 */
QWidget *StarDelegate::createEditor(QWidget *parent,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const

{
 if ((index.data()).canConvert<StarRating>()) {
     StarEditor *editor = new StarEditor(parent);
     editor->setAllocInt(this->allocDisp);
     connect(editor, SIGNAL(editingFinished()),
             this, SLOT(commitAndCloseEditor()));
     return editor;
 } else {
     return QStyledItemDelegate::createEditor(parent, option, index);
 }
}

/**
 * @brief StarDelegate::setEditorData
 * @param editor
 * @param index
 */
void StarDelegate::setEditorData(QWidget *editor,
                              const QModelIndex &index) const
{
 if ((index.data()).canConvert<StarRating>()) {
     StarRating starRating = index.data().value<StarRating>();
     StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
     starEditor->setStarRating(starRating);
 } else {
     QStyledItemDelegate::setEditorData(editor, index);
 }
}

/**
 * @brief StarDelegate::setModelData
 * @param editor
 * @param model
 * @param index
 */
void StarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                             const QModelIndex &index) const
{
 if ((index.data()).canConvert<StarRating>()) {
     StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
     model->setData(index, qVariantFromValue(starEditor->starRating()));
 } else {
     QStyledItemDelegate::setModelData(editor, model, index);
 }
}

/**
 * @brief StarDelegate::commitAndCloseEditor
 */
void StarDelegate::commitAndCloseEditor()
{
 StarEditor *editor = qobject_cast<StarEditor *>(sender());
 emit commitData(editor);
 emit closeEditor(editor);
}
