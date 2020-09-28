#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QObject>
#include <QDockWidget>
#include <string.h>
#include <QLabel>
#include <QString>
#include <QWindow>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPoint>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

class task: public QDockWidget
{
    Q_OBJECT
public:
    explicit task(QWidget *parent = 0);
    ~task();
    static int ResID;
    int getID();

private:
    int wdwId = 0;
    int priority = 0;
    int group = 0;
    QString description = "Empty";
    QString title = "New task";
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    QPoint dragStartPosition;
};
#endif // TASK_H
