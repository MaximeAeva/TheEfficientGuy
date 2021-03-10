#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    designHomePage();
    designPage();
    designParms();
    designGanttPage();
    designConnections();
    designArchive();
    designChargePage();
    ui->stackedWidget->addWidget(ui->page_6);
    ui->stackedWidget->addWidget(ui->page);
    ui->stackedWidget->addWidget(ui->page_2);
    ui->stackedWidget->addWidget(ui->page_3);
    ui->stackedWidget->addWidget(ui->page_5);
    ui->stackedWidget->addWidget(ui->page_4);
    ui->stackedWidget->setCurrentIndex(crtPage);
    this->selector->setStyleSheet("QPushButton{"
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
    selector->setText("Menu");


    QGridLayout *fl = new QGridLayout;
    QWidget *widget = new QWidget();
    widget->setStyleSheet("QWidget {color: #E6E6E8;}");
    fl->addWidget(ui->stackedWidget, 0, 0, 2, 10);
    fl->addWidget(selector, 0, 9, 1, 1);
    widget->setLayout(fl);
    this->setCentralWidget(widget);

    this->setWindowState(Qt::WindowMaximized);
    this->setWindowTitle("The Efficient Guy !");
    this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() +"/icone.ico"));

    connect(selector, SIGNAL(clicked()), this, SLOT(showMenu()));
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    QSqlQueryModel *model4 = new QSqlQueryModel;
    model4->setQuery("SELECT AVG(valeur) as val FROM (SELECT COUNT(*) as valeur FROM target GROUP BY parentTask)", db->db);
    QSqlQueryModel *model5 = new QSqlQueryModel;
    model5->setQuery("SELECT COUNT(*) as cnt FROM task", db->db);
    QSqlQueryModel *model6 = new QSqlQueryModel;
    model6->setQuery("SELECT COUNT(*) as cnt FROM task WHERE active=1", db->db);
    this->db->updateMiscellaneous(this->timer->elapsed());

    db->CloseDB();
    this->db = NULL;
    delete db;
    this->crtDb = NULL;
    this->subdbl = NULL;
    this->bef = NULL;
    this->aft = NULL;
    this->adder = NULL;
    this->g = NULL;
    this->t1 = NULL;
    this->t2 = NULL;
    this->t3 = NULL;
    this->t4 = NULL;
    this->t5 = NULL;
    this->timer = NULL;
    this->ganttDisp = NULL;
    this->b = NULL;
    this->pageL = NULL;
    this->col = NULL;

    delete crtDb;
    delete subdbl;
    delete bef;
    delete aft;
    delete adder;
    delete g;
    delete t1;
    delete t2;
    delete t3;
    delete t4;
    delete t5;
    delete timer;
    delete ganttDisp;
    delete b;
    delete pageL;
    delete col;

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
    connect(ui->displayArchive, SIGNAL(clicked()), this, SLOT(rngGantt()));
    connect(ui->editModeButton, SIGNAL(clicked()), this, SLOT(rngGantt()));
    connect(ui->displayFrom, SIGNAL(dateChanged(QDate)), this, SLOT(rngGantt()));
    connect(ui->displayFrom, SIGNAL(dateChanged(QDate)), this, SLOT(rngGantt()));
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(refreshSelector(int)));
    connect(ui->addDb, SIGNAL(clicked()), this, SLOT(addDb()));
    connect(ui->dlDb, SIGNAL(clicked()), this, SLOT(dlDb()));
    connect(ui->page_6, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));
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
            loadHomePage();
        break;
        case 1 :
            reloadPage();
        break;
        case 2 :
            rngGantt();
        break;
        case 3:
            loadPage();
        break;
        case 4:
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
    QList<postit *> note = ui->page_6->findChildren<postit *>();
    foreach(postit* p, note)
        p->~postit();
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
    loadHomePage();
    QSqlQueryModel *Notes = new QSqlQueryModel;
    Notes->setQuery("SELECT id, x, y, text FROM note", db->db);
    for(int i = 0; i<Notes->rowCount(); i++)
    {
        QPoint p = {Notes->record(i).value(1).toInt(), Notes->record(i).value(2).toInt()};
        new postit(ui->page_6, db, p, Notes->record(i).value(3).toString(), Notes->record(i).value(0).toInt());
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
    loadHomePage();
    QSqlQueryModel *Notes = new QSqlQueryModel;
    Notes->setQuery("SELECT id, x, y, text FROM note", db->db);
    for(int i = 0; i<Notes->rowCount(); i++)
    {
        QPoint p = {Notes->record(i).value(1).toInt(), Notes->record(i).value(2).toInt()};
        new postit(ui->page_6, db, p, Notes->record(i).value(3).toString(), Notes->record(i).value(0).toInt());
    }
}

