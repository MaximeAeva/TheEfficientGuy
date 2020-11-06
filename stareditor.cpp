#include <QtGui>

 #include "stareditor.h"
 #include "starrating.h"

 StarEditor::StarEditor(QWidget *parent)
     : QWidget(parent)
 {
     setMouseTracking(true);
     setAutoFillBackground(true);
 }

 QSize StarEditor::sizeHint() const
 {
     return myStarRating.sizeHint();
 }

 void StarEditor::paintEvent(QPaintEvent *)
 {
     QPainter painter(this);
     myStarRating.paint(&painter, rect(), this->palette(),
                        StarRating::Editable);
 }

 void StarEditor::mousePressEvent(QMouseEvent *event)
 {
    int star = starAtPosition(event->x());
    myStarRating.setStar(star-1);
    update();
 }

 void StarEditor::mouseDoubleClickEvent(QMouseEvent * /* event */)
 {
     emit editingFinished();
 }

 int StarEditor::starAtPosition(int x)
 {
     int star = (x / (myStarRating.sizeHint().width()
                      / myStarRating.maxStarCount())) + 1;
     if (star <= 0 || star > myStarRating.maxStarCount())
         return -1;

     return star;
 }
