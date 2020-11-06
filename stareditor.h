#ifndef STAREDITOR_H
#define STAREDITOR_H

#include <QWidget>

#include "starrating.h"

class StarEditor : public QWidget
{
    Q_OBJECT

public:
    StarEditor(QWidget *parent = 0);

    QSize sizeHint() const;
    void setStarRating(const StarRating &starRating) {
        myStarRating = starRating;
    }
    StarRating starRating() { return myStarRating; }

signals:
    void editingFinished();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    int starAtPosition(int x);

    StarRating myStarRating;
};

#endif
