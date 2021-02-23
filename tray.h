#ifndef TRAY_H
#define TRAY_H

#include <QDockWidget>
#include <QString>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QWidget>
#include <QVBoxLayout>
#include "task.h"

class tray: public QDockWidget
{
    Q_OBJECT
public:
    tray(QString title, int id, database *db);
    ~tray();
    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *content = new QWidget;
    inline int getId(){return this->id;};
    inline void changeDb(database *db){this->db = db;};


private:
    QString title;
    database *db;
    int id;
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // TRAY_H
