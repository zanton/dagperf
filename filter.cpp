#include "filter.h"

Filter::Filter(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);
    mainLayout->setAlignment(Qt::AlignTop);

    /* DAG parameters */
    {
        QGroupBox *groupBox = new QGroupBox();
        mainLayout->addWidget(groupBox);
        groupBox->setTitle("Selecting fields");
        groupBox->setStatusTip("Selecting DAG parameters");
        groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        dagComboBoxFormLayout = new QFormLayout();
        groupBox->setLayout(dagComboBoxFormLayout);
        for (int i = 0; i < dagComboBoxNames.count(); i++) {
            QString name = dagComboBoxNames.at(i);
            QLabel *label = new QLabel(QString("<b>%1</b>").arg(name));
            QComboBox *combobox = new QComboBox();
            dagComboBoxLabels.append(label);
            dagComboBoxes.append(combobox);
            dagComboBoxFormLayout->addRow(label, combobox);
            connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combobox_changed()));
        }
    }
}

Filter::~Filter()
{
}

void Filter::refresh(const QSqlDatabase &db, const QString &table)
{
    QSqlQuery query = QSqlQuery(db);
    QSqlRecord record = db.record(table);
                
    for (int i = 0; i < dagComboBoxNames.count(); i++) {
        QString name = dagComboBoxNames.at(i);
        QLabel *label = dagComboBoxLabels.at(i);
        QComboBox *combobox = dagComboBoxes.at(i);
        disconnect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combobox_changed()));
        while (combobox->count() > 0) {
            combobox->removeItem(0);
        }
        /* add distinct values of column 'name' from table 'table' */
        if (record.contains(name)) {
            query.exec(QString("select distinct %1 from %2 order by %1").arg(name, table));
            if (!query.isActive()) {
                qDebug() << "Query Error: " << query.lastError().text();
            } else {
                QStringList values;
                while (query.next()) {
                    values.append(query.value(0).toString());
                }
                /* arrange the list as predefined */
                if (arrangedFields.contains(name)) {
                    int pos = arrangedFields.indexOf(name);
                    QStringList *arrangedList = &arrangedFieldItems[pos];
                    for (int i = 0; i < arrangedList->count(); i++) {
                        if (values.contains(arrangedList->at(i))) {
                            combobox->addItem(arrangedList->at(i));
                        }
                    }
                    if (combobox->count() < values.count()) {
                        for (int i = 0; i < values.count(); i++) {
                            if (!arrangedList->contains(values.at(i))) {
                                combobox->addItem(values.at(i)); 
                            }
                        }
                    }
                } else {
                    combobox->addItems(values);
                }
            }
            if (combobox->currentIndex() == -1) {
                combobox->setCurrentIndex(0);
            }
            if (combobox->count() > 1) {
                label->setText(QString("<b>%1</b>").arg(name));
            } else {
                label->setText(name);
            }
        } else {
            label->setText(name);
        }
        connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combobox_changed()));
    }
}

const QString Filter::currentParameter(QString fieldName) const
{
    for (int i = 0; i < dagComboBoxCount; i ++) {
        const QString name = dagComboBoxNames.at(i);
        if (name == fieldName) {
            QComboBox *combobox = dagComboBoxes.at(i);
            return combobox->currentText();
        }
    }
    return "";
}

const QStringList Filter::allParameters(QString fieldName) const
{
    QStringList itemList;
    for (int i = 0; i < dagComboBoxCount; i ++) {
        const QString name = dagComboBoxNames.at(i);
        if (name == fieldName) {
            QComboBox *combobox = dagComboBoxes.at(i);
            for (int j = 0; j < combobox->count(); j++) {
                itemList.append(combobox->itemText(j));
            }
            break;
        }
    }
    return itemList;
}
