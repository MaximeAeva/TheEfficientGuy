#include "starrating.h"

/**
 * @brief PaintingScaleFactor
 */
const int PaintingScaleFactor = 20;

/**
 * @brief Constructor (form, color, number to paint ...)
 * @param starCount
 * @param maxStarCount
 * @param task
 * @param day
 * @param colorStar
 */
StarRating::StarRating(std::vector<int> starCount, int maxStarCount,
                       QDateTime task, QDateTime day, QColor colorStar, bool isEd)
{
    myStarCount = starCount;
    myMaxStarCount = maxStarCount;
    this->task = task;
    this->day = day;
    this->colorStar = colorStar;
    if(!isEd) isEditable = false;

    starPolygon << QPointF(0.1, 0.0) << QPointF(0.9, 0.0) <<
                   QPointF(0.9, 1.0) << QPointF(0.1, 1.0) << QPointF(0.1, 0.0);

}

/**
 * @brief Resize
 * @return
 */
QSize StarRating::sizeHint() const
{
    return PaintingScaleFactor * QSize(myMaxStarCount, 1);
}

/**
 * @brief Paint it
 * @param painter
 * @param rect
 * @param palette
 * @param mode
 */
void StarRating::paint(QPainter *painter, const QRect &rect,
                       const QPalette, EditMode) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);


    int yOffset = (rect.height() - PaintingScaleFactor) / 2;
    painter->translate(rect.x(), rect.y() + yOffset);
    painter->scale(PaintingScaleFactor, PaintingScaleFactor);
    bool isdeadline = false;

    if(isEditable)
        if(db->isDead(this->task, this->day)<=0) isdeadline = true;

    for (int i = 0; i < myMaxStarCount; ++i) {
        if (myStarCount[i])
        {
            painter->setBrush(this->colorStar);
            painter->drawPolygon(starPolygon, Qt::WindingFill);
        }
        else
        {
            if(isdeadline) painter->setBrush(QColor::fromRgb(30, 30, 30));
            else painter->setBrush(Qt::black);
            painter->drawPolygon(starPolygon, Qt::WindingFill);
        }
        painter->translate(1.0, 0.0);
    }

    painter->restore();
}

StarRating::~StarRating()
{
    this->db = NULL;
    delete db;
}
