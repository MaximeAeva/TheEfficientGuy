#include <QtGui>

 #include "stardelegate.h"
 #include "stareditor.h"
 #include "starrating.h"

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

 QWidget *StarDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const

 {
     if ((index.data()).canConvert<StarRating>()) {
         StarEditor *editor = new StarEditor(parent);
         connect(editor, SIGNAL(editingFinished()),
                 this, SLOT(commitAndCloseEditor()));
         return editor;
     } else {
         return QStyledItemDelegate::createEditor(parent, option, index);
     }
 }

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

 void StarDelegate::commitAndCloseEditor()
 {
     StarEditor *editor = qobject_cast<StarEditor *>(sender());
     emit commitData(editor);
     emit closeEditor(editor);
 }
