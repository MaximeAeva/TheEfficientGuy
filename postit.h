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

class postit: public QWidget
{
    Q_OBJECT
public:
    postit(QWidget *parent, QPoint p  = {0, 0});
    ~postit();

public slots:
    void dl();

private:
    QLabel *labelTest = new QLabel;
    QVBoxLayout *layTest = new QVBoxLayout;
    QHBoxLayout *layTest1 = new QHBoxLayout;
    QPushButton *quit = new QPushButton;
    QPoint ref;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *pe) override;


};
#endif // POSTIT_H
