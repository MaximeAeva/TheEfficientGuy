#ifndef STARRATING_H
#define STARRATING_H

#include <QMetaType>
#include <QPointF>
#include <QVector>
#include <vector>
#include <QPainter>
#include <QDateTime>
#include <QtGui>
#include <math.h>

#include "database.h"

class StarRating
{
public:
    enum EditMode { Editable, ReadOnly };

    StarRating(std::vector<int> myStarCount = {}, int maxStarCount = 10,
               QDateTime task = QDateTime::currentDateTime(), QDateTime day = QDateTime::currentDateTime(), QColor star = Qt::red);

    void paint(QPainter *painter, const QRect &rect,
               const QPalette, EditMode) const;
    QSize sizeHint() const;
    int starCount() const { int k=0; for(int i = 0; i < myMaxStarCount; i++) k += myStarCount[i]; return k; }
    inline void setDB(database *db){this->db = db;};
    std::vector<int> getVect() {return myStarCount;};
    int maxStarCount() const { return myMaxStarCount; }
    void setStar(int pos) { myStarCount[pos]+=1; myStarCount[pos]%=2;}
    void setMaxStarCount(int maxStarCount) { myMaxStarCount = maxStarCount; }
    QDateTime task;
    QDateTime day;
    database *db;

private:
    QPolygonF starPolygon;
    QColor colorStar;
    std::vector<int> myStarCount;
    int myMaxStarCount;
};

Q_DECLARE_METATYPE(StarRating)

#endif
