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
    QString description = "Nothing to add";
    QString title = "New task";
};

#endif // TASK_H