void MainWindow::designHomePage()
{

    QVBoxLayout *ml = new QVBoxLayout;
    QHBoxLayout *dbl = new QHBoxLayout;
    QGridLayout *ssubl = new QGridLayout;
    ssubl->addWidget(ui->graphics_Time, 0, 0);//Time spent on app (1 color)
    ssubl->addWidget(ui->graphics_Avg, 0, 1);// Average time spent per task (several colors)
    ssubl->addWidget(ui->graphics_Active, 0, 2);//Active task count evolution (several colors)
    ssubl->addWidget(ui->graphics_Duration, 1, 0);//Avergage estimed time (several colors)
    ssubl->addWidget(ui->graphics_Allocation, 1, 1);//Average Allocation (several colors)
    ssubl->addWidget(ui->graphics_Overall, 1, 2);//Overall active count (1 color)
    subdbl->setAlignment(Qt::AlignHCenter);
    bef->setText("<");
    bef->setMaximumSize(40, 40);
    aft->setText(">");
    aft->setMaximumSize(40, 40);
    dbl->addStretch(1);
    dbl->addWidget(bef);
    dbl->addWidget(crtDb);
    dbl->addWidget(aft);
    dbl->addStretch(1);
    ml->addItem(dbl);
    ml->addWidget(subdbl);
    ml->addItem(ssubl);
    ui->page_6->setLayout(ml);
    ui->page_6->setContextMenuPolicy(Qt::CustomContextMenu);
    loadHomePage();
    QSqlQueryModel *Notes = new QSqlQueryModel;
    Notes->setQuery("SELECT id, x, y, text FROM note", db->db);
    for(int i = 0; i<Notes->rowCount(); i++)
    {
        QPoint p = {Notes->record(i).value(1).toInt(), Notes->record(i).value(2).toInt()};
        new postit(ui->page_6, db, p, Notes->record(i).value(3).toString(), Notes->record(i).value(0).toInt());
    }
}

