#include "target.h"

/**
 * @brief Create an empty target in a task
 */
target::target()
{
    QHBoxLayout *layout = new QHBoxLayout;
    this->setAttribute(Qt::WA_DeleteOnClose);
    b->setCheckable(true);
    b->setText("X");
    b->setMaximumWidth(20);
    b->setMaximumHeight(20);
    b->setMinimumWidth(20);
    b->setMinimumHeight(20);
    b->setChecked(true);
    b->setStyleSheet("QPushButton{font-size : 12pt;font-weight : 600;color: black;"
                     "background-color : #F25244; border-radius : 3px; height : 1em; width : 1em;}");
    QString str = targetDesc;
    str.truncate(c->width());
    c->setText(str);
    c->setMinimumHeight(20);
    layout->addWidget(c);

    layout->addWidget(b);
    this->setLayout(layout);
    QString q;
    QString col;
    if(c->isChecked()) {q = "#6AE68D"; col = "black";}
    else {q = "rgba( 24, 26, 31, 255)"; col = "rgb(214, 216, 218)";}
    this->setStyleSheet("QWidget{color: "+col+"; background-color : "+q+"; border-radius : 3px; border : none; font-weight : 200;}");
    connect(b, SIGNAL(clicked()), this, SLOT(close()));
}

/**
 * @brief Create a filled target
 * @param targetDesc
 * @param db
 * @param parentTime
 */
target::target(QString targetDesc, database *db, QDateTime parentTime)
{

    wdwId = QDateTime::currentDateTime();
    this->db = db;
    this->db->addTarget(this->wdwId, targetDesc, this->c->isChecked(), parentTime);
    QHBoxLayout *layout = new QHBoxLayout;
    this->setAttribute(Qt::WA_DeleteOnClose);
    b->setCheckable(true);
    b->setText("X");
    b->setChecked(true);
    b->setMaximumWidth(20);
    b->setMaximumHeight(20);
    b->setMinimumWidth(20);
    b->setMinimumHeight(20);
    b->setStyleSheet("QPushButton{font-size : 12pt;font-weight : 600;color: black;"
                     "background-color :#F25244; border-radius : 3px; height : 1em; width : 1em;}");
    QString str = targetDesc;
    str.truncate(c->width());
    c->setText(str);
    c->setMinimumHeight(20);
    layout->addWidget(c);

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

/**
 * @brief target::~target
 */
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

/**
 * @brief Set hint to a target
 * @param id
 * @param check
 * @param title
 * @param parent
 * @param db
 */
void target::set(QDateTime id, bool check, QString title, QDateTime parent, database *db)
{
    this->wdwId = id;
    this->parent = parent;
    this->c->setChecked(check);
    this->b->setChecked(!check);
    this->targetDesc = title;
    this->db = db;
    QString str = targetDesc;
    str.truncate(c->width());
    c->setText(str);
    QString q;
    QString col;
    if(c->isChecked()) {q = "#6AE68D"; col = "black";}
    else {q = "rgba( 24, 26, 31, 255)"; col = "rgb(214, 216, 218)";}
    this->setStyleSheet("QWidget{color: "+col+"; background-color : "+q+"; border-radius : 3px; "
                                "font-weight : 200; font-size: 14px; border : none;}");
    connect(c, SIGNAL(stateChanged(int)), this, SLOT(up()));
}

/**
 * @brief update target (mostly color)
 */
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
