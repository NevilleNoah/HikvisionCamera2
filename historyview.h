#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include <QWidget>
#include "database.h"
namespace  Ui{
class HistoryView;
}

class HistoryView : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryView(QWidget *parent = nullptr);
    ~HistoryView();

    void initTable();
    void allData();
    static Database db;
    static QList<RECORD> records;
    static QDateTime startDateTime;
    static QDateTime endDateTime;

signals:

public slots:
    void showByDateTimeRange();
private slots:
    void on_pushButton_clicked();

private:
    Ui::HistoryView *ui;
};

#endif // HISTORYVIEW_H
