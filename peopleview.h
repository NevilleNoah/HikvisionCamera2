#ifndef PEOPLEVIEW_H
#define PEOPLEVIEW_H

#include <QWidget>
#include "addpersondialog.h"
namespace Ui {
class PeopleView;
}

class PeopleView : public QWidget
{
    Q_OBJECT

public:
    explicit PeopleView(QWidget *parent = nullptr);
    ~PeopleView();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PeopleView *ui;
    AddPersonDialog *addPersonDialog;
};

#endif // PEOPLEVIEW_H
