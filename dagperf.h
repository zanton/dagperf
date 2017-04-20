#ifndef DAGPERF_H
#define DAGPERF_H

#include <QtCore>
#include <QtWidgets>
#include "browser.h"
#include "statgrapher.h"

class DAGPerf : public QMainWindow
{
    Q_OBJECT
    
public:
    DAGPerf(QWidget *parent = 0, QApplication *app = 0);
                                                       
public slots:
    void about();
    void OnStat1Pressed();
    void OnStat2Pressed();
    void postStatus(const QString &status)
    { statusBar()->showMessage(status); }
    void on_browser_databaseActivated(const QString &activeDb)
    { statgrapher.setActiveDb(activeDb); }
    
private:
    QApplication *app;
    Browser browser;
    StatGrapher statgrapher;
    QFormLayout *formLayout;
};

#endif // DAGPERF_H
