#include "target.h"

target::target()
{
    QHBoxLayout *layout = new QHBoxLayout;
    this->setAttribute(Qt::WA_DeleteOnClose);
    b->setText("X");
    b->setMaximumWidth(20);
    b->setMaximumHeight(20);
    b->setStyleSheet("QPushButton{font-size : 12pt;font-weight : 600;color: black;"
                     "background-color : #F25244; border-radius : 3px; height : 1em; width : 1em;}");
    c->setText(targetDesc);
    c->setMinimumHeight(20);
    layout->addWidget(c);
    //layout->addStretch(1);
    layout->addWidget(b);
    this->setLayout(layout);
    this->setStyleSheet("QWidget{color: rgb(214, 216, 218); background-color : rgba( 24, 26, 31, 255); border-radius : 3px; border : none;}");
    connect(b, SIGNAL(clicked()), this, SLOT(close()));
}

target::target(QString targetDesc, database *db, QDateTime parentTime)
{

    wdwId = QDateTime::currentDateTime();
    this->db = db;
    this->db->addTarget(this->wdwId, targetDesc, this->c->isChecked(), parentTime);
    QHBoxLayout *layout = new QHBoxLayout;
    this->setAttribute(Qt::WA_DeleteOnClose);
    b->setText("X");
    b->setMaximumWidth(20);
    b->setMaximumHeight(20);
    b->setStyleSheet("QPushButton{font-size : 12pt;font-weight : 600;color: black;"
                     "background-color :#F25244; border-radius : 3px; height : 1em; width : 1em;}");
    c->setText(targetDesc);
    c->setMinimumHeight(20);
    layout->addWidget(c);
    //layout->addStretch(1);
    layout->addWidget(b);
    this->setLayout(layout);
    this->setStyleSheet("QWidget{color: rgb(214, 216, 218); background-color : rgba( 24, 26, 31, 255); border-radius : 3px; border : none;}");
    connect(c, SIGNAL(stateChanged(int)), this, SLOT(up()));
    connect(b, SIGNAL(clicked()), this, SLOT(close()));
}

target::~target()
{
    this->db->deleteTarget(this->wdwId);
    this->db = NULL;
    delete this->db;
    this->c = NULL;
    delete this->c;
    this->b = NULL;
    delete this->b;
}

void target::set(QDateTime id, bool check, QString title, QDateTime parent, database *db)
{
    this->wdwId = id;
    this->parent = parent;
    this->c->setChecked(check);
    this->targetDesc = title;
    this->db = db;
    c->setText(targetDesc);
    connect(c, SIGNAL(stateChanged(int)), this, SLOT(up()));
}

void target::up()
{
    this->db->updateTarget(this->wdwId, this->c->checkState());
}

