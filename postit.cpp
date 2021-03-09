#include "postit.h"

postit::postit(QWidget *parent, database *datab, QPoint p, QString text, int id)
{
    this->db = datab;
    this->id = id;
    this->setStyleSheet("border:1px solid #66767A;background-color:#FFDF7D;");
    quit->setText("X");
    quit->setStyleSheet("QPushButton {color: #B07309;border:none;}");
    this->setMinimumSize(20, 20);
    layTest1->addStretch(1);
    labelTest->setText(text);
    QFont font;
    font.setPixelSize(20);
    font.setBold(true);
    labelTest->setFont(font);
    layTest1->addWidget(quit);
    labelTest->setStyleSheet("QLabel {color: #B07309;border:none;}");
    layTest->addItem(layTest1);
    layTest->addWidget(labelTest);
    this->setLayout(layTest);
    this->setParent(parent);
    this->move(p);
    this->show();
    this->setMinimumSize(200, 200);
    this->adjustSize();
    if(!id)
        this->id = db->addNote(this->pos().x(), this->pos().y(), this->labelTest->text());
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
    db->deleteNote(id);
    this->id=-1;
    this->~postit();
}

void postit::mousePressEvent(QMouseEvent *event)
{
    ref = event->pos();
    this->raise();
}

void postit::mouseReleaseEvent(QMouseEvent *)
{
    db->updateNote(id, this->pos().x(), this->pos().y(), this->labelTest->text());
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

void postit::mouseDoubleClickEvent(QMouseEvent *)
{
    QDialog *d = new QDialog;
    QGridLayout *layout = new QGridLayout;
    QTextEdit *t = new QTextEdit;
    t->setText(labelTest->text());
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
        db->updateNote(id, this->pos().x(), this->pos().y(), t->toPlainText());
    }
}

void postit::paintEvent(QPaintEvent *)
{
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
};
