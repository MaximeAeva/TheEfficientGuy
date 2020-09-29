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
                                   "border-color: rgb(25, 52, 65);"
                                   "color: rgb(25, 52, 65);"
                                   "background-color: rgb(203, 219, 215);"
                                   "border : 2px solid;"
                                   "border-radius : 15px;"
                                   "height : 1em;"
                                   "width : 1em;}"
                                   "QPushButton:hover{"
                                       "background-color: rgb(25, 52, 65);"
                                       "color: rgb(203, 219, 215);"
                                       "border-color: rgb(203, 219, 215);}"
                                   "QPushButton:pressed{"
                                       "background-color: rgb(203, 219, 215);"
                                       "color: rgb(25, 52, 65);"
                                       "border-color: rgb(25, 52, 65);}");
    col->addWidget(t1);
    t1->setVisible(true);
    col->addWidget(t2);
    t2->setVisible(true);
    col->addWidget(t3);
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






