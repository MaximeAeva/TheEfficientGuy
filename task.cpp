#include "task.h"

task::task()
{
    designTitleBar();
    this->time->setText("Remain: ");
    this->aloc->setText("Alloc: ");
    alloc->setFormat("%vhours (%p%)");
    projTim->setFormat("%vdays (%p%)");


    projTim->setRange(0, 0);
    alloc->setRange(0, 0);
    this->labelColor->setStyleSheet("border-radius : none; border : none;"
                                    " background-color:"+this->colorTask.name()+"; font-weight : 300; font-size : 14px;");

    this->labelColor->setMinimumHeight(20);
    this->labelColor->setMinimumWidth(20);
    this->stateBar->addWidget(this->labelColor);
    this->stateBar->addStretch(1);
    this->stateBar->addWidget(this->time);
    this->stateBar->addWidget(projTim);
    this->stateBar->addStretch(1);
    this->stateBar->addWidget(this->aloc);
    this->stateBar->addWidget(alloc);
    aloc->setStyleSheet("border : none");
    time->setStyleSheet("border : none");
    projTim->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #6AE68D;font-weight : 300; font-size : 14px;}");
    alloc->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #6AE68D;font-weight : 300; font-size : 14px;}");
    lab->setStyleSheet("QWidget{color: black;background-color :rgb( 49, 54, 63);"
                       "font-weight :300; font-size : 14px;border-radius : 7px;border : 1px solid black;} QCheckBox{border:none;}");
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetFloatable);

    this->layout->addItem(this->stateBar);
    this->layout->setAlignment(Qt::AlignTop);
    lab->setLayout(layout);
    this->setWidget(lab);
    this->lab->setMaximumHeight(0);

}

