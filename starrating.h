#ifndef STARRATING_H
#define STARRATING_H

#include <QMetaType>
#include <QPointF>
#include <QVector>
#include <vector>
#include <QPainter>

class StarRating
{
public:
    enum EditMode { Editable, ReadOnly };

    StarRating(std::vector<int> myStarCount = {}, int maxStarCount = 10);

    void paint(QPainter *painter, const QRect &rect,
               const QPalette &palette, EditMode mode) const;
    QSize sizeHint() const;
    int starCount() const { int k=0; for(int i = 0; i < myMaxStarCount; i++) k += myStarCount[i]; return k; }
    int maxStarCount() const { return myMaxStarCount; }
    void setStar(int pos) { myStarCount[pos]+=1; myStarCount[pos]%=2;}
    void setMaxStarCount(int maxStarCount) { myMaxStarCount = maxStarCount; }

private:
    QPolygonF starPolygon;
    QPolygonF diamondPolygon;
    std::vector<int> myStarCount;
    int myMaxStarCount;
};

Q_DECLARE_METATYPE(StarRating)

#endif
