#include "target.h"

target::target()
{
    QHBoxLayout *layout = new QHBoxLayout;
    this->setAttribute(Qt::WA_DeleteOnClose);
    b->setText("X");
    b->setStyleSheet("QPushButton{background-color : rgba( 200, 0, 0, 255); border-radius : 3px; height : 1em; width : 1em;}");
    c->setText(targetDesc);
    layout->addWidget(c);
    layout->addStretch(1);
    layout->addWidget(b);
    this->setLayout(layout);
    //this->show();
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
    b->setStyleSheet("QPushButton{background-color : rgba( 200, 0, 0, 255); border-radius : 3px; height : 1em; width : 1em;}");
    c->setText(targetDesc);
    layout->addWidget(c);
    layout->addStretch(1);
    layout->addWidget(b);
    this->setLayout(layout);
    //this->show();
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

