#ifndef FILTER_H
#define FILTER_H

#include <QtWidgets>
#include <QtSql>

class Filter: public QWidget
{
    Q_OBJECT

public:
    Filter(QWidget *parent = 0);
    virtual ~Filter();
    
    void refresh(const QSqlDatabase &db, const QString &table);
    const QString currentParameter(const QString fieldName) const;
    const QStringList allParameters(const QString fieldName) const;

    /*
    QString stdoutTableName;
    QString drTableName;
    QString speedupTableName;

    QStringList stdout_table_possible_names = {"o", "a", "out"};
    QStringList dr_table_possible_names = {"d", "b", "dr"};
    QStringList speedup_table_possible_names = {"speedup", "s"};
    */

    int dagComboBoxCount = 11;
    QFormLayout *dagComboBoxFormLayout;
    QStringList dagComboBoxNames = {"host", "platform", "input", "exp", "dr", "malloc", "try", "collmax",
                                    "app", "type", "ppn"};
    QVector<QLabel *> dagComboBoxLabels;
    QVector<QComboBox *> dagComboBoxes;

    /*
    QStringList default_fields = {NULL, NULL, NULL, "bodytrack", "task_mth", NULL, "36"};
    QVector<QComboBox *> comboboxes;
    QStringList types = {"pthreads", "openmp", "tbb", "task_cilkplus", "task_mth", "task_omp", "task_qth", "task_tbb"};
    */

    QStringList arrangedFields = {"exp", "type"};
    QStringList arrangedFieldItems[2] = { {"hooks", "dr"},
                                          {"serial", "pthreads", "cilkplus", "mth", "omp", "openmp", "qth", "tbb",
                                           "task_serial", "task_cilkplus", "task_mth", "task_omp", "task_qth", "task_tbb"} };
                                                      

public slots:
    void on_combobox_changed()
    { emit changed(); }

signals:
    void changed();
};

#endif
