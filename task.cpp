#include "task.h"

task::task(QWidget *parent) :
    QDockWidget(parent)
{
    ResID++;
    wdwId = ResID;
    setStyleSheet( "QWidget{ background-color : rgba( 160, 160, 160, 255); border-radius : 7px;  }" );
    this->setWindowTitle(title);
    QLabel *lab = new QLabel;
    lab->setText(description);
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetFloatable);
    this->setWidget(lab);
    this->show();
}

task::~task()
{

}

int task::getID()
{
    return wdwId;
}

int task::ResID = 0;

void task::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();
}

void task::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setData("application/x-item", description.toUtf8());
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void task::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-item"))
        event->acceptProposedAction();
}

void task::dropEvent(QDropEvent *event)
{
    event->acceptProposedAction();
}
