#include "statgrapher.h"

StatGrapher::StatGrapher(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout();
    this->setLayout(mainLayout);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setAlignment(Qt::AlignTop);
    mainLayout->addLayout(leftLayout);
    QHBoxLayout *rightLayout = new QHBoxLayout();
    rightLayout->setAlignment(Qt::AlignTop);
    mainLayout->addLayout(rightLayout);

    /* initialize parameter boards */

    {
        parameterBoards[0] = new ParameterBoard(this, {"type", "ppn"});
        parameterBoards[1] = new ParameterBoard(this, {"type", "ppn"});
        parameterBoards[2] = new ParameterBoard(this, {"type"});
        parameterBoards[3] = new ParameterBoard(this, {"ppn"});
    }

    /* table name parameters */
    {
        QGroupBox *groupBox = new QGroupBox();
        leftLayout->addWidget(groupBox);
        groupBox->setTitle("Selecting table names");
        groupBox->setStatusTip("Selecting DB parameters");
        groupBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        QFormLayout *formLayout = new QFormLayout();
        groupBox->setLayout(formLayout);
        for (int i = 0; i < dbComboBoxCount; i++) {
            QComboBox *combobox = new QComboBox();
            dbComboBoxes.append(combobox);
            formLayout->addRow(QString("%1").arg(dbComboBoxNames.at(i)), combobox);
            for (int j = 0; j < dbComboBoxItems[i].count(); j++) {
                combobox->addItem(dbComboBoxItems[i].at(j));
            }
            combobox->setCurrentIndex(0);
        }
    }

    /* filter widget */
    {
        stdoutTableFilter.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        leftLayout->addWidget(&stdoutTableFilter);
    }

    QTabWidget *tabWidget = new QTabWidget();
    rightLayout->addWidget(tabWidget);

    /* tab 0: all graphs */
    {
        QSplitter *splitter1 = new QSplitter();
        splitter1->setOrientation(Qt::Vertical);
        tabWidget->addTab(splitter1, "&All");

        QSplitter *splitter2 = new QSplitter();
        QSplitter *splitter3 = new QSplitter();
        splitter1->addWidget(splitter2);
        splitter1->addWidget(splitter3);
        splitter2->setOrientation(Qt::Horizontal);
        splitter3->setOrientation(Qt::Horizontal);

        splitter2->addWidget(&chartViewsTab0[0]);
        splitter2->addWidget(&chartViewsTab0[1]);
        splitter3->addWidget(&chartViewsTab0[2]);
        splitter3->addWidget(&chartViewsTab0[3]);

        for (int i = 0; i < 4; i++) {
            chartViewsTab0[i].setRenderHint(QPainter::Antialiasing);
        }
    }

    /* tab 1: time graph */
    {
        QWidget *tab = new QWidget;
        QVBoxLayout *tabLayout = new QVBoxLayout;
        tabWidget->addTab(tab, "&Time");
        tab->setLayout(tabLayout);
        tabLayout->setContentsMargins(0, 0, 0, 0);

        {
            tabLayout->addWidget(parameterBoards[0]);
        }

        {
            QWidget *widget = new QWidget();
            QVBoxLayout *vbox = new QVBoxLayout();
            QHBoxLayout *hbox = new QHBoxLayout();
            QPushButton *button = new QPushButton("Export");
            tabLayout->addWidget(widget);
            widget->setLayout(vbox);
            vbox->setContentsMargins(0, 0, 0, 0);
            vbox->addWidget(&chartViewsTabs[0]);
            vbox->addLayout(hbox);
            hbox->setContentsMargins(4, 4, 4, 4);
            hbox->addStretch(1);
            hbox->addWidget(button);
            button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(button, &QPushButton::clicked, this, &StatGrapher::on_exportButton0_clicked);
        }
    }

    /* tab 2: speedup graph */
    {
        QWidget *tab = new QWidget;
        QVBoxLayout *tabLayout = new QVBoxLayout;
        tabWidget->addTab(tab, "S&peedup");
        tab->setLayout(tabLayout);
        tabLayout->setContentsMargins(0, 0, 0, 0);

        {
            tabLayout->addWidget(parameterBoards[1]);
        }

        {
            QWidget *widget = new QWidget();
            QVBoxLayout *vbox = new QVBoxLayout();
            QHBoxLayout *hbox = new QHBoxLayout();
            QPushButton *button = new QPushButton("Export");
            tabLayout->addWidget(widget);
            widget->setLayout(vbox);
            vbox->setContentsMargins(0, 0, 0, 0);
            vbox->addWidget(&chartViewsTabs[1]);
            vbox->addLayout(hbox);
            hbox->setContentsMargins(4, 4, 4, 4);
            hbox->addStretch(1);
            hbox->addWidget(button);
            button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(button, &QPushButton::clicked, this, &StatGrapher::on_exportButton1_clicked);
        }
    }

    /* tab 3: ppn-based breakdown graph */
    {
        QWidget *tab = new QWidget();
        QVBoxLayout *tabLayout = new QVBoxLayout();
        tabWidget->addTab(tab, "&Breakdown 1");
        tab->setLayout(tabLayout);
        tabLayout->setContentsMargins(0, 0, 0, 0);

        {
            tabLayout->addWidget(parameterBoards[2]);
        }

        {
            QWidget *widget = new QWidget();
            QVBoxLayout *vbox = new QVBoxLayout();
            QHBoxLayout *hbox = new QHBoxLayout();
            QPushButton *button = new QPushButton("Export");
            tabLayout->addWidget(widget);
            widget->setLayout(vbox);
            vbox->setContentsMargins(0, 0, 0, 0);
            vbox->addWidget(&chartViewsTabs[2]);
            vbox->addLayout(hbox);
            hbox->setContentsMargins(4, 4, 4, 4);
            hbox->addStretch(1);
            hbox->addWidget(button);
            button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(button, &QPushButton::clicked, this, &StatGrapher::on_exportButton2_clicked);
        }
    }

    /* tab 4: type-based breakdown graph */
    {
        QWidget *tab = new QWidget();
        QVBoxLayout *tabLayout = new QVBoxLayout();
        tabWidget->addTab(tab, "Break&down 2");
        tab->setLayout(tabLayout);
        tabLayout->setContentsMargins(0, 0, 0, 0);

        {
            tabLayout->addWidget(parameterBoards[3]);
        }

        {
            QWidget *widget = new QWidget();
            QVBoxLayout *vbox = new QVBoxLayout();
            QHBoxLayout *hbox = new QHBoxLayout();
            QPushButton *button = new QPushButton("Export");
            tabLayout->addWidget(widget);
            widget->setLayout(vbox);
            vbox->setContentsMargins(0, 0, 0, 0);
            vbox->addWidget(&chartViewsTabs[3]);
            vbox->addLayout(hbox);
            hbox->setContentsMargins(4, 4, 4, 4);
            hbox->addStretch(1);
            hbox->addWidget(button);
            button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(button, &QPushButton::clicked, this, &StatGrapher::on_exportButton3_clicked);
        }
    }

    /* signal connections */
    {
        for (int i = 0; i < dbComboBoxes.count(); i++) {
            QComboBox *combobox = dbComboBoxes.at(i);
            connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_dbComboBoxes_currentIndexChanged(int)));
        }
        connect(&stdoutTableFilter, SIGNAL(changed()), this, SLOT(refresh()));
        connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tabWidget_currentChanged(int)));
        for (int i = 0; i < 4; i++) {
            connect(parameterBoards[i], SIGNAL(selectionChanged()), this, SLOT(refresh()));
        }
    }
}

