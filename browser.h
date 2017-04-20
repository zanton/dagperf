#ifndef BROWSER_H
#define BROWSER_H

#include <QtWidgets>
#include <QtSql>
#include "ui_browser.h"

class ConnectionWidget;

class Browser: public QWidget, public Ui::Browser
{
    Q_OBJECT
    
public:
    Browser(QWidget *parent = 0);
    virtual ~Browser();
    
    QSqlError addConnection(const QString &driver, const QString &dbName, const QString &host,
                            const QString &user, const QString &passwd, int port = -1);
    
    void insertRow();
    void deleteRow();
    void updateActions();
    QSqlDatabase currentDatabase() const;
    
signals:
    void statusPosted(const QString &message);
    void databaseActivated(const QString &activeDb);

public slots:
    void exec();
    void showTable(const QString &table);
    void showMetaData(const QString &table);
    void addDatabase();
    void currentChanged() { updateActions(); }
    
    void on_insertRowAction_triggered()
    { insertRow(); }
    void on_deleteRowAction_triggered()
    { deleteRow(); }
    void on_fieldStrategyAction_triggered();
    void on_rowStrategyAction_triggered();
    void on_manualStrategyAction_triggered();
    void on_submitAction_triggered();
    void on_revertAction_triggered();
    void on_selectAction_triggered();

    void on_connectionWidget_tableActivated(const QString &table)
    { showTable(table); }
    void on_connectionWidget_metaDataRequested(const QString &table)
    { showMetaData(table); }
    void on_connectionWidget_databaseActivated(const QString &activeDb)
    { emit databaseActivated(activeDb); }
    void on_connectionWidget_addDatabaseRequested()
    { addDatabase(); }

    void on_submitButton_clicked()
    {
        exec();
        sqlEdit->setFocus();
    }
    void on_clearButton_clicked()
    {
        sqlEdit->clear();
        sqlEdit->setFocus();
    }
};

class CustomModel: public QSqlTableModel
{
    Q_OBJECT
public:
    explicit CustomModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase()):QSqlTableModel(parent, db) {}
    QVariant data(const QModelIndex &idx, int role) const override
    {
        if (role == Qt::BackgroundRole && isDirty(idx))
            return QBrush(QColor(Qt::yellow));
        return QSqlTableModel::data(idx, role);
    }
};

#endif
