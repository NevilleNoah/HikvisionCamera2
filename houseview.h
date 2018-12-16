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

    static int row;
    static int col;

    static int rowSum;
    static int colSum;

    void initDatabase();
    void initTable();
    void setHouse();
    void changeHouseStatus();

};

#endif // HOUSEVIEW_H
