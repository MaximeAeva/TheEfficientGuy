#include "target.h"

int target::ResID = 0;

target::target(QString targetDesc)
{
    wdwId = QDateTime::currentDateTime();
    QHBoxLayout *layout = new QHBoxLayout;
    this->setAttribute(Qt::WA_DeleteOnClose);
    b->setText("X");
    b->setStyleSheet("QPushButton{background-color : rgba( 200, 0, 0, 255); border-radius : 3px; height : 1em; width : 1em;}");
    c->setText(targetDesc);
    layout->addWidget(c);
    layout->addStretch(1);
    layout->addWidget(b);
    this->setStyleSheet("background-color :#A6C0C6;border-radius : 7px;border : 1px solid black;");
    this->setLayout(layout);
    this->show();
    connect(b, SIGNAL(clicked()), this, SLOT(close()));
}

void target::rm()
{
}
