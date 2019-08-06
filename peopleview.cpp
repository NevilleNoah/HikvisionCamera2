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

void PeopleView::on_pushButton_clicked()
{
    addPersonDialog = new AddPersonDialog(this);
    addPersonDialog->setModal(false);
    addPersonDialog->show();
}
