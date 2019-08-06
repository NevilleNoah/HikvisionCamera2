#include "peopleview.h"
#include "ui_peopleview.h"

PeopleView::PeopleView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PeopleView)
{
    ui->setupUi(this);
}

PeopleView::~PeopleView()
{
    delete ui;
}
