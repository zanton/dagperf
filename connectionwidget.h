#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QtWidgets>
#include <QtSql>

class ConnectionWidget: public QWidget
{
    Q_OBJECT
    
public:
    ConnectionWidget(QWidget *parent = 0);
    virtual ~ConnectionWidget();
    QSqlDatabase currentDatabase() const;
    
public slots:
    void refresh();
    void showMetaData();
    void on_tree_itemActivated(QTreeWidgetItem *item, int column);
    void on_tree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    
signals:
    void tableActivated(const QString &table);
    void metaDataRequested(const QString &tableName);
    void databaseActivated(const QString &activeDb);
    void addDatabaseRequested();
                                                    
private:
    void setActive(QTreeWidgetItem *);
    
    QTreeWidget *tree;
    QAction *metaDataAction;
    QString activeDb;
    QPushButton *addBtn;
};

#endif
