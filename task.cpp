#include "task.h"

task::task()
{
    this->setStyleSheet( "QWidget{ background-color : #546670; border-radius : 7px;border : 1px solid black;}" );
    completion->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #05B8CC;}");
    lab->setStyleSheet("background-color :#66767A;border-radius : 7px;border : 1px solid black;} QCheckBox{border:none;");
    this->setWindowTitle(title);
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetFloatable);
    this->setAttribute(Qt::WA_DeleteOnClose);
    completion->setOrientation(Qt::Horizontal);
    completion->setValue(0);
    completion->setRange(0, 1);
    this->labelColor->setStyleSheet("background-color:"+this->colorTask.name());
    this->labelColor->show();
    this->labelColor->setMaximumHeight(20);
    this->labelColor->setMaximumWidth(20);
    this->layout->addWidget(this->labelColor);
    this->layout->addWidget(completion);
    this->layout->addStretch(1);
    completion->show();
    lab->setLayout(layout);
    this->setWidget(lab);
    this->show();
}

task::task(database *db, QWidget *parent) :
    QDockWidget(parent)
{
    wdwId = QDateTime::currentDateTime();
    this->db = db;
    db->addTask(this->wdwId, this->priority, this->duration, this->group, this->itemCount, this->deadLine, this->title, this->colorTask.name());
    this->setStyleSheet( "QWidget{ background-color : #546670; border-radius : 7px;border : 1px solid black;}" );
    completion->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #05B8CC;}");
    lab->setStyleSheet("background-color :#66767A;border-radius : 7px;border : 1px solid black;} QCheckBox{border:none;");
    this->setWindowTitle(title);
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetFloatable);
    this->setAttribute(Qt::WA_DeleteOnClose);
    completion->setOrientation(Qt::Horizontal);
    completion->setValue(0);
    completion->setRange(0, 1);
    this->labelColor->setStyleSheet("background-color:"+this->colorTask.name());
    this->labelColor->show();
    this->labelColor->setMaximumHeight(20);
    this->labelColor->setMaximumWidth(20);
    this->layout->addWidget(this->labelColor);
    this->layout->addWidget(completion);
    this->layout->addStretch(1);
    completion->show();
    lab->setLayout(layout);
    this->setWidget(lab);
    this->show();
}

task::~task()
{
    this->db->deleteTask(this->wdwId);
    this->db = NULL;
    delete this->db;
    this->layout = NULL;
    delete this->layout;
    this->lab = NULL;
    delete this->lab;
    this->completion = NULL;
    delete this->completion;
}

void task::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->accept();
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
    QSpinBox *spin = new QSpinBox;
    QColorDialog *colorD = new QColorDialog;
    QPushButton *opCol = new QPushButton;
    colorD->setCurrentColor(this->colorTask);
    spin->setValue(this->duration);
    spin->setMinimum(1);
    connect(slide, SIGNAL(valueChanged(int)), slideVal, SLOT(setNum(int)));
    secondLay->addWidget(slideVal);
    secondLay->addWidget(slide);
    slide->setValue(priority);
    slide->setMaximum(5);
    slide->setMinimum(0);
    slide->setOrientation(Qt::Horizontal);
    t1->setText("Title");
    t2->setText("Task");
    t3->setText("Priority");
    t4->setText("Duration");
    opCol->setText("Color");
    button->setText("OK");
    layout->addWidget(t1, 0, 0);
    layout->addWidget(title, 0, 1);
    layout->addWidget(t2, 1, 0);
    layout->addWidget(task, 1, 1);
    layout->addWidget(t3, 2, 0);
    layout->addItem(secondLay, 2, 1);
    layout->addWidget(t4, 3, 0);
    layout->addWidget(spin, 3, 1);
    layout->addWidget(opCol, 4, 0);
    layout->addWidget(button, 5, 0);
    connect(opCol, SIGNAL(clicked()), colorD, SLOT(open()));
    d->setLayout(layout);
    d->setWindowTitle("Content");
    connect(button, SIGNAL(clicked()), d, SLOT(accept()));
    int dialogCode = d->exec();
    if(dialogCode == QDialog::Accepted)
    {
        if(colorD->currentColor()!=this->colorTask) this->colorTask = colorD->selectedColor();
        this->labelColor->setStyleSheet("background-color:"+this->colorTask.name());
        qDebug() << colorTask;
        if(!title->text().isEmpty())
        {
            this->setWindowTitle(title->text());
            this->title = title->text();
        }
        if(!task->text().isEmpty())
        {
            itemCount++;
            target *t = new target(task->text(), this->db, this->wdwId);
            this->layout->addWidget(t);
            completion->setMaximum(itemCount);
            connect(t->c, SIGNAL(stateChanged(int)), this, SLOT(completionVal(int)));
            connect(t->b, SIGNAL(clicked()), this, SLOT(deleteTarget()));
        }
        this->priority = slide->value();
        this->duration = spin->value();
        color();
    }
    this->db->updateTask(this->wdwId, this->priority, this->duration, this->group, this->itemCount, this->deadLine, this->title, this->colorTask.name());
}

