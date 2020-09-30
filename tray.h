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

class tray: public QDockWidget
{
    Q_OBJECT
public:
    tray(QString title);
    QVBoxLayout *layout = new QVBoxLayout;

private:
    QString title;
    QWidget *content = new QWidget;

    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // TRAY_H
