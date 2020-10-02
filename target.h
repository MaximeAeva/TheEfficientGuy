#ifndef TARGET_H
#define TARGET_H

#include <QString>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include "database.h"

class target: public QWidget
{
    Q_OBJECT
public:
    target(QString targetDesc);
    static int ResID;
    QCheckBox *c = new QCheckBox;
    QPushButton *b = new QPushButton;

public slots:
    void rm();

private:
    int wdwId;
};

#endif // TARGET_H
