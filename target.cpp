#include "target.h"

target::target()
{
    QHBoxLayout *layout = new QHBoxLayout;
    this->setAttribute(Qt::WA_DeleteOnClose);
    b->setText("X");
    b->setMaximumWidth(20);
    b->setMaximumHeight(20);
    b->setMinimumWidth(20);
    b->setMinimumHeight(20);
    b->setStyleSheet("QPushButton{font-size : 12pt;font-weight : 600;color: black;"
                     "background-color : #F25244; border-radius : 3px; height : 1em; width : 1em;}");
    c->setText(targetDesc);
    c->setMinimumHeight(20);
    layout->addWidget(c);
    //layout->addStretch(1);
    layout->addWidget(b);
    this->setLayout(layout);
    QString q;
    QString col;
    if(c->isChecked()) {q = "#6AE68D"; col = "black";}
    else {q = "rgba( 24, 26, 31, 255)"; col = "rgb(214, 216, 218)";}
    this->setStyleSheet("QWidget{color: "+col+"; background-color : "+q+"; border-radius : 3px; border : none; font-weight : 200;}");
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
    b->setMinimumWidth(20);
    b->setMinimumHeight(20);
    b->setStyleSheet("QPushButton{font-size : 12pt;font-weight : 600;color: black;"
                     "background-color :#F25244; border-radius : 3px; height : 1em; width : 1em;}");
    c->setText(targetDesc);
    c->setMinimumHeight(20);
    layout->addWidget(c);
    //layout->addStretch(1);
    layout->addWidget(b);
    this->setLayout(layout);
    QString q;
    QString col;
    if(c->isChecked()) {q = "#6AE68D"; col = "black";}
    else {q = "rgba( 24, 26, 31, 255)"; col = "rgb(214, 216, 218)";}
    this->setStyleSheet("QWidget{color: "+col+"; background-color : "+q+"; border-radius : 3px; border : none; font-weight : 200;}");
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
    QString q;
    QString col;
    if(c->isChecked()) {q = "#6AE68D"; col = "black";}
    else {q = "rgba( 24, 26, 31, 255)"; col = "rgb(214, 216, 218)";}
    this->setStyleSheet("QWidget{color: "+col+"; background-color : "+q+"; border-radius : 3px; "
                                "font-weight : 200; font-size: 14px; border : none;}");
    connect(c, SIGNAL(stateChanged(int)), this, SLOT(up()));
}

void target::up()
{
    this->db->updateTarget(this->wdwId, this->c->checkState());
    QString q;
    QString col;
    if(c->isChecked()) {q = "#6AE68D"; col = "black";}
    else {q = "rgba( 24, 26, 31, 255)"; col = "rgb(214, 216, 218)";}
    this->setStyleSheet("QWidget{color: "+col+"; background-color : "+q+"; border-radius : 3px; "
                                                "font-weight : 200; font-size: 14px; border : none;}");
}

