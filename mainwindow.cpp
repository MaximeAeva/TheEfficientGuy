#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    this->setWindowTitle("The Efficient Guy !");
    this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() +"/icone.ico"));
    designPage();
    designParms();
    designGanttPage();
    designConnections();
    designArchive();
    designChargePage();
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    db->CloseDB();
    delete ui;
}

/**
 * @brief Edit links between buttons and actions
 */
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
    connect(ui->toolBox, SIGNAL(currentChanged(int)), this, SLOT(refreshSelector(int)));
    connect(ui->addDb, SIGNAL(clicked()), this, SLOT(addDb()));
    connect(ui->dlDb, SIGNAL(clicked()), this, SLOT(dlDb()));
}

/**
 * @brief Refresh the asked page (on screen swap)
 * @param i
 */
void MainWindow::refreshSelector(int i)
{
    switch(i)
    {
        case 0 :
            reloadPage();
        break;
        case 1 :
            rngGantt();
        break;
        case 2:
            loadPage();
        break;
        case 3:
            loadArchive();
        break;
    }
}

/**
 * @brief Refresh kanban
 */
void MainWindow::reloadPage()
{
    QList<tray *> trays = this->findChildren<tray *>();
    foreach(tray* T, trays)
    {
        QList<task *> tasks = T->findChildren<task *>();
        foreach(task* t, tasks)
            t->close();
        load(T);
    }
}

/**
 * @brief Close DB and clean trays
 */
void MainWindow::kill()
{
    db->CloseDB();
    db->db.removeDatabase(db->db.connectionName());
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

/**
 * @brief Go to the next DB
 */
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
    this->g->changeDb(db);
    rngGantt();
    update();
}

/**
 * @brief Go to the previous Db
 */
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
    this->g->changeDb(db);
    rngGantt();
    update();
}

/**
 * @brief Design Kanban page
 */
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
    crtDb->setMaximumWidth((this->width()+30)/4);
    crtDb->setMinimumHeight(40);
    load(this->t1);
    load(this->t2);
    load(this->t3);
    load(this->t4);

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

/**
 * @brief Add a task to the Todo list
 */
void MainWindow::createTask()
{
    task *w = new task(db, this);

    t2->layout->addWidget(w);
}

/**
 * @brief Charge trays of task (mainly on app load)
 * @param t
 */
void MainWindow::load(tray *t)
{
    QString str = "SELECT COUNT(*) as cnt FROM task WHERE tray="+QString::fromStdString(std::to_string(t->getId()))+" AND active=1";
    QSqlQueryModel *modelTaskCount = new QSqlQueryModel;
    modelTaskCount->setQuery(str, db->db);
    QString str1 = "SELECT number, priority, duration, tray, itemCount, "
                   "color, deadline, title FROM task WHERE tray="+QString::fromStdString(std::to_string(t->getId()))+
            " AND active=1 ORDER BY priority DESC";
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
        if(aTask->itemCount)
            aTask->completion->setMaximum(aTask->itemCount);
        else
            aTask->completion->setMaximum(1);

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
            connect(targ->b, SIGNAL(clicked(bool)), aTask, SLOT(deleteTarget(bool)));
            aTask->layout->addWidget(targ);
        }

        aTask->color();
        t->layout->addWidget(aTask);
        t->layout->setAlignment(Qt::AlignTop);
    }
}

/**
 * @brief Design the parameters page
 */
