#include "dagperf.h"
#include <QtSql>

DAGPerf::DAGPerf(QWidget *parent, QApplication *app)
    : QMainWindow(parent), app(app)
{
    QSplitter *mainWidget = new QSplitter();
    setCentralWidget(mainWidget);
    mainWidget->setOrientation(Qt::Horizontal);

    /* actions */
    QAction *openAct = new QAction(tr("&Open a DAG database..."), this);
    {
        const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
        openAct->setIcon(openIcon);
        openAct->setShortcut(QKeySequence::Open);
        openAct->setStatusTip(tr("Press to add another DAG database"));
        openAct->setToolTip(tr("Add an existing DAG database"));
        connect(openAct, &QAction::triggered, &browser, &Browser::addDatabase);
        addAction(openAct);
    }

    /* browser widget */
    {
        mainWidget->addWidget(&browser);
        /* not effective inside a splitter */
        /*
        QSizePolicy sizePolicy = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        browser.setSizePolicy(sizePolicy);
        */
    }

    /* statgrapher widget */
    {
        /*
        QWidget *container = new QWidget;
        mainWidget->addWidget(container);
        QSizePolicy sizePolicy = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(3);
        container->setSizePolicy(sizePolicy);
        QVBoxLayout *layout = new QVBoxLayout();
        container->setLayout(layout);
        layout->setSpacing(5);
    
        QHBoxLayout *hbox = new QHBoxLayout();
        layout->addLayout(hbox);
        hbox->setSpacing(5);

        QPushButton *helloBtn = new QPushButton("Hello");
        QPushButton *goodbyeBtn = new QPushButton("Goodbye");
        QPushButton *quitBtn = new QPushButton("Quit");
        hbox->addWidget(helloBtn);
        hbox->addWidget(goodbyeBtn);
        hbox->addWidget(quitBtn);
        connect(helloBtn, &QPushButton::clicked, this, &DAGPerf::OnStat1Pressed);
        connect(goodbyeBtn, &QPushButton::clicked, this, &DAGPerf::OnStat2Pressed);
        connect(quitBtn, &QPushButton::clicked, qApp, &QApplication::quit);

        layout->addWidget(&statgrapher);
        */

        mainWidget->addWidget(&statgrapher);
    }

    mainWidget->setSizes({200, 800});

    /* menubar, statusbar, toolbar */
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *helpMenu = menuBar()->addMenu(QObject::tr("&Help"));
    statusBar();
    {
        fileMenu->addAction(openAct);
        //fileMenu->addAction(tr("Add &database..."), &browser, SLOT(addDatabase()));
        fileMenu->addSeparator();
        fileMenu->addAction(tr("&Quit"), this->app, &QApplication::quit);

        helpMenu->addAction(QObject::tr("&About"), this, SLOT(about()));
        helpMenu->addAction(QObject::tr("About &Qt"), this->app, SLOT(aboutQt()));
    }
    //QToolBar *toolbar = addToolBar("Toolbar");
    {        
        //toolbar->addAction(openAct);
    }

    /* signal-slot connections */
    //connect(&browser, SIGNAL(statusPosted(QString)), statusBar(), SLOT(showMessage(QString)));
    //QMetaObject::connectSlotsByName(this);
    connect(&browser, SIGNAL(statusPosted(QString)), this, SLOT(postStatus(QString)));
    connect(&browser, SIGNAL(statusCleared()), this, SLOT(clearStatus()));
    connect(&statgrapher, SIGNAL(statusPosted(QString)), this, SLOT(postStatus(QString)));
    connect(&statgrapher, SIGNAL(statusCleared()), this, SLOT(clearStatus()));
    connect(&browser, SIGNAL(databaseActivated(QString)), this, SLOT(on_browser_databaseActivated(QString)));
}

void DAGPerf::about()
{
    QMessageBox::about(this, tr("About"), tr("DAGPerf tool "
                                             "shows statistical graphs derived from computation DAG traces, "
                                             "visualizes these DAGs, "
                                             "and analyzes scalability factors "
                                             "contributing to the performance loss of parallel programs."));
}
