#ifndef POSTIT_H
#define POSTIT_H

#include <QWidget>
#include <QObject>
#include <QDockWidget>
#include <string.h>
#include <QLabel>
#include <QString>
#include <QStyleOption>
#include <QWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QPoint>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QDebug>
#include <QTextEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QSpinBox>
#include <QDateTime>
#include <QColorDialog>
#include <QDateEdit>

#include "database.h"

class postit: public QWidget
{
    Q_OBJECT
public:
    postit(QWidget *parent, database *db, QPoint p  = {0, 0}, QString text = "Note", int id = 0);
    ~postit();

public slots:
    void dl();

private:
    database *db;
    QLabel *labelTest = new QLabel;
    QVBoxLayout *layTest = new QVBoxLayout;
    QHBoxLayout *layTest1 = new QHBoxLayout;
    QPushButton *quit = new QPushButton;
    QPoint ref;
    int id = 0;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *pe) override;


};
#endif // POSTIT_H