void MainWindow::designParms()
{
    QStringList l = this->db->getDbNames();
    ui->comboDb->addItem(" ");
    ui->comboDb->addItems(l);
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *dbl = new QHBoxLayout;
    QHBoxLayout *dbls = new QHBoxLayout;
    QHBoxLayout *lhf = new QHBoxLayout;
    QHBoxLayout *lhs = new QHBoxLayout;
    QHBoxLayout *lht = new QHBoxLayout;
    QHBoxLayout *lhfo = new QHBoxLayout;
    /*mainLay->setAlignment(Qt::AlignCenter);
    dbl->setAlignment(Qt::AlignCenter);
    lhf->setAlignment(Qt::AlignCenter);
    lhs->setAlignment(Qt::AlignCenter);
    lht->setAlignment(Qt::AlignCenter);
    lhfo->setAlignment(Qt::AlignCenter);*/
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

    mainLay->addWidget(ui->labDb);

    dbl->addWidget(ui->dbName);
    dbl->addWidget(ui->addDb);
    mainLay->addItem(dbl);

    dbls->addWidget(ui->comboDb);
    dbls->addWidget(ui->dlDb);
    mainLay->addItem(dbls);

    mainLay->addWidget(ui->labDay);

    lhf->addWidget(ui->label);
    lhf->addWidget(ui->spinMonday);
    lhf->addWidget(ui->label_5);
    lhf->addWidget(ui->spinFriday);
    mainLay->addItem(lhf);

    lhs->addWidget(ui->label_2);
    lhs->addWidget(ui->spinTuesday);
    lhs->addWidget(ui->label_6);
    lhs->addWidget(ui->spinSaturday);
    mainLay->addItem(lhs);

    lht->addWidget(ui->label_3);
    lht->addWidget(ui->spinWednesday);
    lht->addWidget(ui->label_7);
    lht->addWidget(ui->spinSunday);
    mainLay->addItem(lht);

    lhfo->addWidget(ui->label_4);
    lhfo->addWidget(ui->spinThursday);
    mainLay->addItem(lhfo);

    ui->page_4->setLayout(mainLay);
}

/**
 * @brief Design the Gantt page
 */
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
    update();
}

/**
 * @brief Refresh a Gantt in the specified time range
 */
void MainWindow::rngGantt()
{
    int dayLength[7];
    QStringList lst;
    QStringList lstNumb;
    QSqlQuery *query = new QSqlQuery(db->db);
    query->exec("SELECT COUNT(*) FROM task WHERE active=1");
    query->first();
    QSqlQueryModel *modelTask = new QSqlQueryModel;
    modelTask->setQuery("SELECT * FROM parms", db->db);
    for(int i = 0; i<7; i++)
    {
        dayLength[i] = modelTask->record(0).value(i).toInt();
    }
    QSqlQueryModel *titles = new QSqlQueryModel;
    titles->setQuery("SELECT title as title, number as numb FROM task WHERE active=1 ORDER BY priority DESC", db->db);
    for(int i = 0; i<query->value(0).toInt(); i++)
    {
        lst << titles->record(i).value("title").toString();
        lstNumb << titles->record(i).value("numb").toString();
    }

    this->g->build(lst, lstNumb, ui->displayFrom->date().daysTo(ui->displayTo->date())+1, dayLength, ui->displayFrom->date());
    ganttDisp->update();
}

/**
 * @brief Refresh Charge page
 */
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

    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::white));
    axisX->setTitleBrush(QBrush(Qt::white));
    axisY->setTitleBrush(QBrush(Qt::white));

    QBrush axisBrush(Qt::white);
    axisX->setLabelsBrush(axisBrush);
    axisY->setLabelsBrush(axisBrush);

    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);

    int l[6] = {};
    QBarSet *set = new QBarSet("Count");
    QStringList prio;
    QSqlQueryModel *model3 = new QSqlQueryModel;
    model3->setQuery("SELECT COUNT(*) as cnt, priority as pr FROM task WHERE active = 1 GROUP BY priority ORDER BY priority ASC", db->db);
    for(int i = 0; i < model3->rowCount(); ++i)
        l[model3->record(i).value("pr").toInt()] = model3->record(i).value("cnt").toInt();

    for(int i = 0; i < 6; ++i)
    {
        *set << l[i];
        prio << QString::number(i);
    }

    QBarSeries *barseries = new QBarSeries();
    barseries->append(set);

    QChart *chart2 = new QChart();
    chart2->addSeries(barseries);
    chart2->setTitle("Priority distribution");
    chart2->setBackgroundBrush(brush);
    chart2->legend()->hide();

    QBarCategoryAxis *axisX1 = new QBarCategoryAxis();
    axisX1->append(prio);
    chart2->addAxis(axisX1, Qt::AlignBottom);
    barseries->attachAxis(axisX1);
    axisX1->setTitleText("Priority");

    QValueAxis *axisY1 = new QValueAxis();
    chart2->addAxis(axisY1, Qt::AlignLeft);
    axisY1->setLabelFormat("%i");
    barseries->attachAxis(axisY1);
    axisY1->setTitleText("Count");

    chart2->setTitleFont(font);
    chart2->setTitleBrush(QBrush(Qt::white));
    axisX1->setTitleBrush(QBrush(Qt::white));
    axisY1->setTitleBrush(QBrush(Qt::white));

    axisX1->setLabelsBrush(axisBrush);
    axisY1->setLabelsBrush(axisBrush);

    ui->graphicsView_2->setChart(chart2);
    ui->graphicsView_2->setRenderHints(QPainter::Antialiasing);
}

