#include "tray.h"

tray::tray(QString title, int id, database *db)
{
    this->title = title;
    this->db = db;
    this->id = id;
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetFloatable);
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetClosable);
    this->setWindowTitle(this->title);
    setStyleSheet( "QWidget{ color: rgb(214, 216, 218); background-color : rgba( 24, 26, 31, 255); border-radius : 7px;  }" );
    this->content->setLayout(this->layout);
    this->setWidget(this->content);
    this->setAcceptDrops(true);
    //this->show();
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
    this->db->updateTask(object->wdwId, object->priority, object->duration, object->group, object->itemCount, object->deadLine, object->title, object->colorTask.name());
}