void MainWindow::loadHomePage()
{
    QFont font;
    font.setPixelSize(18);
    QStringList dbNames = this->db->getDbNames();
    subdbl->clear();
    subdbl->setFont(font);
    QString dbNamesStr;
    for(int i = 0; i<dbNames.length(); i++)
    {
        dbNamesStr += dbNames.at(i);
        dbNamesStr += "   ";
    }
    subdbl->setText(dbNamesStr);

    this->db->updateMiscellaneous(0);

    QSqlQuery *timeElap = new QSqlQuery(db->db);
    timeElap->exec("SELECT SUM(spentTime) FROM miscellaneous");
    timeElap->first();

    QSqlQueryModel *generalMod = new QSqlQueryModel;
    generalMod->setQuery("SELECT id, avgTask, activeTask, spentTime, avgTask0, avgTask1, avgTask2, avgTask3,"
                         " avgTask4, avgTask5, activeTask0, activeTask1, activeTask2, activeTask3,"
                         " activeTask4, activeTask5, estimedTime0, estimedTime1, estimedTime2, estimedTime3,"
                         " estimedTime4, estimedTime5, allocated0, allocated1, allocated2, allocated3,"
                         " allocated4, allocated5 FROM miscellaneous ORDER BY id DESC LIMIT 10", db->db);


    float max = 0;
    QLineSeries *spentTimeLines = new QLineSeries();
    for(int i = 0; i<10; i++)
    {
        spentTimeLines->append(10-i,(generalMod->record(i).value(3).toInt()/1000)/60);
        if(((generalMod->record(i).value(3).toInt()/1000)/60)>max)
            max = ((generalMod->record(i).value(3).toInt()/1000)/60);
    }

    QPen pen = spentTimeLines->pen();
    pen.setWidth(5);
    pen.setBrush(QBrush(QColor("#11AEBF")));
    spentTimeLines->setPen(pen);

    QBrush brush;
    brush.setColor(QColor::fromRgb(49, 54, 63));

    QChart *chart = new QChart();
    chart->addSeries(spentTimeLines);
    chart->legend()->hide();
    chart->setTitle("Time. ("+myTime(timeElap->record().value(0).toInt(), 0)+")");
    chart->setBackgroundBrush(brush);

    QValueAxis *axisX = new QValueAxis;
    axisX->setTickCount(10);
    axisX->setLabelFormat("%i");
    chart->addAxis(axisX, Qt::AlignBottom);
    spentTimeLines->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Amount (mn)");
    chart->addAxis(axisY, Qt::AlignLeft);
    spentTimeLines->attachAxis(axisY);
    axisY->setRange(0, max+(0.1*max));

    //Chart avgTask

    QChart *chart2 = new QChart();

    QValueAxis *avgTaskX = new QValueAxis;
    avgTaskX->setTickCount(10);
    avgTaskX->setLabelFormat("%i");
    chart2->addAxis(avgTaskX, Qt::AlignBottom);

    QValueAxis *avgTaskY = new QValueAxis;
    avgTaskY->setLabelFormat("%i");
    avgTaskY->setTitleText("Targets per task");
    chart2->addAxis(avgTaskY, Qt::AlignLeft);

    max = 0;
    for(int p = 0; p<6; p++)
    {
        QLineSeries *avgTaskLines = new QLineSeries();
        QPen avgTaskPen = avgTaskLines->pen();
        avgTaskPen.setWidth(5);
        avgTaskPen.setBrush(QBrush(QColor(PrioToColor(p))));
        avgTaskLines->setPen(avgTaskPen);
        for(int i = 0; i<10; i++)
        {
            avgTaskLines->append(10-i,generalMod->record(i).value(4+p).toReal());
            if(generalMod->record(i).value(4+p).toReal()>max)
                max = generalMod->record(i).value(4+p).toReal();
        }
        chart2->addSeries(avgTaskLines);
        avgTaskLines->attachAxis(avgTaskX);
        avgTaskLines->attachAxis(avgTaskY);
    }
    chart2->legend()->hide();
    chart2->setTitle("Average targets per task");
    chart2->setBackgroundBrush(brush);
    avgTaskY->setRange(0, max+(0.1*max));

    //Chart activeTask

    QChart *chart3 = new QChart();

    QValueAxis *actTaskX = new QValueAxis;
    actTaskX->setTickCount(10);
    actTaskX->setLabelFormat("%i");
    chart3->addAxis(actTaskX, Qt::AlignBottom);

    QValueAxis *actTaskY = new QValueAxis;
    actTaskY->setLabelFormat("%i");
    actTaskY->setTitleText("Active tasks");
    chart3->addAxis(actTaskY, Qt::AlignLeft);

    max = 0;
    for(int p = 0; p<6; p++)
    {
        QLineSeries *actTaskLines = new QLineSeries();
        QPen actTaskPen = actTaskLines->pen();
        actTaskPen.setWidth(5);
        actTaskPen.setBrush(QBrush(QColor(PrioToColor(p))));
        actTaskLines->setPen(actTaskPen);
        for(int i = 0; i<10; i++)
        {
            actTaskLines->append(10-i,generalMod->record(i).value(10+p).toInt());
            if(generalMod->record(i).value(10+p).toInt()>max)
                max = generalMod->record(i).value(10+p).toInt();
        }
        chart3->addSeries(actTaskLines);
        actTaskLines->attachAxis(actTaskX);
        actTaskLines->attachAxis(actTaskY);
    }
    chart3->legend()->hide();
    chart3->setTitle("Active tasks");
    chart3->setBackgroundBrush(brush);
    actTaskY->setRange(0, max+1);
    actTaskY->setTickInterval(1+((int(max)+1)%10));

    //Chart estimedTime

    QChart *chart4 = new QChart();

    QValueAxis *estimedTimeX = new QValueAxis;
    estimedTimeX->setTickCount(10);
    estimedTimeX->setLabelFormat("%i");
    chart4->addAxis(estimedTimeX, Qt::AlignBottom);

    QValueAxis *estimedTimeY = new QValueAxis;
    estimedTimeY->setLabelFormat("%i");
    estimedTimeY->setTitleText("Estimed time (d)");
    chart4->addAxis(estimedTimeY, Qt::AlignLeft);

    max = 0;
    for(int p = 0; p<6; p++)
    {
        QLineSeries *estimedTimeLines = new QLineSeries();
        QPen estimedTimePen = estimedTimeLines->pen();
        estimedTimePen.setWidth(5);
        estimedTimePen.setBrush(QBrush(QColor(PrioToColor(p))));
        estimedTimeLines->setPen(estimedTimePen);
        for(int i = 0; i<10; i++)
        {
            estimedTimeLines->append(10-i,generalMod->record(i).value(16+p).toInt());
            if(generalMod->record(i).value(16+p).toInt()>max)
                max = generalMod->record(i).value(16+p).toInt();
        }
        chart4->addSeries(estimedTimeLines);
        estimedTimeLines->attachAxis(estimedTimeX);
        estimedTimeLines->attachAxis(estimedTimeY);
    }
    chart4->legend()->hide();
    chart4->setTitle("Processing time");
    chart4->setBackgroundBrush(brush);
    estimedTimeY->setRange(0, max+(0.1*max));


    //Chart allocated

    QChart *chart5 = new QChart();

    QValueAxis *allocatedX = new QValueAxis;
    allocatedX->setTickCount(10);
    allocatedX->setLabelFormat("%i");
    chart5->addAxis(allocatedX, Qt::AlignBottom);

    QValueAxis *allocatedY = new QValueAxis;
    allocatedY->setLabelFormat("%i");
    allocatedY->setTitleText("Allocated (h)");
    chart5->addAxis(allocatedY, Qt::AlignLeft);

    max = 0;
    for(int p = 0; p<6; p++)
    {
        QLineSeries *allocatedLines = new QLineSeries();
        QPen allocatedPen = allocatedLines->pen();
        allocatedPen.setWidth(5);
        allocatedPen.setBrush(QBrush(QColor(PrioToColor(p))));
        allocatedLines->setPen(allocatedPen);
        for(int i = 0; i<10; i++)
        {
            allocatedLines->append(10-i,generalMod->record(i).value(22+p).toInt());
            if(generalMod->record(i).value(22+p).toInt()>max)
                max = generalMod->record(i).value(22+p).toInt();
        }
        chart5->addSeries(allocatedLines);
        allocatedLines->attachAxis(allocatedX);
        allocatedLines->attachAxis(allocatedY);
    }
    chart5->legend()->hide();
    chart5->setTitle("Allocated time");
    chart5->setBackgroundBrush(brush);
    allocatedY->setRange(0, max+(0.1*max));

    //Chart tot

    QChart *chart6 = new QChart();

    QValueAxis *totX = new QValueAxis;
    totX->setTickCount(10);
    totX->setLabelFormat("%i");
    chart6->addAxis(totX, Qt::AlignBottom);

    QValueAxis *totY = new QValueAxis;
    totY->setLabelFormat("%i");
    totY->setTitleText("Amount");
    chart6->addAxis(totY, Qt::AlignLeft);

    max = 0;
    float min = generalMod->record(0).value(1).toInt();
    for(int p = 0; p<2; p++)
    {
        QLineSeries *totLines = new QLineSeries();
        QPen totPen = totLines->pen();
        totPen.setWidth(5);
        totPen.setBrush(QBrush(QColor(PrioToColor(p+2))));
        totLines->setPen(totPen);
        for(int i = 0; i<10; i++)
        {
            totLines->append(10-i,generalMod->record(i).value(1+p).toInt());
            if(generalMod->record(i).value(1+p).toReal()>max)
                max = generalMod->record(i).value(1+p).toReal();
            if(generalMod->record(i).value(1+p).toReal()<min)
                min = generalMod->record(i).value(1+p).toReal();
        }
        chart6->addSeries(totLines);
        totLines->attachAxis(totX);
        totLines->attachAxis(totY);
    }
    chart6->legend()->hide();
    chart6->setTitle("Total task and archive count");
    chart6->setBackgroundBrush(brush);
    totY->setRange(min-(0.1*min), max+(0.1*max));

    QBrush axisBrush(Qt::white);

    axisX->setLabelsBrush(axisBrush);
    axisY->setLabelsBrush(axisBrush);

    avgTaskX->setLabelsBrush(axisBrush);
    avgTaskY->setLabelsBrush(axisBrush);

    actTaskX->setLabelsBrush(axisBrush);
    actTaskY->setLabelsBrush(axisBrush);

    estimedTimeX->setLabelsBrush(axisBrush);
    estimedTimeY->setLabelsBrush(axisBrush);

    allocatedX->setLabelsBrush(axisBrush);
    allocatedY->setLabelsBrush(axisBrush);

    totX->setLabelsBrush(axisBrush);
    totY->setLabelsBrush(axisBrush);

    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::white));
    axisX->setTitleBrush(QBrush(Qt::white));
    axisY->setTitleBrush(QBrush(Qt::white));

    chart2->setTitleFont(font);
    chart2->setTitleBrush(QBrush(Qt::white));
    avgTaskX->setTitleBrush(QBrush(Qt::white));
    avgTaskY->setTitleBrush(QBrush(Qt::white));

    chart3->setTitleFont(font);
    chart3->setTitleBrush(QBrush(Qt::white));
    actTaskX->setTitleBrush(QBrush(Qt::white));
    actTaskY->setTitleBrush(QBrush(Qt::white));

    chart4->setTitleFont(font);
    chart4->setTitleBrush(QBrush(Qt::white));
    estimedTimeX->setTitleBrush(QBrush(Qt::white));
    estimedTimeY->setTitleBrush(QBrush(Qt::white));

    chart5->setTitleFont(font);
    chart5->setTitleBrush(QBrush(Qt::white));
    allocatedX->setTitleBrush(QBrush(Qt::white));
    allocatedY->setTitleBrush(QBrush(Qt::white));

    chart6->setTitleFont(font);
    chart6->setTitleBrush(QBrush(Qt::white));
    totX->setTitleBrush(QBrush(Qt::white));
    totY->setTitleBrush(QBrush(Qt::white));

    ui->graphics_Time->setChart(chart);
    ui->graphics_Avg->setChart(chart2);
    ui->graphics_Active->setChart(chart3);
    ui->graphics_Duration->setChart(chart4);
    ui->graphics_Allocation->setChart(chart5);
    ui->graphics_Overall->setChart(chart6);
}