task::task(database *db, QWidget *parent) :
    QDockWidget(parent)
{
    designTitleBar();
    this->time->setText("Remain: ");
    this->aloc->setText("Alloc: ");


    alloc->setFormat("%vhours (%p%)");
    projTim->setFormat("%vdays (%p%)");
    aloc->setStyleSheet("border : none");
    time->setStyleSheet("border : none");
    this->projTim->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #6AE68D;font-weight : 300; font-size : 14px;}");
    this->alloc->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #6AE68D;font-weight : 300; font-size : 14px;}");
    this->db = db;
    db->addTask(this->wdwId, this->priority, this->duration, this->group, this->itemCount, this->deadLine, this->title, this->colorTask.name());
    lab->setStyleSheet("QWidget{color: black; background-color :rgb( 49, 54, 63);"
                       "font-weight : 300; font-size : 14px;border-radius : 7px;border : 1px solid black;}");
    this->setFeatures(this->features() & ~QDockWidget::DockWidgetFloatable);

    this->completion->setFormat(this->title + " : %p%");
    projTim->setRange(0, 0);
    alloc->setRange(0, 0);
    this->labelColor->setStyleSheet("border-radius : none; border : none; background-color:"+this->colorTask.name());

    this->labelColor->setMinimumHeight(20);
    this->labelColor->setMinimumWidth(20);
    this->stateBar->addWidget(this->labelColor);
    this->stateBar->addStretch(1);
    this->stateBar->addWidget(this->time);
    this->stateBar->addWidget(projTim);
    this->stateBar->addStretch(1);
    this->stateBar->addWidget(this->aloc);
    this->stateBar->addWidget(alloc);
    this->layout->addItem(this->stateBar);
    this->layout->setAlignment(Qt::AlignTop);
    lab->setLayout(layout);
    this->setWidget(lab);
    this->lab->setMaximumHeight(0);
    color();
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

void task::mouseReleaseEvent(QMouseEvent *event)
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
    QLabel *t5 = new QLabel;
    QLineEdit *task = new QLineEdit;
    QPushButton *button = new QPushButton;
    QSlider *slide = new QSlider;
    QLabel *slideVal = new QLabel;
    QSpinBox *spin = new QSpinBox;
    QColorDialog *colorD = new QColorDialog;
    QPushButton *opCol = new QPushButton;
    QDateEdit *deadl = new QDateEdit;
    opCol->setFocusPolicy(Qt::NoFocus);
    task->setFocus();
    deadl->setDate(this->deadLine.date());
    deadl->setCalendarPopup(true);
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
    t5->setText("Deadline");
    opCol->setText("Color");
    button->setText("OK");
    layout->addWidget(t2, 1, 0);
    layout->addWidget(task, 1, 1);
    layout->addWidget(t1, 0, 0);
    layout->addWidget(title, 0, 1);
    layout->addWidget(t3, 2, 0);
    layout->addItem(secondLay, 2, 1);
    layout->addWidget(t4, 3, 0);
    layout->addWidget(spin, 3, 1);
    layout->addWidget(t5, 4, 0);
    layout->addWidget(deadl, 4, 1);
    layout->addWidget(opCol, 5, 0);
    layout->addWidget(button, 6, 0);
    connect(opCol, SIGNAL(clicked()), colorD, SLOT(open()));
    d->setLayout(layout);
    d->setWindowTitle("Content");
    connect(button, SIGNAL(clicked()), d, SLOT(accept()));
    int dialogCode = d->exec();
    if(dialogCode == QDialog::Accepted)
    {
        if(colorD->currentColor()!=this->colorTask) this->colorTask = colorD->selectedColor();
        this->labelColor->setStyleSheet("background-color:"+this->colorTask.name());
        if(!title->text().isEmpty())
        {
            this->setWindowTitle(title->text());
            this->title = title->text();
            this->completion->setFormat(this->title + " : %p%");
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
        this->deadLine = QDateTime(deadl->date());
        projTim->setMaximum(this->wdwId.daysTo(deadLine));
        alloc->setMaximum(this->duration);
        if(projTim->maximum()<=0)
        {
            projTim->setValue(0);
            projTim->setFormat("illimited");
            this->projTim->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #6AE68D;font-weight : 300; font-size : 14px;}");
        }
        else if(QDate::currentDate().daysTo(deadLine.date())>0)
        {
            projTim->setValue(QDate::currentDate().daysTo(deadLine.date()));
            projTim->setFormat("%vdays (%p%)");
            if(projTim->value()<= 0.1*projTim->maximum())
                this->projTim->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #FAB96F;font-weight : 300; font-size : 14px;}");
            else this->projTim->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #6AE68D;font-weight : 300; font-size : 14px;}");
        }
        else
        {
            projTim->setValue(projTim->maximum());
            projTim->setFormat(QString::number(QDate::currentDate().daysTo(deadLine.date()))+"days ("+
                               QString::number(100*(QDate::currentDate().daysTo(deadLine.date())/projTim->maximum()))+"%)");
            this->projTim->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #FF6861;font-weight : 300; font-size : 14px;}");
        }
        if(this->db->isOverkilled(this->wdwId)<=0)
        {
            alloc->setValue(this->db->getAlloc(this->wdwId));
            alloc->setFormat("%vhours (%p%)");
            if (alloc->value()>= 0.8*alloc->maximum())
                this->alloc->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #FAB96F;font-weight : 300; font-size : 14px;}");
            else this->alloc->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #6AE68D;font-weight : 300; font-size : 14px;}");
        }
        else
        {
            alloc->setValue(alloc->maximum());
            alloc->setFormat(QString::number(this->db->getAlloc(this->wdwId))+"hours ("
                             +QString::number(100*this->db->getAlloc(this->wdwId)/alloc->maximum())+"%)");
            this->alloc->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #FF6861;font-weight : 300; font-size : 14px;}");
        }

        this->labelColor->setStyleSheet("border-radius : none; border : none; background-color:"+this->colorTask.name());


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
    QString s;
    this->setStyleSheet(" QDockWidget{font-weight : 600;color: black;"
                        "background-color : rgb( 49, 54, 63); border-radius : 7px;"
                        "border : 1px solid black;font-size : 12pt;}");
    switch (priority)
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
    this->setStyleSheet(this->titleBarWidget()->styleSheet().append("QWidget {  background-color: "+s+";"
                            " color : rgb( 49, 54, 63);font-weight : 600;font-size : 12pt;}"));
    QColor c = QColor(s);
    c = c.lighter();
    this->completion->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: "+c.name()+";}");
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
    this->labelColor->setStyleSheet("border-radius : none; border : none; background-color:"+this->colorTask.name());
    this->wdwId = number;
    this->priority = priority;
    this->duration = duration;
    this->group = tray;
    this->itemCount = itemCount;
    this->deadLine = deadline;
    this->title = title;

    this->db = db;
    this->completion->setFormat(this->title + " : %p%");
    projTim->setMaximum(this->wdwId.daysTo(deadLine));
    alloc->setMaximum(this->duration);
    if(projTim->maximum()<=0)
    {
        projTim->setValue(0);
        projTim->setFormat("illimited");
        this->projTim->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #6AE68D;font-weight : 300; font-size : 14px;}");
    }
    else if(QDate::currentDate().daysTo(deadLine.date())>0)
    {
        projTim->setValue(QDate::currentDate().daysTo(deadLine.date()));
        projTim->setFormat("%vdays (%p%)");
        if(projTim->value()<= 0.1*projTim->maximum())
            this->projTim->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #FAB96F;font-weight : 300; font-size : 14px;}");
        else this->projTim->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #6AE68D;font-weight : 300; font-size : 14px;}");
    }
    else
    {
        projTim->setValue(projTim->maximum());
        projTim->setFormat(QString::number(QDate::currentDate().daysTo(deadLine.date()))+"days ("+
                           QString::number(100*(QDate::currentDate().daysTo(deadLine.date())/projTim->maximum()))+"%)");
        this->projTim->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #FF6861;font-weight : 300; font-size : 14px;}");
    }
    if(this->db->isOverkilled(this->wdwId)<=0)
    {
        alloc->setValue(this->db->getAlloc(this->wdwId));
        alloc->setFormat("%vhours (%p%)");
        if (alloc->value()>= 0.8*alloc->maximum())
            this->alloc->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #FAB96F;font-weight : 300; font-size : 14px;}");
        else this->alloc->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #6AE68D;font-weight : 300; font-size : 14px;}");
    }
    else
    {
        alloc->setValue(alloc->maximum());
        alloc->setFormat(QString::number(this->db->getAlloc(this->wdwId))+"hours ("
                         +QString::number(100*this->db->getAlloc(this->wdwId)/alloc->maximum())+"%)");
        this->alloc->setStyleSheet("QProgressBar{border: none;} QProgressBar::chunk{background-color: #FF6861;font-weight : 300; font-size : 14px;}");
    }
}

void task::hideShowWid()
{
    if(this->lab->height() != 0)
        this->lab->setMaximumHeight(0);
    else
        this->lab->setMaximumHeight(50*(itemCount+1));
}

void task::dlTask()
{
    this->~task();
}

void task::archi()
{
    this->db->archive(this->wdwId, (this->active+1)%2);
    this->close();
}

void task::designTitleBar()
{
    QHBoxLayout* layout = new QHBoxLayout();
    QPushButton *hideShow = new QPushButton;
    QPushButton *archive = new QPushButton;
    QPushButton *quit = new QPushButton;
    connect(hideShow, SIGNAL(clicked()), this, SLOT(hideShowWid()));
    connect(archive, SIGNAL(clicked()), this, SLOT(archi()));
    connect(quit, SIGNAL(clicked()), this, SLOT(dlTask()));

    QIcon icon = titleBar->style()->standardIcon(QStyle::SP_TitleBarMaxButton, 0, titleBar);
    QIcon icon2 = titleBar->style()->standardIcon(QStyle::SP_TitleBarCloseButton, 0, titleBar);
    hideShow->setStyleSheet("QPushButton::hover{background-color : rgba(255, 255, 255, 200);border-radius : none;}");
    archive->setStyleSheet("QPushButton::hover{background-color : rgba(255, 255, 255, 200);border-radius : none;}");
    quit->setStyleSheet("QPushButton::hover{background-color : rgba(255, 50, 50, 200);border-radius : none;}");
    hideShow->setIcon( icon );
    archive->setText("Ar");
    quit->setIcon( icon2 );
    hideShow->adjustSize();
    quit->adjustSize();
    completion->setOrientation(Qt::Horizontal);
    completion->setValue(0);
    completion->setRange(0, 1);
    this->completion->setFormat(this->title + " : %p%");

    layout->addWidget(completion);
    layout->addWidget(hideShow);
    layout->addWidget(archive);
    layout->addWidget(quit);
    this->titleBar->setLayout(layout);
    this->setTitleBarWidget(titleBar);
}

void task::setArchived()
{
    this->active = 0;
}

void task::resizeIt(int s)
{
    this->titleBar->setMaximumWidth(s);
    this->lab->setMaximumWidth(s);
    this->completion->setMaximumWidth(2*s/3);
    QList<target *> trays = this->findChildren<target *>();
    foreach(target* T, trays)
    {
        T->c->setMaximumWidth(s-30);
    }
}
