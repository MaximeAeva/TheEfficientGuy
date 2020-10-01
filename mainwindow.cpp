#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    designPage();
    connect(this->adder, SIGNAL(clicked()), this, SLOT(createTask()));
    database *db = new database;
    QStringList stl = db->Connections();
    for(int i = 0; i < stl.size(); i++)
        std::cout << i+1 << " : " << stl.at(0).toStdString() << std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::designPage()
{
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
    task *w = new task(this);
    w->setVisible(true);
    t2->layout->addWidget(w);
}






