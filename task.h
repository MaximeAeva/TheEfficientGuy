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

class task: public QDockWidget
{
    Q_OBJECT
public:
    explicit task(QWidget *parent = 0);
    ~task();
    static int ResID;
    int getID();

public slots:
    void completionVal(int i);
    void remove(QPushButton *b);
    void color();

private:
    int wdwId = 0;
    int priority = 1;
    int duration = 0;
    int group = 0;
    int itemCount = 0;
    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *lab = new QWidget;
    QProgressBar *completion = new QProgressBar;
    QString description = "Empty";
    QString title = "New task";
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    QPoint dragStartPosition;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};
#endif // TASK_H
