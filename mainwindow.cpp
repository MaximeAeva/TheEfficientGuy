#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    designPage();
    connect(this->adder, SIGNAL(clicked()), this, SLOT(createTask()));
}

MainWindow::~MainWindow()
{
    db->CloseDB();
    delete ui;
}

void MainWindow::designPage()
{
    load(this->t1);
    load(this->t2);
    load(this->t3);
    load(this->t4);
    QHBoxLayout *b = new QHBoxLayout;
    QVBoxLayout *pageL = new QVBoxLayout;
    QHBoxLayout *col = new QHBoxLayout;
    pageL->addItem(b);
    b->addWidget(this->adder);
    b->addStretch(1);
    this->adder->setText("+");
    this->adder->setStyleSheet("QPushButton{"
                                   "font: 87 14pt;"
                                   "border-color: #66767A;"
                                   "color: black;"
                                   "background-color: #9BB3B9;"
                                   "border : 2px solid;"
                                   "border-radius : 15px;"
                                   "height : 1em;"
                                   "width : 1em;}"
                                   "QPushButton:hover{"
                                       "background-color: #66767A;"
                                       "color: #A6C0C6;"
                                       "border-color: #A6C0C6;}"
                                   "QPushButton:pressed{"
                                       "background-color: #A6C0C6;"
                                       "color: #66767A;"
                                       "border-color: #66767A;}");
    col->addWidget(t1);
    col->addSpacing(10);
    t1->setVisible(true);
    col->addWidget(t2);
    col->addSpacing(10);
    t2->setVisible(true);
    col->addWidget(t3);
    col->addSpacing(10);
    t3->setVisible(true);
    col->addWidget(t4);
    t4->setVisible(true);
    pageL->addItem(col);
    ui->page->setLayout(pageL);
}

void MainWindow::createTask()
{
    task *w = new task(db, this);
    w->setVisible(true);
    t2->layout->addWidget(w);
}

void MainWindow::load(tray *t)
{
    QString str = "SELECT COUNT(*) as cnt FROM task WHERE tray="+QString::fromStdString(std::to_string(t->getId()));
    QSqlQueryModel *modelTaskCount = new QSqlQueryModel;
    modelTaskCount->setQuery(str, db->db);
    QString str1 = "SELECT number, priority, duration, tray, itemCount, deadline, title FROM task WHERE tray="+QString::fromStdString(std::to_string(t->getId()));
    QSqlQueryModel *modelTask = new QSqlQueryModel;
    modelTask->setQuery(str1, db->db);
    for(int i = 0; i<modelTaskCount->record(0).value("cnt").toInt(); i++)//through tasks
    {
        task *aTask = new task;
        aTask->set(QDateTime::fromString(modelTask->record(i).value("number").toString(),"yyyyMMddhhmmssz"),
                   this->db,
                   modelTask->record(i).value("priority").toInt(),
                   modelTask->record(i).value("duration").toInt(),
                   modelTask->record(i).value("tray").toInt(),
                   modelTask->record(i).value("itemCount").toInt(),
                   modelTask->record(i).value("deadline").toDateTime(),
                   modelTask->record(i).value("title").toString());
        QString str2 ="SELECT COUNT(*) as cnt1 FROM target WHERE parentTask="+modelTask->record(i).value("number").toString();
        QSqlQueryModel *modelTargetCount = new QSqlQueryModel;
        modelTargetCount->setQuery(str2, db->db);
        QString str3 ="SELECT number, title, state FROM target WHERE parentTask="+modelTask->record(i).value("number").toString();
        QSqlQueryModel *modelTarget = new QSqlQueryModel;
        modelTarget->setQuery(str3, db->db);
        aTask->completion->setMaximum(aTask->itemCount);
        for(int k = 0; k<modelTargetCount->record(0).value("cnt1").toInt(); k++)//through target
        {
            target *targ = new target;
            targ->set(QDateTime::fromString(modelTarget->record(k).value("number").toString(),"yyyyMMddhhmmssz"),
                modelTarget->record(k).value("state").toBool(),
                modelTarget->record(k).value("title").toString(),
                aTask->get(),
                    this->db);
            if(modelTarget->record(k).value("state").toBool())
                aTask->completion->setValue(aTask->completion->value()+1);
            connect(targ->c, SIGNAL(stateChanged(int)), aTask, SLOT(completionVal(int)));
            connect(targ->b, SIGNAL(clicked()), aTask, SLOT(deleteTarget()));
            targ->setVisible(true);
            aTask->layout->addWidget(targ);
        }
        aTask->setVisible(true);
        aTask->color();
        t->layout->addWidget(aTask);
    }
}





