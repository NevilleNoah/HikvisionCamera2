#ifndef TREECHOOSE_H
#define TREECHOOSE_H

#include "database.h"
#include <QDialog>

namespace Ui {
class TreeChoose;
}

class TreeChoose : public QDialog
{
    Q_OBJECT

public:
    explicit TreeChoose(QWidget *parent = nullptr);
    ~TreeChoose();
    void initDatabase();
    void initUI();
    static Database database;

private:
    Ui::TreeChoose *ui;
    TreeChoose* treechoose;
};

#endif // TREECHOOSE_H
