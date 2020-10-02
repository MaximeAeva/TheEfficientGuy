#include "tray.h"

tray::tray(QString title, int id)
{
    this->title = title;
    this->id = id;
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetFloatable);
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetClosable);
    this->setWindowTitle(this->title);
    setStyleSheet( "QWidget{ background-color : rgba( 160, 160, 160, 255); border-radius : 7px;  }" );
    this->content->setLayout(this->layout);
    this->setWidget(this->content);
    this->setAcceptDrops(true);
    this->show();
}

void tray::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}
void tray::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}
void tray::dropEvent(QDropEvent *event)
{
    task *object = static_cast<task *>(event->source());
    this->layout->addWidget(object);
    object->group = this->id;
}
