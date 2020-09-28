#include "task.h"

task::task(QWidget *parent) :
    QDockWidget(parent)
{
    ResID++;
    wdwId = ResID;
    setStyleSheet( "QWidget{ background-color : rgba( 160, 160, 160, 255); border-radius : 7px;  }" );
    this->setWindowTitle(title);
    QLabel *lab = new QLabel;
    lab->setText(description);
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetFloatable);
    this->setWidget(lab);
    this->show();
}

task::~task()
{

}

int task::getID()
{
    return wdwId;
}

int task::ResID = 0;
