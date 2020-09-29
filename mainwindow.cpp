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
    setAcceptDrops(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createTask()
{
    task *w = new task(this);
    w->setVisible(true);
    addWidgetToBox(ui->todo, w);
}

void MainWindow::addWidgetToBox(QGroupBox *gb, QWidget *w)
{
    if(gb->layout()) gb->layout()->addWidget(w);
    else
    {
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(w);
        vbox->stretch(1);
        gb->setLayout(vbox);
    }
}




