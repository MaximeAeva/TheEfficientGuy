#ifndef TARGET_H
#define TARGET_H

#include <QString>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>

class target: public QWidget
{
    Q_OBJECT
public:
    target(QString targetDesc);
    QCheckBox *c = new QCheckBox;
    QPushButton *b = new QPushButton;
};

#endif // TARGET_H
