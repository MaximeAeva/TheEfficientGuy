#include <QtGui>

#include "stareditor.h"
#include "starrating.h"

/**
* @brief Constructor
* @param parent
*/
StarEditor::StarEditor(QWidget *parent)
 : QWidget(parent)
{
    setAutoFillBackground(true);
}

/**
 * @brief Resize
 * @return
 */
QSize StarEditor::sizeHint() const
{
    return myStarRating.sizeHint();
}

/**
 * @brief Painter
 */
void StarEditor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    myStarRating.paint(&painter, rect(), this->palette(),
                    StarRating::Editable);
}

/**
 * @brief Catch position
 * @param event
 */
void StarEditor::mousePressEvent(QMouseEvent *event)
{
    this->start = starAtPosition(event->x());
}

/**
 * @brief Catch release and edit
 * @param event
 */
void StarEditor::mouseReleaseEvent(QMouseEvent *event)
{
    int end = starAtPosition(event->x());
    if(end<start)
    {
        int a = start;
        this->start = end;
        end = a;
    }
    for(int star = this->start; star<=end; star++)
    {
        if(myStarRating.getVect().at(star-1))
            myStarRating.setStar(star-1);
        else if(!myStarRating.db->isAllocated(myStarRating.day, star-1))
            myStarRating.setStar(star-1);

        if(myStarRating.getVect().at(star-1))
        {
            myStarRating.db->addAllocation(myStarRating.task, myStarRating.day, star-1);
        }
        else
            myStarRating.db->deleteAllocation(myStarRating.task, myStarRating.day, star-1);
        update();
    }
    emit editingFinished();
}

/**
 * @brief Give item from x position
 * @param x
 * @return
 */
int StarEditor::starAtPosition(int x)
{
    int star = (x / (myStarRating.sizeHint().width()
                  / myStarRating.maxStarCount())) + 1;
    if (star <= 0 || star > myStarRating.maxStarCount())
     return myStarRating.maxStarCount();

    return star;
}