StatGrapher::~StatGrapher()
{
}

void StatGrapher::on_dbComboBoxes_currentIndexChanged(int index)
{
    (void) index;
    QSqlDatabase db = QSqlDatabase::database(activeDb);
    stdoutTableFilter.refresh(db, dbSelector("stdout table"));
    parameterBoards[0]->refresh(db, dbSelector("stdout table"));
    parameterBoards[1]->refresh(db, dbSelector("speedup table"));
    parameterBoards[2]->refresh(db, dbSelector("dr table"));
    parameterBoards[3]->refresh(db, dbSelector("dr table"));
    refresh();
}

void StatGrapher::setActiveDb(const QString &activeDb)
{
    this->activeDb = activeDb;
    QSqlDatabase db = QSqlDatabase::database(activeDb);

    QStringList tables = db.tables();
    int i;
    /* disconnect db comboboxes */
    for (i = 0; i < dbComboBoxes.count(); i++) {
        QComboBox *combobox = dbComboBoxes.at(i);
        disconnect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_dbComboBoxes_currentIndexChanged(int)));
    }

    /* stdout/dr table */
    i = 0;
    {
        for (int i = 0; i <= 1; i++) {
            for (int j = 0; j < dbComboBoxItems[i].count(); j++) {
                if (tables.contains(dbComboBoxItems[i].at(j))) {
                    QComboBox *combobox = dbComboBoxes.at(i);
                    combobox->setCurrentIndex(j);
                    break;
                }
            }
        }
    }
    
    /* time field */
    i = 2;
    {
        QSqlRecord record = db.record(dbSelector("stdout table"));
        for (int j = 0; j < dbComboBoxItems[i].count(); j++) {
            if (record.contains(dbComboBoxItems[i].at(j))) {
                QComboBox *combobox = dbComboBoxes.at(i);
                combobox->setCurrentIndex(j);
                break;
            }
        }
    }

    /* work field */
    i = 3;
    {
        QSqlRecord record = db.record(dbSelector("dr table"));
        for (int j = 0; j < dbComboBoxItems[i].count(); j++) {
            if (record.contains(dbComboBoxItems[i].at(j))) {
                QComboBox *combobox = dbComboBoxes.at(i);
                combobox->setCurrentIndex(j);
                break;
            }
        }
    }
    
    /* refresh filter before making speedup table */
    Filter &filter = stdoutTableFilter;
    filter.refresh(db, dbSelector("stdout table"));

    /* serial type / task types */
    for (i = 4; i <= 5; i++) {
        QSqlQuery query = QSqlQuery(db);
        query.exec(QString("select distinct type from %1").arg(dbSelector("stdout table")));
        QStringList distinctTypes;
        if (query.isActive()) {
            while (query.next()) {
                distinctTypes.append(query.value(0).toString());
            }
        }
        for (int j = 0; j < dbComboBoxItems[i].count(); j++) {
            if (distinctTypes.contains(dbComboBoxItems[i].at(j))) {
                QComboBox *combobox = dbComboBoxes.at(i);
                combobox->setCurrentIndex(j);
                break;
            }
        }
    }
    
    /* speedup table */
    i = 6;
    {
        int j;
        for (j = 0; j < dbComboBoxItems[i].count(); j++) {
            if (tables.contains(dbComboBoxItems[i].at(j))) {
                QComboBox *combobox = dbComboBoxes.at(i);
                combobox->setCurrentIndex(j);
                break;
            }
        }
        if (j == dbComboBoxItems[i].count()) {
            /* make a temporary speedup table */
            QSqlQuery query = QSqlQuery(db);
            QString queryString = "create temporary table temp_seq as select ";
            bool first_addition = true;
            /* append conditions */
            for (int j = 0; j < filter.dagComboBoxNames.count(); j++) {
                QString field = filter.dagComboBoxNames.at(j);
                if (filter.dagComboBoxes.at(j)->count() > 0
                    && field != "ppn" && field != "type" && field != "dr") {
                    if (first_addition) {
                        first_addition = false;
                    } else {
                        queryString.append(",");
                    }
                    queryString.append(field);
                }
            }
            queryString.append(QString(",%1 as temp_seq from %2 where type=\"%3\" and ppn=1 and dr=0").arg(dbSelector("time field"), dbSelector("stdout table"), dbSelector("serial type")));
            query.exec(queryString);
            if (!query.isActive()) {
                qDebug() << "Error adding temporary temp_seq table:" << query.lastError().text();
            } else {
                queryString = QString("create temporary table temp_seq_2 as select * from %1 natural join temp_seq").arg(dbSelector("stdout table"));
                query.exec(queryString);
                if (!query.isActive()) {
                    qDebug() << "Error joining temporary temp_seq table:" << query.lastError().text();
                } else {
                    queryString = QString("create temporary table speedup as select *,cast(temp_seq as float)/cast(%1 as float) as speedup from temp_seq_2").arg(dbSelector("time field"));
                    query.exec(queryString);
                    if (!query.isActive()) {
                        qDebug() << "Error making temporary speedup table:" << query.lastError().text();
                    } else {
                        QComboBox *combobox = dbComboBoxes.at(i);
                        combobox->setCurrentIndex(0);
                    }
                }
            }
        }
    }
        /*
          create temporary table temp_seq as select host,app,platform,malloc,try,collmax,t as t_seq from a where type="serial" and ppn=1 and dr=0;
          create temporary table temp_seq_2 as select * from a natural join temp_seq;
          create temporary table speedup as select *,cast(t as float)/cast(t_seq as float) as speedup from temp_seq_2;
        */
 
    /* speedup field */
    i = 7;
    {
        QSqlRecord record = db.record(dbSelector("speedup table"));
        for (int j = 0; j < dbComboBoxItems[i].count(); j++) {
            if (record.contains(dbComboBoxItems[i].at(j))) {
                QComboBox *combobox = dbComboBoxes.at(i);
                combobox->setCurrentIndex(j);
                break;
            }
        }
    }
    
    /* refresh parameter boards after speedup table is processed */
    {
        parameterBoards[0]->refresh(db, dbSelector("stdout table"));
        parameterBoards[1]->refresh(db, dbSelector("speedup table"));
        parameterBoards[2]->refresh(db, dbSelector("dr table"));
        parameterBoards[3]->refresh(db, dbSelector("dr table"));
    }
    
    /* re-connect db comboboxes */
    for (i = 0; i < dbComboBoxes.count(); i++) {
        QComboBox *combobox = dbComboBoxes.at(i);
        connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_dbComboBoxes_currentIndexChanged(int)));
    }

    refresh();
}

