#ifndef STATGRAPHER_H
#define STATGRAPHER_H

#include <QtWidgets>
#include <QtCharts>
#include "filter.h"

class StatGrapher: public QWidget
{
    Q_OBJECT

public:
    StatGrapher(QWidget *parent = 0);
    virtual ~StatGrapher();

signals:
    void statusPosted(const QString &message);

public slots:
    void refresh();
    void setActiveDb(const QString &activeDb);
    void on_dbComboBoxes_currentIndexChanged(int index);
    void on_tabWidget_currentChanged(int index);
    
private:
    QString activeDb;

    const int dbComboBoxCount = 8;
    QVector<QComboBox *> dbComboBoxes;
    QStringList dbComboBoxNames = {"stdout table", "dr table",
                                   "time field", "work field",
                                   "serial type", "task types",
                                   "speedup table", "speedup field"};
    QStringList dbComboBoxItems[8] = { {"o", "a", "out"},
                                       {"d", "b", "dr"},
                                       {"sec", "t"},
                                       {"work", "t1"},
                                       {"task_serial", "serial"},
                                       {"task_mth", "mth"},
                                       {"speedup", "s"},
                                       {"clocks_speedup", "sec_speedup", "speedup"} };
    const QString dbSelector(const QString name) const;

    Filter stdoutTableFilter;

    QChartView chartViewsTab0[4];
    QChartView chartViewsTab1[2];
    QChartView chartViewsTab2[2];

    void plotRuntimeGraph(QSqlDatabase &db, QChart *chart);
    void plotSpeedupGraph(QSqlDatabase &db, QChart *chart);
    void plotPpnBreakdownGraph(QSqlDatabase &db, QChart *chart);
    void plotTypeBreakdownGraph(QSqlDatabase &db, QChart *chart);
};

#endif
