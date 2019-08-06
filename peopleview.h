#ifndef PEOPLEVIEW_H
#define PEOPLEVIEW_H

#include <QWidget>

namespace Ui {
class PeopleView;
}

class PeopleView : public QWidget
{
    Q_OBJECT

public:
    explicit PeopleView(QWidget *parent = nullptr);
    ~PeopleView();

private:
    Ui::PeopleView *ui;
};

#endif // PEOPLEVIEW_H