void StatGrapher::plotRuntimeGraph(QSqlDatabase &db, QChart *chart)
{
    emit statusPosted("plotting...");
    QSqlQuery query = QSqlQuery(db);
    chart->removeAllSeries();
    QString table = dbSelector("stdout table");
    ParameterBoard *filter = parameterBoards[0];
    const QStringList types = filter->currentParameters("type");
    const QStringList ppns = filter->currentParameters("ppn");
    QValueAxis *axis = new QValueAxis();
    axis->setMin(1);
    axis->setLabelFormat("%d");
    axis->setTitleText("cores");
    for (int i = 0; i < types.count(); i++) {
        QString type = types.at(i);
        /* make query string */
        QString queryString = QString("select ppn,%1 from %2 where").arg(dbSelector("time field"), table);
        QSqlRecord record = db.record(table);
        bool first_addition = true;
        /* append conditions */
        for (int j = 0; j < record.count(); j++) {
            QString field = record.fieldName(j);
            if (filter->isValidField(field) && field != "ppn" && field != "type") {
                if (first_addition) {
                    first_addition = false;
                } else {
                    queryString.append(" and");
                }
                if (field == "dr" || field == "try" || field == "collmax") {
                    queryString.append(QString(" %1=%2").arg(field, filter->currentParameter(field)));
                } else {
                    queryString.append(QString(" %1=\"%2\"").arg(field, filter->currentParameter(field)));
                }
            }
        }
        /* append ppn condition */
        if (first_addition) {
            first_addition = false;
        } else {
            queryString.append(" and");
        }
        bool first_ppn_addition = true;
        for (int i = 0; i < ppns.count(); i++) {
            if (first_ppn_addition) {
                first_ppn_addition = false;
                queryString.append(" (");
            } else {
                queryString.append(" or");
            }
            queryString.append(QString(" ppn=%2").arg(ppns.at(i)));
        }
        if (!first_ppn_addition) {
            queryString.append(" )");
        }
        /* append type condition */
        if (first_addition) {
            first_addition = false;
        } else {
            queryString.append(" and");
        }
        queryString.append(QString(" type=\"%2\"").arg(type));
        queryString.append(QString(" order by ppn"));
        /* query */
        emit statusPosted("plotting... query executing...");
        query.exec(queryString);
        emit statusPosted("plotting...");
        if (!query.isActive()) {
            qDebug() << query.lastError().text();
        } else {
            QLineSeries *series = new QLineSeries();
            series->setName(type);
            while (query.next()) {
                int ppn = query.value("ppn").toInt();
                double sec = query.value(dbSelector("time field")).toDouble();
                series->append(ppn, sec);
                if (ppn > axis->max())
                    axis->setMax(ppn);
            }
            if (series->count() > 0) {
                chart->addSeries(series);
            }
            query.finish();
        }
    }
    chart->createDefaultAxes();
    chart->setAxisX(axis);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setTitle("Execution time");
    chart->setAnimationOptions(QChart::NoAnimation);
    /* set max value for y axis */
    if (chart->axisY()) {
        QAbstractAxis *axisY = chart->axisY();
        axisY->setTitleText("seconds");
        axisY->setMin(QVariant(0));
        emit statusPosted("plotting... query executing...");
        query.exec(QString("select max(%1) from %2 where app=\"%3\" group by app").arg(dbSelector("time field"), dbSelector("stdout table"), filter->currentParameter("app")));
        emit statusPosted("plotting...");
        if (!query.isActive()) {
            qDebug() << query.lastError().text();
        } else {
            if (query.first()) {
                axisY->setMax(query.value(0));
            }
        }
        ((QValueAxis *) axisY)->applyNiceNumbers();
        ((QValueAxis *) axisY)->setMinorTickCount(5);
    }
    query.finish();
    emit statusPosted("plotting... finished.");
}

