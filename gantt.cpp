#include "gantt.h"

gantt::gantt(database *db)
{
    this->db = db;
    connect(table, SIGNAL(pressed(const QModelIndex &)), this, SLOT(getter(const QModelIndex &)));
}

void gantt::build(QStringList lst, int col, int dayLength[7])
{
    int row = lst.length();
    QStringList headerH;
    QStringList headerV;
    this->table->setRowCount(row);
    this->table->setColumnCount(col);
    int day;
    for(int i = 0; i<row; i++)
    {
        std::string s = lst.at(i).toStdString();
        const char *a = s.c_str();
        headerV << a;
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
        headerH << a;
    }

    this->table->setHorizontalHeaderLabels(headerH);
    this->table->setVerticalHeaderLabels(headerV);


    int da;
    int w = this->table->width()/(col+1);

    for(int i = 0; i<col; i++)
    {

        da = QDate::currentDate().addDays(i).dayOfWeek();
        if(this->table->width()/(col+1)<(50*dayLength[da-1])) w = 50*dayLength[da-1];
        if (dayLength[da-1] == 0) w = 50;
        tableDelegate *delegate = new tableDelegate(dayLength[da-1], w, this->table);
        this->table->setItemDelegate(new StarDelegate);


        this->table->setColumnWidth(i, w);
        this->table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    }

    for(int i = 0; i<row; i++)
    {
        for(int j = 0; j<col; j++)
        {
            QTableWidgetItem *item3 = new QTableWidgetItem;
            item3->setData(0,
                           qVariantFromValue(StarRating(i)));
            table->setItem(i, j, item3);
        }
    }

}


void gantt::getter(const QModelIndex &index)
{
    int col = this->table->currentIndex().column();
    int row = this->table->currentIndex().row();

    std::cout << col << ", " << row << ": " << index.column() << std::endl;
}


