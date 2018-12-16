#ifndef HOUSEVIEW_H
#define HOUSEVIEW_H

#include <QWidget>
#include "database.h"
#include <QtCore>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>

namespace Ui {
class HouseView;
}

class HouseView : public QWidget
{
    Q_OBJECT

public:
    explicit HouseView(QWidget *parent = nullptr);
    ~HouseView();

private:
    Ui::HouseView *ui;
    static Database database;
    static QList<House> houses;

    static int front;
    static int end;

    void initDatabase();
    void initTable(int row, int col);
    void setHouse();
    void changeHouseStatus();

};

#endif // HOUSEVIEW_H