void StatGrapher::plotSpeedupGraph(QSqlDatabase &db, QChart *chart)
{
    emit statusPosted("plotting...");
    QSqlQuery query = QSqlQuery(db);
    chart->removeAllSeries();
    QString table = dbSelector("speedup table");
    ParameterBoard *filter = parameterBoards[1];
    const QStringList types = filter->currentParameters("type");
    const QStringList ppns = filter->currentParameters("ppn");
    QValueAxis *axis = new QValueAxis();
    axis->setMin(1);
    axis->setLabelFormat("%d");
    axis->setTitleText("cores");
    for (int i = 0; i < types.count(); i++) {
        QString type = types.at(i);
        QString queryString = QString("select ppn,%1 from %2 where").arg(dbSelector("speedup field"), table);
        QSqlRecord record = db.record(table);
        bool first_addition = true;
        /* append conditions */
        for (int j = 0; j < record.count(); j++) {
            QString field = record.fieldName(j);
            if (filter->isValidField(field) && field != "ppn" && field != "type") {
                if (first_addition) {
                    first_addition = false;
                } else {
                    queryString.append(" and");
                }
                if (field == "dr" || field == "try" || field == "collmax") {
                    queryString.append(QString(" %1=%2").arg(field, filter->currentParameter(field)));
                } else {
                    queryString.append(QString(" %1=\"%2\"").arg(field, filter->currentParameter(field)));
                }
            }
        }
        /* append ppn condition */
        if (first_addition) {
            first_addition = false;
        } else {
            queryString.append(" and");
        }
        bool first_ppn_addition = true;
        for (int i = 0; i < ppns.count(); i++) {
            if (first_ppn_addition) {
                first_ppn_addition = false;
                queryString.append(" (");
            } else {
                queryString.append(" or");
            }
            queryString.append(QString(" ppn=%2").arg(ppns.at(i)));
        }
        if (!first_ppn_addition) {
            queryString.append(" )");
        }
        /* append type condition */
        if (first_addition) {
            first_addition = false;
        } else {
            queryString.append(" and");
        }
        queryString.append(QString(" type=\"%2\"").arg(type));
        queryString.append(QString(" order by ppn"));
        emit statusPosted("plotting... query executing...");
        query.exec(queryString);
        emit statusPosted("plotting...");
        if (!query.isActive()) {
            qDebug() << query.lastError().text();
        } else {
            QLineSeries *series = new QLineSeries();
            series->setName(type);
            while (query.next()) {
                int ppn = query.value("ppn").toInt();
                double speedup = query.value(dbSelector("speedup field")).toDouble();
                series->append(ppn, speedup);
                if (ppn > axis->max())
                    axis->setMax(ppn);
            }
            if (series->count() > 0) {
                chart->addSeries(series);
            }
            query.finish();
        }
    }
    chart->createDefaultAxes();
    chart->setAxisX(axis);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::NoAnimation);
    if (chart->axisY()) {
        QAbstractAxis *axisY = chart->axisY();
        ((QValueAxis *) axisY)->setLabelFormat("%d");
        axisY->setTitleText("speedup");
        ((QValueAxis *) axisY)->setMinorTickCount(5);
    }
    chart->setTitle("Scalability");
    query.finish();
    emit statusPosted("plotting... finished.");
}

