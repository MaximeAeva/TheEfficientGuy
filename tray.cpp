#include "tray.h"

/**
 * @brief Tray's constructor
 * @param title
 * @param id
 * @param db
 */
tray::tray(QString title, int id, database *db)
{
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    this->title = title;
    this->db = db;
    this->id = id;
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetFloatable);
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetClosable);
    this->setWindowTitle(this->title);
    setStyleSheet( "QDockWidget::title{text-align: center; color: rgb(214, 216, 218); background-color : rgba( 24, 26, 31, 255); "
                   "} QWidget{background-color : rgba( 24, 26, 31, 255);border-radius : 7px;  font-weight : 600;font-size : 12pt;}" );
    this->content->setLayout(this->layout);
    this->setWidget(scroll);
    scroll->setWidget(content);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAcceptDrops(true);
}

/**
 * @brief tray::~tray
 */
tray::~tray()
{
    this->db = NULL;
    delete db;
    this->layout = NULL;
    delete layout;
    this->content = NULL;
    delete content;
}

/**
 * @brief DragEvent enter accepted
 * @param event
 */
void tray::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

/**
 * @brief DragEvent leave accepted
 * @param event
 */
void tray::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

/**
 * @brief Drop event management
 * @param event
 */
void tray::dropEvent(QDropEvent *event)
{
    task *object = static_cast<task *>(event->source());
    this->layout->addWidget(object);
    object->group = this->id;
    this->db->updateTask(object->wdwId, object->priority, object->duration, object->group, object->itemCount, object->deadLine, object->title, object->colorTask.name());
}

void tray::resizeEvent(QResizeEvent *)
{
    QList<task *> tasks = this->findChildren<task *>();
    foreach(task* T, tasks)
    {
        T->setMaximumWidth(this->width());
        T->titleBarWidget()->setMaximumWidth(this->width());
    }
}
