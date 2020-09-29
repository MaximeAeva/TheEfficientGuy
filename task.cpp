#include "task.h"

task::task(QWidget *parent) :
    QDockWidget(parent)
{
    ResID++;
    wdwId = ResID;
    setStyleSheet( "QWidget{ background-color : rgba( 160, 160, 160, 255); border-radius : 7px;  }" );
    this->setWindowTitle(title);
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetFloatable);
    completion->setOrientation(Qt::Horizontal);
    completion->setValue(0);
    completion->setRange(0, 1);
    this->layout->addWidget(completion);
    completion->show();
    lab->setLayout(layout);
    this->setWidget(lab);
    this->show();
}

task::~task()
{

}

int task::getID()
{
    return wdwId;
}

int task::ResID = 0;

void task::mouseDoubleClickEvent(QMouseEvent *event)
{
    QDialog *d = new QDialog;
    QGridLayout *layout = new QGridLayout;
    QVBoxLayout *secondLay = new QVBoxLayout;
    QLabel *t1 = new QLabel;
    QLineEdit *title = new QLineEdit;
    QLabel *t2 = new QLabel;
    QLabel *t3 = new QLabel;
    QLabel *t4 = new QLabel;
    QLineEdit *task = new QLineEdit;
    QPushButton *button = new QPushButton;
    QSlider *slide = new QSlider;
    QLabel *slideVal = new QLabel;
    connect(slide, SIGNAL(valueChanged(int)), slideVal, SLOT(setNum(int)));
    secondLay->addWidget(slideVal);
    secondLay->addWidget(slide);
    slide->setValue(priority);
    slide->setMaximum(5);
    slide->setMinimum(1);
    slide->setOrientation(Qt::Horizontal);
    t1->setText("Titre");
    t2->setText("Tâche");
    t3->setText("Priorité");
    t4->setText("Durée");
    button->setText("Valider");
    layout->addWidget(t1, 0, 0);
    layout->addWidget(title, 0, 1);
    layout->addWidget(t2, 1, 0);
    layout->addWidget(task, 1, 1);
    layout->addWidget(t3, 2, 0);
    layout->addItem(secondLay, 2, 1);
    layout->addWidget(t4, 3, 0);
    layout->addWidget(button, 4, 0);
    d->setLayout(layout);
    d->setWindowTitle("Contenu");
    connect(button, SIGNAL(clicked()), d, SLOT(accept()));
    int dialogCode = d->exec();
    if(dialogCode == QDialog::Accepted)
    {
        if(!title->text().isEmpty()) this->setWindowTitle(title->text());
        if(!task->text().isEmpty())
        {
            itemCount++;
            QHBoxLayout *lays = new QHBoxLayout;
            QCheckBox *c = new QCheckBox;
            QPushButton *b = new QPushButton;
            b->setText("X");
            b->setStyleSheet("background-color : rgba( 160, 0, 0, 255); border-radius : 7px;");
            c->setText(task->text());
            lays->addWidget(c);
            lays->addWidget(b);
            this->layout->addLayout(lays);
            completion->setRange(0, itemCount);
            connect(c, SIGNAL(stateChanged(int)), this, SLOT(completionVal(int)));
            connect(b, SIGNAL(clicked()), this, SLOT(remove(b)));
        }
        if((slide->value())!=priority) priority = slide->value();
        color();
    }
}

void task::completionVal(int i)
{
    if(!i) completion->setValue(completion->value()-1);
    else completion->setValue(completion->value()+1);
}

void task::remove(QPushButton *b)
{
    itemCount--;
    if(itemCount) completion->setMaximum(itemCount);
    b->parentWidget()->destroyed();
}

void task::color()
{
    switch (priority)
    {
        case 1:
        setStyleSheet("QDockWidget::title {background: #235B66; color: black;} QWidget{background : rgba( 160, 160, 160, 255); border-radius : 7px;}");
        break;
        case 2:
        setStyleSheet("QDockWidget::title {background: #11AEBF; color: black;} QWidget{background : rgba( 160, 160, 160, 255); border-radius : 7px;}");
        break;
        case 3:
        setStyleSheet("QDockWidget::title {background: #A0BF30; color: black;} QWidget{background : rgba( 160, 160, 160, 255); border-radius : 7px;}");
        break;
        case 4:
        setStyleSheet("QDockWidget::title {background: #F2AE30; color: black;} QWidget{background : rgba( 160, 160, 160, 255); border-radius : 7px;}");
        break;
        case 5:
        setStyleSheet("QDockWidget::title {background: #F25244; color: black;} QWidget{background : rgba( 160, 160, 160, 255); border-radius : 7px;}");
        break;
    }
}