void StatGrapher::plotPpnBreakdownGraph(QSqlDatabase &db, QChart *chart)
{
    emit statusPosted("plotting...");
    QSqlQuery query = QSqlQuery(db);
    chart->removeAllSeries();
    QString table = dbSelector("dr table");
    ParameterBoard *filter = parameterBoards[2];
    const QString ppn = filter->currentParameter("ppn");
    QStackedBarSeries *series = new QStackedBarSeries();
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->setTitleText("types");
    bool toAppendAxis = true;
    for (int i = 0; i < dag_attrs.count(); i++) {
        QString attr = dag_attrs.at(i);
        if (!db.record(table).contains(attr)) {
            continue;
        }
        QBarSet *set = new QBarSet(attr);
        QStringList types = filter->currentParameters("type");
        for (int j = 0; j < types.count(); j++) {
            QString type = types.at(j);
            QString queryString = QString("select %1 from %2 where").arg(attr, table);
            QSqlRecord record = db.record(table);
            bool first_addition = true;
            /* append conditions */
            for (int k = 0; k < record.count(); k++) {
                QString field = record.fieldName(k);
                /* exp is always "dr", dr is always 1, try is always 1 */
                if (filter->isValidField(field)
                    && field != "ppn" && field != "type" && field != "exp" && field != "dr" && field != "try") {
                    if (first_addition) {
                        first_addition = false;
                    } else {
                        queryString.append(" and");
                    }
                    if (field == "collmax") {
                        queryString.append(QString(" %1=%2").arg(field, filter->currentParameter(field)));
                    } else {
                        queryString.append(QString(" %1=\"%2\"").arg(field, filter->currentParameter(field)));
                    }
                }
            }
            /* append other conditions */
            if (first_addition) {
                first_addition = false;
            } else {
                queryString.append(" and");
            }
            queryString.append(QString(" type=\"%1\" and (ppn=%2 or (ppn=1 and type=\"%3\"))").arg(type, ppn, dbSelector("serial type")));
            emit statusPosted("plotting... query executing...");
            query.exec(queryString);
            emit statusPosted("plotting...");
            if (!query.isActive()) {
                qDebug() << query.lastError().text();
            } else {
                if (query.first()) {
                    set->append(query.value(attr).toDouble());
                    if (toAppendAxis) {
                        axis->append(type);
                    }
                }
            }
        }
        series->append(set);
        /* append items to axis in only one iteration of the loop */
        toAppendAxis = false;
    }
    chart->addSeries(series);
    chart->createDefaultAxes();
    //axis->setLabelsAngle(10);
    chart->setAxisX(axis, series);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->setTitle("Breakdown of cumul. execution time");
    chart->setAnimationOptions(QChart::NoAnimation);
    /* set max value for y axis */
    if (chart->axisY()) {
        QAbstractAxis *axisY = chart->axisY();
        ((QValueAxis *) axisY)->setTitleText("clocks");
        ((QValueAxis *) axisY)->setLabelFormat("%.3g");
        emit statusPosted("plotting... query executing...");
        query.exec(QString("select max(%1+delay+nowork) from %2 where app=\"%3\" group by app").arg(dbSelector("work field"), dbSelector("dr table"), filter->currentParameter("app")));
        emit statusPosted("plotting...");
        if (!query.isActive()) {
            qDebug() << query.lastError().text();
        } else {
            if (query.first()) {
                axisY->setMax(query.value(0));
            }
        }
        //((QValueAxis *) axisY)->applyNiceNumbers();
        ((QValueAxis *) axisY)->setMinorTickCount(5);
    }
    query.finish();
    emit statusPosted("plotting... finished.");
}

