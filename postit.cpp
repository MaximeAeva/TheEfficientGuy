#include "postit.h"

postit::postit(QWidget *parent, QPoint p)
{
    this->setStyleSheet("border:1px solid #66767A;background-color:#FFDF7D;");
    quit->setText("X");
    quit->setStyleSheet("QPushButton {color: #BFA75E;border:none;}");
    this->setMinimumSize(20, 20);
    layTest1->addStretch(1);
    labelTest->setText("Note");
    QFont font;
    font.setPixelSize(18);
    labelTest->setFont(font);
    layTest1->addWidget(quit);
    labelTest->setStyleSheet("QLabel {color: black;border:none;}");
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
    this->raise();
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

void postit::paintEvent(QPaintEvent *pe)
{
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
};