/**
 * @brief Design Kanban page
 */
void MainWindow::designPage()
{

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
        QString str3 ="SELECT number, title, state FROM target WHERE parentTask="+modelTask->record(i).value("number").toString()+" ORDER BY state";
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
    lh->addWidget(ui->displayArchive);
    lh->addWidget(ui->editModeButton);
    lh->addStretch(1);
    ui->displayFrom->setDate(QDate::currentDate());
    ui->displayTo->setDate(QDate::currentDate().addDays(14));
    ui->displayArchive->setChecked(false);
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
    if(ui->editModeButton->isChecked())
        ui->editModeButton->setText("View Mode");
    else
        ui->editModeButton->setText("Edit Mode");

    if(ui->displayArchive->isChecked())
        ui->displayArchive->setText("Hide archives");
    else
        ui->displayArchive->setText("Display archives");
    int dayLength[7];
    QStringList lst;
    QStringList lstNumb;
    QSqlQuery *query = new QSqlQuery(db->db);
    if(ui->displayArchive->isChecked())
        query->exec("SELECT COUNT(*) FROM task");
    else
        query->exec("SELECT COUNT(*) FROM task WHERE active=1");
    query->first();
    QSqlQueryModel *modelTask = new QSqlQueryModel;
    modelTask->setQuery("SELECT * FROM parms", db->db);
    for(int i = 0; i<7; i++)
    {
        dayLength[i] = modelTask->record(0).value(i).toInt();
    }
    QSqlQueryModel *titles = new QSqlQueryModel;
    if(ui->displayArchive->isChecked())
        titles->setQuery("SELECT title as title, number as numb FROM task ORDER BY priority DESC", db->db);
    else
        titles->setQuery("SELECT title as title, number as numb FROM task WHERE active=1 ORDER BY priority DESC", db->db);

    for(int i = 0; i<query->value(0).toInt(); i++)
    {
        lst << titles->record(i).value("title").toString();
        lstNumb << titles->record(i).value("numb").toString();
    }

    this->g->build(lst, lstNumb, ui->displayFrom->date().daysTo(ui->displayTo->date())+1,
                   dayLength, ui->displayFrom->date(), ui->editModeButton->isChecked());
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
        QString str3 ="SELECT number, title, state FROM target WHERE parentTask="+modelTask->record(i).value("number").toString()+" ORDER BY state";
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
    QDialog *d = new QDialog;
    QGridLayout *layout = new QGridLayout;
    QLabel *t = new QLabel;
    QPushButton *ok = new QPushButton;
    ok->setText("OK");
    QPushButton *canc = new QPushButton;
    canc->setText("Cancel");
    connect(ok, SIGNAL(clicked()), d, SLOT(accept()));
    connect(canc, SIGNAL(clicked()), d, SLOT(reject()));
    t->setText("Do you want to delete "+ ui->comboDb->currentText());
    layout->addWidget(t, 0, 0, 1, 2);
    layout->addWidget(ok, 1, 0);
    layout->addWidget(canc, 1, 1);
    d->setLayout(layout);
    d->setWindowTitle("Confirmation box");
    int dialogCode = d->exec();
    if(dialogCode == QDialog::Accepted)
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
}