void StatGrapher::plotTypeBreakdownGraph(QSqlDatabase &db, QChart *chart)
{
    emit statusPosted("plotting...");
    QSqlQuery query = QSqlQuery(db);
    chart->removeAllSeries();
    QString table = dbSelector("dr table");
    ParameterBoard *filter = parameterBoards[3];
    const QString type = filter->currentParameter("type");
    const QStringList ppns = filter->currentParameters("ppn");
    QStackedBarSeries *series = new QStackedBarSeries();
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->setTitleText("cores");
    bool toAppendAxis = true;
    for (int i = 0; i < dag_attrs.count(); i++) {
        QString attr = dag_attrs.at(i);
        if (!db.record(table).contains(attr)) {
            continue;
        }
        QBarSet *set = new QBarSet(attr);
        {
            QString ppn = "1";
            QString type = dbSelector("serial type");
            QString queryString = QString("select %1 from %2 where").arg(attr, table);
            QSqlRecord record = db.record(table);
            bool first_addition = true;
            /* append conditions */
            for (int j = 0; j < record.count(); j++) {
                QString field = record.fieldName(j);
                /* exp is always "dr", dr is always 1, try is always 1 */
                if (filter->isValidField(field)
                    && field != "ppn" && field != "type" && field != "exp" && field != "dr" && field != "try") {
                    if (first_addition) {
                        first_addition = false;
                    } else {
                        queryString.append(" and");
                    }
                    if (field == "collmax") {
                        queryString.append(QString(" %1=%2").arg(field, filter->currentParameter(field)));
                    } else {
                        queryString.append(QString(" %1=\"%2\"").arg(field, filter->currentParameter(field)));
                    }
                }
            }
            /* append other conditions */
            if (first_addition) {
                first_addition = false;
            } else {
                queryString.append(" and");
            }
            queryString.append(QString(" type=\"%1\" and ppn=%2").arg(type, ppn));
            emit statusPosted("plotting... query executing...");
            query.exec(queryString);
            emit statusPosted("plotting...");
            if (!query.isActive()) {
                qDebug() << query.lastError().text();
            } else {
                if (query.first()) {
                    set->append(query.value(attr).toDouble());
                    if (toAppendAxis) {
                        axis->append("serial");
                    }
                }
            }
        }
        for (int j = 0; j < ppns.count(); j++) {
            QString ppn = ppns.at(j);
            QString queryString = QString("select %1 from %2 where").arg(attr, table);
            QSqlRecord record = db.record(table);
            bool first_addition = true;
            /* append conditions */
            for (int j = 0; j < record.count(); j++) {
                QString field = record.fieldName(j);
                /* exp is always "dr", dr is always 1, try is always 1 */
                if (filter->isValidField(field)
                    && field != "ppn" && field != "type" && field != "exp" && field != "dr" && field != "try") {
                    if (first_addition) {
                        first_addition = false;
                    } else {
                        queryString.append(" and");
                    }
                    if (field == "collmax") {
                        queryString.append(QString(" %1=%2").arg(field, filter->currentParameter(field)));
                    } else {
                        queryString.append(QString(" %1=\"%2\"").arg(field, filter->currentParameter(field)));
                    }
                }
            }
            /* append other conditions */
            if (first_addition) {
                first_addition = false;
            } else {
                queryString.append(" and");
            }
            queryString.append(QString(" type=\"%1\" and ppn=%2").arg(type, ppn));
            emit statusPosted("plotting... query executing...");
            query.exec(queryString);
            emit statusPosted("plotting...");
            if (!query.isActive()) {
                qDebug() << query.lastError().text();
            } else {
                if (query.first())
                    set->append(query.value(attr).toDouble());
                if (toAppendAxis) {
                    axis->append(QString(ppn));
                }
            }
        }
        series->append(set);
        /* append items to axis in only one iteration of the loop */
        toAppendAxis = false;
    }
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->setTitle("Breakdown of cumul. execution time");
    chart->setAnimationOptions(QChart::NoAnimation);
    /* set max value for y axis */
    if (chart->axisY()) {
        QAbstractAxis *axisY = chart->axisY();
        emit statusPosted("plotting... query executing...");
        query.exec(QString("select max(%1+delay+nowork) from %2 where app=\"%3\" group by app").arg(dbSelector("work field"), dbSelector("dr table"), filter->currentParameter("app")));
        emit statusPosted("plotting...");
        if (!query.isActive()) {
            qDebug() << query.lastError().text();
        } else {
            if (query.first()) {
                axisY->setMax(query.value(0));
            }
        }
        ((QValueAxis *) axisY)->setLabelFormat("%.3g");
        ((QValueAxis *) axisY)->setMinorTickCount(5);
        axisY->setTitleText("clocks");
    }
    query.finish();
    emit statusPosted("plotting... finished.");
}

