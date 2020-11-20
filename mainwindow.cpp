#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("The Efficient Guy !");
    this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() +"/icone.ico"));
    designPage();
    designParms();
    designGanttPage();
    designConnections();
    designChargePage();
}

MainWindow::~MainWindow()
{
    db->CloseDB();
    delete ui;
}

void MainWindow::designConnections()
{
    connect(this->adder, SIGNAL(clicked()), this, SLOT(createTask()));
    connect(this->bef, SIGNAL(clicked()), this, SLOT(prevDb()));
    connect(this->aft, SIGNAL(clicked()), this, SLOT(nextDb()));
    connect(ui->spinMonday, SIGNAL(valueChanged(int)), this, SLOT(setMon(int)));
    connect(ui->spinTuesday, SIGNAL(valueChanged(int)), this, SLOT(setTue(int)));
    connect(ui->spinWednesday, SIGNAL(valueChanged(int)), this, SLOT(setWed(int)));
    connect(ui->spinThursday, SIGNAL(valueChanged(int)), this, SLOT(setThu(int)));
    connect(ui->spinFriday, SIGNAL(valueChanged(int)), this, SLOT(setFri(int)));
    connect(ui->spinSaturday, SIGNAL(valueChanged(int)), this, SLOT(setSat(int)));
    connect(ui->spinSunday, SIGNAL(valueChanged(int)), this, SLOT(setSun(int)));
    connect(ui->displayTo, SIGNAL(dateChanged(QDate)), this, SLOT(rngGantt()));
    connect(ui->displayFrom, SIGNAL(dateChanged(QDate)), this, SLOT(rngGantt()));
    connect(ui->displayFrom, SIGNAL(dateChanged(QDate)), this, SLOT(rngGantt()));
    connect(ui->toolBox, SIGNAL(currentChanged(int)), this, SLOT(rngGantt()));
    connect(ui->toolBox, SIGNAL(currentChanged(int)), this, SLOT(loadPage()));
    connect(ui->toolBox, SIGNAL(currentChanged(int)), this, SLOT(reloadPage()));
}

void MainWindow::reloadPage()
{
    QList<tray *> trays = this->findChildren<tray *>();
    foreach(tray* T, trays)
    {

        QList<task *> tasks = T->findChildren<task *>();


        foreach(task* t, tasks)
        {
            QString str1 = "SELECT number, priority, duration, tray, itemCount, "
                        "color, deadline, title FROM task WHERE tray="+QString::fromStdString(std::to_string(T->getId()))+
                 " AND number="+t->wdwId.toString("yyyyMMddhhmmssz")+ " ORDER BY priority DESC";
            QSqlQueryModel *modelTask = new QSqlQueryModel;
            modelTask->setQuery(str1, db->db);
            t->set(QDateTime::fromString(modelTask->record(0).value("number").toString(),"yyyyMMddhhmmssz"),
                       this->db,
                       modelTask->record(0).value("priority").toInt(),
                       modelTask->record(0).value("duration").toInt(),
                       modelTask->record(0).value("tray").toInt(),
                       modelTask->record(0).value("itemCount").toInt(),
                       modelTask->record(0).value("color").toString(),
                       modelTask->record(0).value("deadline").toDateTime(),
                       modelTask->record(0).value("title").toString());
        }
    }
}

void MainWindow::kill()
{
    db->CloseDB();
    QList<tray *> trays = this->findChildren<tray *>();
    foreach(tray* T, trays)
    {

        QList<task *> tasks = T->findChildren<task *>();


        foreach(task* t, tasks)
        {
            t->~task();
        }
    }
}

void MainWindow::nextDb()
{
    QString nameDb = db->nextDb();
    kill();
    this->db = new database(nameDb);
    t1->changeDb(db);
    t2->changeDb(db);
    t3->changeDb(db);
    t4->changeDb(db);
    QList<tray *> trays = this->findChildren<tray *>();
    foreach(tray* T, trays)
    {
        load(T);
    }
    crtDb->setText(nameDb.remove(".db"));
    ganttDisp->removeWidget(this->g->table);
    this->g->changeDb(db);
    rngGantt();
    ganttDisp->addWidget(this->g->table);
    update();
}

void MainWindow::prevDb()
{
    QString nameDb = db->prevDb();
    kill();
    this->db = new database(nameDb);
    t1->changeDb(db);
    t2->changeDb(db);
    t3->changeDb(db);
    t4->changeDb(db);
    QList<tray *> trays = this->findChildren<tray *>();
    foreach(tray* T, trays)
    {
        load(T);
    }
    crtDb->setText(nameDb.remove(".db"));
    ganttDisp->removeWidget(this->g->table);
    this->g->changeDb(db);
    rngGantt();
    ganttDisp->addWidget(this->g->table);
    update();
}