/**
 * @brief Handle focus/unfocus to evaluate time spent on app
 * @param e
 * @return
 */
bool MainWindow::event(QEvent * e)
{
    switch(e->type())
    {
        case QEvent::WindowActivate :
            this->timer->start();
            break ;

        case QEvent::WindowDeactivate :
            myTime(this->timer->elapsed(), 1);
            break ;
    } ;
    return QMainWindow::event(e) ;
}

/**
 * @brief Transform in day, hours, minutes, secondes
 * @param timeElapse
 * @return
 */
QString MainWindow::myTime(int timeElapse, bool rec)
{
    QString TimeElapseShow;

    if(rec) this->db->updateMiscellaneous(timeElapse);
    int s = round(float(timeElapse)/float(1000));
    int d = s/86400;
    TimeElapseShow = QString::number(d)+" days, ";
    s %= 86400;
    int h = s/3600;
    TimeElapseShow += QString::number(h)+" hours, ";
    s %= 3600;
    int m = s/60;
    TimeElapseShow += QString::number(m)+" minutes, ";
    s %= 60;
    TimeElapseShow += QString::number(s)+" secondes.";
    return TimeElapseShow;
}

QString MainWindow::PrioToColor(int p)
{
    QString s;
    switch (p)
    {
        case 1:
        s = "#235B66";
        break;
        case 2:
        s = "#11AEBF";
        break;
        case 3:
        s = "#A0BF30";
        break;
        case 4:
        s = "#F2AE30";
        break;
        case 5:
        s = "#F25244";
        break;
        default:
        s = "#546670";
        break;
    }
    return s;
}