void StatGrapher::refresh()
{
    QSqlDatabase db = QSqlDatabase::database(activeDb);
    
    /* time graph */
    {
        plotRuntimeGraph(db, chartViewsTab0[0].chart());
        plotRuntimeGraph(db, chartViewsTabs[0].chart());
    }
    
    /* speedup graph */
    if (db.tables().contains(dbSelector("speedup table"))) {
        plotSpeedupGraph(db, chartViewsTab0[1].chart());
        plotSpeedupGraph(db, chartViewsTabs[1].chart());
    }

    /* ppn-based breakdown graph */
    {
        plotPpnBreakdownGraph(db, chartViewsTab0[2].chart());
        plotPpnBreakdownGraph(db, chartViewsTabs[2].chart());
    }

    /* type-based breakdown graph */
    {
        plotTypeBreakdownGraph(db, chartViewsTab0[3].chart());
        plotTypeBreakdownGraph(db, chartViewsTabs[3].chart());
    }
}

const QString StatGrapher::dbSelector(QString name) const
{
    for (int i = 0; i < dbComboBoxCount; i ++) {
        const QString n = dbComboBoxNames.at(i);
        if (n == name) {
            QComboBox *combobox = dbComboBoxes.at(i);
            return combobox->currentText();
        }
    }
    return "";
}

void StatGrapher::on_tabWidget_currentChanged(int index)
{
    (void) index;
}

void StatGrapher::exportChartToPdf(int chartIndex)
{
    QString pdfFileName = QFileDialog::getSaveFileName(this, tr("Export Chart to PDF"), QString(),
                                                       tr("Pdf Files (*.pdf)"), 0, 0);
    if (!pdfFileName.isEmpty()) {
        QChartView *chartView = &chartViewsTabs[chartIndex];
        QPdfWriter writer(pdfFileName);
        //writer.setPageSize(QPageSize(QSizeF(100.0,100.0), QPageSize::Point, 0, QPageSize::ExactMatch));
        //writer.setPageSize(QPageSize(QSize(100,100), 0, QPageSize::ExactMatch));
        writer.setPageSize(QPageSize(chartView->size(), 0, QPageSize::ExactMatch));
        QPainter painter(&writer);
        chartView->render(&painter);
        painter.end();
    }    
}
