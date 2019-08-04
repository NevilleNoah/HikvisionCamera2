#ifndef EXCELUTIL_H
#define EXCELUTIL_H

#include <QObject>

class ExcelUtil : public QObject
{
     Q_OBJECT
public:
    explicit ExcelUtil(QObject *parent = nullptr);
    void setPath(QString det);

signals:
    //void finish();
    //void errorSignal(QString strMsg);
    //void progressData(int nValue, QString str);

private:
    void getData();
    void writeSheet();

public slots:
    void startTrans();

private:
    QList<QList<QVariant>> m_oDate;
    QString detFileName;
};

#endif // EXCELUTIL_H