void task::completionVal(int i)
{
    if(!i) completion->setValue(completion->value()-1);
    else completion->setValue(completion->value()+1);
}

void task::deleteTarget()
{
    itemCount--;
    completionVal(0);
    if(itemCount) completion->setMaximum(itemCount);
    this->db->updateTask(this->wdwId, this->priority, this->duration, this->group, this->itemCount, this->deadLine, this->title, this->colorTask.name());
}

void task::color()
{
    switch (priority)
    {
        case 1:
        this->setStyleSheet("QWidget::title {background: #235B66; color: black;} QWidget{background : rgba( 160, 160, 160, 255); border-radius : 7px;border : 1px solid black;}");
        break;
        case 2:
        this->setStyleSheet("QWidget::title {background: #11AEBF; color: black;} QWidget{background : rgba( 160, 160, 160, 255); border-radius : 7px;border : 1px solid black;}");
        break;
        case 3:
        this->setStyleSheet("QWidget::title {background: #A0BF30; color: black;} QWidget{background : rgba( 160, 160, 160, 255); border-radius : 7px;border : 1px solid black;}");
        break;
        case 4:
        this->setStyleSheet("QWidget::title {background: #F2AE30; color: black;} QWidget{background : rgba( 160, 160, 160, 255); border-radius : 7px;border : 1px solid black;}");
        break;
        case 5:
        this->setStyleSheet("QWidget::title {background: #F25244; color: black;} QWidget{background : rgba( 160, 160, 160, 255); border-radius : 7px;border : 1px solid black;}");
        break;
        default:
        this->setStyleSheet("QWidget::title {background: #546670; color: black;} QWidget{background : rgba( 160, 160, 160, 255); border-radius : 7px;border : 1px solid black;}");
        break;
    }
}

void task::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
            dragStartPosition = event->pos();
}

void task::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
           return;
       if ((event->pos() - dragStartPosition).manhattanLength()
            < QApplication::startDragDistance())
           return;

       QDrag *drag = new QDrag(this);
       QMimeData *mimeData = new QMimeData;
       QByteArray q_b;
       q_b.setNum(this->wdwId.toMSecsSinceEpoch());
       mimeData->setData("application/x-item", q_b);
       drag->setMimeData(mimeData);
       drag->exec();
}

void task::dropEvent(QDropEvent *event)
{
    event->accept();
    this->db->updateTask(this->wdwId, this->priority, this->duration, this->group, this->itemCount, this->deadLine, this->title, this->colorTask.name());
}

void task::set(QDateTime number, database *db, int priority, int duration,
               int tray, int itemCount, QString color, QDateTime deadline, QString title)
{
    this->colorTask = QColor(color);
    this->labelColor->setStyleSheet("background-color:"+this->colorTask.name());
    this->wdwId = number;
    this->priority = priority;
    this->duration = duration;
    this->group = tray;
    this->itemCount = itemCount;
    this->deadLine = deadline;
    this->title = title;
    this->db = db;
    setWindowTitle(title);
}
