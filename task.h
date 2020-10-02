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
    explicit task(database *db, QWidget *parent);
    ~task();
    int group = 0;

public slots:
    void completionVal(int i);
    void deleteTarget();
    void color();

private:
    QDateTime wdwId;
    int priority = 0;
    int duration = 0;
    int itemCount = 0;
    QDateTime deadLine = QDateTime::currentDateTime();
    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *lab = new QWidget;
    QProgressBar *completion = new QProgressBar;
    QString description = "Empty";
    QString title = "New task";
    void mouseDoubleClickEvent(QMouseEvent *event, database *db);
    QPoint dragStartPosition;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};
#endif // TASK_H