void MainWindow::designPage()
{
    ui->toolBox->setCurrentIndex(0);
    bef->setText("<");
    bef->setMaximumSize(40, 40);
    aft->setText(">");
    aft->setMaximumSize(40, 40);

    crtDb->setStyleSheet("QLabel{font-weight : 600;font-size : 18pt;"
                         "color: rgb(200, 200, 200);}");
    crtDb->setAlignment(Qt::AlignCenter);
    crtDb->setText(db->db.databaseName().remove(".db"));
    crtDb->setMinimumWidth(100);
    crtDb->setMinimumHeight(40);
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
    b->addWidget(bef);
    b->addWidget(crtDb);
    b->addWidget(aft);
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

    col->addWidget(t2);
    col->addSpacing(10);

    col->addWidget(t3);
    col->addSpacing(10);

    col->addWidget(t4);

    pageL->addItem(col);
    ui->page->setLayout(pageL);
}

void MainWindow::createTask()
{
    task *w = new task(db, this);

    t2->layout->addWidget(w);
}

void MainWindow::load(tray *t)
{
    QString str = "SELECT COUNT(*) as cnt FROM task WHERE tray="+QString::fromStdString(std::to_string(t->getId()));
    QSqlQueryModel *modelTaskCount = new QSqlQueryModel;
    modelTaskCount->setQuery(str, db->db);
    QString str1 = "SELECT number, priority, duration, tray, itemCount, "
                   "color, deadline, title FROM task WHERE tray="+QString::fromStdString(std::to_string(t->getId()))+
            " ORDER BY priority DESC";
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
                   modelTask->record(i).value("color").toString(),
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
            //targ->setVisible(true);
            aTask->layout->addWidget(targ);
        }

        aTask->color();
        t->layout->addWidget(aTask);
        //aTask->setVisible(true);
    }
}

void MainWindow::designParms()
{
    QSqlQuery *query = new QSqlQuery(db->db);
    query->exec("SELECT monday, tuesday, wednesday, thursday, friday, saturday, sunday FROM parms");
    query->first();
    ui->spinMonday->setValue(query->value("monday").toInt());
    ui->spinTuesday->setValue(query->value("tuesday").toInt());
    ui->spinWednesday->setValue(query->value("wednesday").toInt());
    ui->spinThursday->setValue(query->value("thursday").toInt());
    ui->spinFriday->setValue(query->value("friday").toInt());
    ui->spinSaturday->setValue(query->value("saturday").toInt());
    ui->spinSunday->setValue(query->value("sunday").toInt());
}

void MainWindow::designGanttPage()
{

    QHBoxLayout *lh = new QHBoxLayout;
    lh->addWidget(ui->displayFrom);
    lh->addWidget(ui->displayTo);
    lh->addStretch(1);
    ui->displayFrom->setDate(QDate::currentDate());
    ui->displayTo->setDate(QDate::currentDate().addDays(14));
    ui->page_2->setLayout(ganttDisp);
    ganttDisp->addItem(lh);
    rngGantt();
    ganttDisp->addWidget(this->g->table);
    //this->g->table->show();
}

void MainWindow::rngGantt()
{
    int dayLength[7];
    QStringList lst;
    QStringList lstNumb;
    QSqlQuery *query = new QSqlQuery(db->db);
    query->exec("SELECT COUNT(*) FROM task");
    query->first();
    QSqlQueryModel *modelTask = new QSqlQueryModel;
    modelTask->setQuery("SELECT * FROM parms", db->db);
    for(int i = 0; i<7; i++)
    {
        dayLength[i] = modelTask->record(0).value(i).toInt();
    }
    QSqlQueryModel *titles = new QSqlQueryModel;
    titles->setQuery("SELECT title as title, number as numb FROM task ORDER BY priority DESC", db->db);
    for(int i = 0; i<query->value(0).toInt(); i++)
    {
        lst << titles->record(i).value("title").toString();
        lstNumb << titles->record(i).value("numb").toString();
    }

    this->g->build(lst, lstNumb, ui->displayFrom->date().daysTo(ui->displayTo->date())+1, dayLength, ui->displayFrom->date());
}

void MainWindow::loadPage()
{
    QLineSeries *lines = new QLineSeries();
    for(int i = 0; i<ui->displayFrom->date().daysTo(ui->displayTo->date()); i++)
        lines->append(QDateTime(ui->displayFrom->date().addDays(i)).toMSecsSinceEpoch(),
                      db->dayOccupation(QDateTime(ui->displayFrom->date().addDays(i))));


    QPen pen = lines->pen();
    pen.setWidth(5);
    pen.setBrush(QBrush(QColor("#F25244")));
    lines->setPen(pen);

    QBrush brush;
    brush.setColor(QColor::fromRgb(49, 54, 63));

    QChart *chart = new QChart();
    chart->addSeries(lines);
    chart->legend()->hide();
    chart->setTitle("Charge in %");
    chart->setBackgroundBrush(brush);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(ui->displayFrom->date().daysTo(ui->displayTo->date()));
    axisX->setFormat("dd MMM");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    lines->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 100);
    axisY->setTickCount(11);
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Amount (%)");
    chart->addAxis(axisY, Qt::AlignLeft);
    lines->attachAxis(axisY);

    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);
}

void MainWindow::designChargePage()
{
    QVBoxLayout *l = new QVBoxLayout;
    l->addWidget(ui->graphicsView);
    ui->page_3->setLayout(l);
    loadPage();
}





