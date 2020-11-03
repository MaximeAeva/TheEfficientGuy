#include "gantt.h"

gantt::gantt(database *db)
{
    this->db = db;
}

void gantt::build(QStringList lst, int col, int dayLength[7])
{
    int row = lst.length();
    QStandardItemModel *model = new QStandardItemModel(row, col);
    int day;
    for(int i = 0; i<row; i++)
    {
        std::string s = lst.at(i).toStdString();
        const char *a = s.c_str();
        model->setHeaderData(i, Qt::Vertical, QObject::tr(a));
    }
    for(int i = 0; i<col; i++)
    {
        std::string s = QDate::currentDate().addDays(i).toString("dd-MM-yyyy").toStdString();
        std::string k;
        day = QDate::currentDate().addDays(i).dayOfWeek();
        switch (day) {
        case 1:
            k = "Mon "+s;
            break;
        case 2:
            k = "Tue "+s;
            break;
        case 3:
            k = "Wed "+s;
            break;
        case 4:
            k = "Thu "+s;
            break;
        case 5:
            k = "Fri "+s;
            break;
        case 6:
            k = "Sat "+s;
            break;
        case 7:
            k = "Sun "+s;
            break;
        }
        const char *a = k.c_str();
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(a));
    }

    this->tableView->setModel(model);

    int da;
    int w = this->tableView->width()/(col+1);

    for(int i = 0; i<col; i++)
    {

        da = QDate::currentDate().addDays(i).dayOfWeek();
        if(this->tableView->width()/(col+1)<(50*dayLength[da-1])) w = 50*dayLength[da-1];
        if (dayLength[da-1] == 0) w = 50;
        tableDelegate *delegate = new tableDelegate(dayLength[da-1], w, this->tableView);
        this->tableView->setItemDelegateForColumn(i, delegate);

        this->tableView->setColumnWidth(i, w);
        this->tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    }

    for(int i = 0; i<row; i++)
    {
        for(int j = 0; j<col; j++)
        {
            QWidget *chil = this->tableView->childAt(i+1, j+1);
            QTableView *mt = static_cast<QTableView*>(chil);
            connect(mt , SIGNAL(pressed(const QModelIndex &)), this, SLOT(getter(const QModelIndex &)));
            QModelIndex index = model->index(i, j, QModelIndex());
            this->tableView->openPersistentEditor(index);
        }
    }

}


void gantt::getter(const QModelIndex &index)
{
    int col = this->tableView->currentIndex().column();
    int row = this->tableView->currentIndex().row();
    std::cout << col << ", " << row << ": " << index.column() << std::endl;
}