void MainWindow::ShowContextMenu(const QPoint &pos)
{
   QMenu contextMenu(tr("Context menu"), this);
   QFont font;
   font.setPixelSize(18);

   contextMenu.setFont(font);
   contextMenu.setStyleSheet("QMenu::item {"
                                 "color: #66767A;"
                                 "background-color: #A6C0C6;"
                             "} "
                             "QMenu::item:selected {"
                                "color: #66767A;"
                                 "background-color: black;"
                             "}");
   QAction action1("Add a note", this);
   connect(&action1, SIGNAL(triggered()), this, SLOT(addNotes()));
   contextMenu.addAction(&action1);

   contextMenu.exec(mapToGlobal(pos));
}

void MainWindow::addNotes()
{
    new postit(ui->page_6, db);
}

void MainWindow::showMenu()
{
    QPoint p = selector->pos();
    QMenu appMenu(tr("Menu"), this);
    QFont font;
    font.setPixelSize(18);

    appMenu.setFont(font);
    appMenu.setStyleSheet("QMenu::item {"
                                  "color: #66767A;"
                                  "background-color: #A6C0C6;"
                              "} "
                              "QMenu::item:selected {"
                                 "color: #66767A;"
                                  "background-color: black;"
                              "}");
    QAction action1("Dashboard");
    QAction action2("Kanban");
    QAction action3("Gantt");
    QAction action4("Charge");
    QAction action5("Archive");
    QAction action6("Parameters");
    QAction action7("Quit");
    QSignalMapper* signalMapper = new QSignalMapper (this) ;
    connect (&action1, SIGNAL(triggered()), signalMapper, SLOT(map())) ;
    connect (&action2, SIGNAL(triggered()), signalMapper, SLOT(map())) ;
    connect (&action3, SIGNAL(triggered()), signalMapper, SLOT(map())) ;
    connect (&action4, SIGNAL(triggered()), signalMapper, SLOT(map())) ;
    connect (&action5, SIGNAL(triggered()), signalMapper, SLOT(map())) ;
    connect (&action6, SIGNAL(triggered()), signalMapper, SLOT(map())) ;

    signalMapper -> setMapping (&action1, 0) ;
    signalMapper -> setMapping (&action2, 1) ;
    signalMapper -> setMapping (&action3, 2) ;
    signalMapper -> setMapping (&action4, 3) ;
    signalMapper -> setMapping (&action5, 4) ;
    signalMapper -> setMapping (&action6, 5) ;

    connect (signalMapper, SIGNAL(mapped(int)), ui->stackedWidget, SLOT(setCurrentIndex(int))) ;

    connect(&action7, SIGNAL(triggered()), this, SLOT(close()));
    appMenu.addAction(&action1);
    appMenu.addAction(&action2);
    appMenu.addAction(&action3);
    appMenu.addAction(&action4);
    appMenu.addAction(&action5);
    appMenu.addAction(&action6);
    appMenu.addAction(&action7);

    appMenu.exec(mapToGlobal(p));
}
