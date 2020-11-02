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
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QPoint>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QSpinBox>
#include <QDateTime>

#include "target.h"
#include "database.h"

class task: public QDockWidget
{
    Q_OBJECT
public:
    task();
    explicit task(database *db, QWidget *parent = 0);
    ~task();
    int group = 1;
    void set(QDateTime number, database *db, int priority = 0, int duration = 1, int tray = 1, int itemCount = 0, QDateTime deadline = QDateTime::currentDateTime(), QString title = "");
    QVBoxLayout *layout = new QVBoxLayout;
    QProgressBar *completion = new QProgressBar;
    inline QDateTime get(){return this->wdwId;};
    int itemCount = 0;
    int priority = 0;
    int duration = 1;
    QDateTime wdwId;
    QDateTime deadLine = QDateTime::currentDateTime();
    QString title = "New task";

public slots:
    void completionVal(int i);
    void deleteTarget();
    void color();

private:

    database *db;


    QWidget *lab = new QWidget;
    QString description = "Empty";

    void mouseDoubleClickEvent(QMouseEvent *event) override;
    QPoint dragStartPosition;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void dropEvent(QDropEvent *event) override;

};
#endif // TASK_H
