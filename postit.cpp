#include "postit.h"

postit::postit(QWidget *parent, QPoint p)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Background, QColor("#FFDF7D"));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    quit->setText("X");
    layTest1->addStretch(1);
    labelTest->setText("Note");
    QFont font;
    font.setPixelSize(18);
    labelTest->setFont(font);
    layTest1->addWidget(quit);
    labelTest->setStyleSheet("QLabel {color: black;background-color:#FFDF7D;}");
    layTest->addItem(layTest1);
    layTest->addWidget(labelTest);
    this->setLayout(layTest);
    this->setParent(parent);
    this->move(p);
    this->show();
    this->setMinimumSize(200, 200);
    this->adjustSize();
    connect(quit, SIGNAL(clicked()), this, SLOT(dl()));
}

postit::~postit()
{
    layTest = NULL;
    delete layTest;
    labelTest = NULL;
    delete labelTest;
}

void postit::dl()
{
    this->~postit();
}

void postit::mousePressEvent(QMouseEvent *event)
{
    ref = event->pos();
}

void postit::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
           return;
       if ((event->pos() - ref).manhattanLength()
            < 10)
           return;

       this->move(this->pos()+event->pos()-ref);
}

void postit::mouseDoubleClickEvent(QMouseEvent *event)
{
    QDialog *d = new QDialog;
    QGridLayout *layout = new QGridLayout;
    QTextEdit *t = new QTextEdit;
    QPushButton *ok = new QPushButton;
    ok->setText("OK");
    QPushButton *canc = new QPushButton;
    canc->setText("Cancel");
    connect(ok, SIGNAL(clicked()), d, SLOT(accept()));
    connect(canc, SIGNAL(clicked()), d, SLOT(reject()));
    layout->addWidget(t, 0, 0, 1, 2);
    layout->addWidget(ok, 1, 0);
    layout->addWidget(canc, 1, 1);
    d->setLayout(layout);
    d->setWindowTitle("Edit box");
    int dialogCode = d->exec();
    if(dialogCode == QDialog::Accepted)
    {
        this->labelTest->setText(t->toPlainText());
        this->adjustSize();
    }
}
