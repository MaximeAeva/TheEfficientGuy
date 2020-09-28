#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->adder, SIGNAL(clicked()), this, SLOT(createTask()));
    ui->ideas->setAcceptDrops(true);
    ui->todo->setAcceptDrops(true);
    ui->inprogress->setAcceptDrops(true);
    ui->standby->setAcceptDrops(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createTask()
{
    task *w = new task(this);
    w->setVisible(true);
    if(ui->todo->layout()) ui->todo->layout()->addWidget(w);
    else
    {
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(w);
        vbox->stretch(1);
        ui->todo->setLayout(vbox);
    }

}

