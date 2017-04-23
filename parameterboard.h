#ifndef PARAMETER_BOARD_H
#define PARAMETER_BOARD_H

#include <QtWidgets>
#include <QtSql>

class ParameterBoard: public QWidget
{
    Q_OBJECT

public:
    ParameterBoard(QWidget *parent = 0, const QStringList &pluralFields = {});
    virtual ~ParameterBoard();
    void refresh(const QSqlDatabase &db, const QString &table);
    const QString currentParameter(const QString &field) const;
    const QStringList currentParameters(const QString &field) const;
    const QStringList allParameters(const QString &field) const;
    bool isValidField(const QString &field) const;

signals:
    void selectionChanged();
        
public slots:
    void on_primaryFieldButtonGroup_buttonClicked(QAbstractButton *button)
    { /*qDebug() << "clicked button:" << button->text();*/ (void) button; emit selectionChanged(); }

private:
    QStringList primaryFields = {"host", "platform", "input", "exp", "dr",
                                 "malloc", "try", "collmax", "app", "type",
                                 "ppn"};
    QVector<QButtonGroup *> primaryFieldButtonGroups;
    QVector<QGroupBox *> primaryFieldGroupBoxes;
    QMap<QString, QStringList> primaryFieldArrangedValues =
        { {"exp", {"hooks", "dr"}},
          {"type", {"serial", "pthreads", "cilkplus", "mth", "omp", "openmp", "qth", "tbb",
                    "task_serial", "task_cilkplus", "task_mth", "task_omp", "task_qth", "task_tbb"}} };
    QMap<QString, QVector<QAbstractButton *>> buttonMap;

    QStringList pluralFields;
};

#endif
