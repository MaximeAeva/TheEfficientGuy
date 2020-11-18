#include "starrating.h"

const int PaintingScaleFactor = 20;

StarRating::StarRating(std::vector<int> starCount, int maxStarCount, QDateTime task, QDateTime day, QColor colorStar)
{
    myStarCount = starCount;
    myMaxStarCount = maxStarCount;
    this->task = task;
    this->day = day;
    this->colorStar = colorStar;

    starPolygon << QPointF(0.1, 0.0) << QPointF(0.9, 0.0) <<
                   QPointF(0.9, 1.0) << QPointF(0.1, 1.0) << QPointF(0.1, 0.0);

}

QSize StarRating::sizeHint() const
{
    return PaintingScaleFactor * QSize(myMaxStarCount, 1);
}

void StarRating::paint(QPainter *painter, const QRect &rect,
                       const QPalette &palette, EditMode mode) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);


    int yOffset = (rect.height() - PaintingScaleFactor) / 2;
    painter->translate(rect.x(), rect.y() + yOffset);
    painter->scale(PaintingScaleFactor, PaintingScaleFactor);
    bool isdeadline = false;

    if(this->colorStar != Qt::transparent)
        if(db->isDead(this->task, this->day)<=0) isdeadline = true;

    for (int i = 0; i < myMaxStarCount; ++i) {
        if (myStarCount[i])
        {
            painter->setBrush(this->colorStar);
            painter->drawPolygon(starPolygon, Qt::WindingFill);
        }
        else
        {
            if(isdeadline) painter->setBrush(QColor::fromRgb(36, 41, 46));
            else painter->setBrush(Qt::black);
            painter->drawPolygon(starPolygon, Qt::WindingFill);
        }
        painter->translate(1.0, 0.0);
    }

    painter->restore();
}
