#ifndef TARGET_H
#define TARGET_H

#include <QString>
#include <QDateTime>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include "database.h"

class target: public QWidget
{
    Q_OBJECT
public:
    target();
    target(QString targetDesc, database *db, QDateTime parent);
    ~target();
    QCheckBox *c = new QCheckBox;
    QPushButton *b = new QPushButton;
    void set(QDateTime id, bool check, QString title, QDateTime parent, database *db);

public slots:
    void up();

private:
    QString targetDesc;
    QDateTime parent;
    QDateTime wdwId;
    database *db;
};

#endif // TARGET_H