void MainWindow::designChargePage()
{
    QHBoxLayout *l = new QHBoxLayout;
    l->addWidget(ui->graphicsView);
    l->addWidget(ui->graphicsView_2);
    ui->page_3->setLayout(l);
    loadPage();
}

/**
 * @brief Refresh archive page (kill all, recreate)
 */
void MainWindow::loadArchive()
{

    QList<task *> tasks = t5->findChildren<task *>();
    foreach(task* t, tasks)
        t->close();

    QString str = "SELECT COUNT(*) as cnt FROM task WHERE active=0";
    QSqlQueryModel *modelTaskCount = new QSqlQueryModel;
    modelTaskCount->setQuery(str, db->db);
    QString str1 = "SELECT number, priority, duration, tray, itemCount, "
                   "color, deadline, title FROM task WHERE active=0 ORDER BY number DESC";
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
        if(aTask->itemCount)
            aTask->completion->setMaximum(aTask->itemCount);
        else
            aTask->completion->setMaximum(1);
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
            connect(targ->b, SIGNAL(clicked(bool)), aTask, SLOT(deleteTarget(bool)));
            //targ->setVisible(true);
            aTask->layout->addWidget(targ);
        }

        aTask->color();
        aTask->setArchived();
        t5->layout->addWidget(aTask);
        t5->layout->setAlignment(Qt::AlignTop);
    }
}

/**
 * @brief Design Archive page
 */
void MainWindow::designArchive()
{
    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(t5);
    ui->page_5->setLayout(lay);
    loadArchive();
}

/**
 * @brief Handle resize event to ensure adaptability
 */
void MainWindow::resizeEvent(QResizeEvent*)
{
    t1->setMinimumWidth((this->width()-50)/5);
    t2->setMinimumWidth((this->width()-50)/5);
    t3->setMinimumWidth((this->width()-50)/5);
    t4->setMinimumWidth((this->width()-50)/5);
    QList<tray *> trays = this->findChildren<tray *>();
    foreach(tray* T, trays)
    {
        QList<task *> tasks = T->findChildren<task *>();
        foreach(task* t, tasks)
            t->resizeIt(T->width()-10);
    }
}

/**
 * @brief Add a database file
 */
void MainWindow::addDb()
{
    if(!ui->dbName->text().isEmpty())
    {
        std::string s = ui->dbName->text().toStdString();
        std::ofstream file { s+".db" };
    }
    ui->dbName->clear();
    ui->comboDb->clear();
    QStringList l = this->db->getDbNames();
    ui->comboDb->addItem(" ");
    ui->comboDb->addItems(l);
}

/**
 * @brief Remove a database file
 */
void MainWindow::dlDb()
{
    if(ui->comboDb->currentIndex())
    {
        if(ui->comboDb->currentText() == this->db->db.databaseName())
            nextDb();
        if(!QFile::remove(ui->comboDb->currentText()))
            qDebug() << ui->comboDb->currentText();
    }
    ui->dbName->clear();
    ui->comboDb->clear();
    QStringList l = this->db->getDbNames();
    ui->comboDb->addItem(" ");
    ui->comboDb->addItems(l);
}


