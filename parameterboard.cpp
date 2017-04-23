#include "parameterboard.h"

ParameterBoard::ParameterBoard(QWidget *parent, const QStringList &pluralFields)
    : QWidget(parent), pluralFields(pluralFields)
{
    QVBoxLayout *preLayout = new QVBoxLayout;
    preLayout->setAlignment(Qt::AlignTop);
    this->setLayout(preLayout);
    QGroupBox *groupBox = new QGroupBox;
    groupBox->setTitle("Parameter Board");
    groupBox->setStatusTip("Select parameter(s) for each field");
    groupBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    preLayout->addWidget(groupBox);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setAlignment(Qt::AlignLeft);
    groupBox->setLayout(mainLayout);

    for (int i = 0; i < primaryFields.count(); i++) {
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->setAlignment(Qt::AlignTop);
        mainLayout->addLayout(vbox);
        QString field = primaryFields.at(i);
        QButtonGroup *buttonGroup = new QButtonGroup(this);
        primaryFieldButtonGroups.append(buttonGroup);
        QGroupBox *groupBox = new QGroupBox(field);
        groupBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        groupBox->setLayout(new QVBoxLayout);
        primaryFieldGroupBoxes.append(groupBox);
        vbox->addWidget(groupBox);
        connect(buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(on_primaryFieldButtonGroup_buttonClicked(QAbstractButton*)));
    }
}

ParameterBoard::~ParameterBoard()
{
}

void ParameterBoard::refresh(const QSqlDatabase &db, const QString &table)
{
    QSqlQuery query = QSqlQuery(db);
    QSqlRecord record = db.record(table);

    for (int i = 0; i < primaryFields.count(); i++) {
        QString field = primaryFields.at(i);
        QButtonGroup *buttonGroup = primaryFieldButtonGroups.at(i);
        QGroupBox *groupBox = primaryFieldGroupBoxes.at(i);
        /* remove current buttons */
        disconnect(buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(on_primaryFieldButtonGroup_buttonClicked(QAbstractButton*)));
        QVector<QAbstractButton *> &buttons = buttonMap[field];
        for (int i = 0; i < buttons.count(); i++) {
            QAbstractButton *button = buttons.at(i);
            buttonGroup->removeButton(button);
            delete button;
        }
        buttons.clear();
        /* add new buttons */
        if (record.contains(field)) {
            query.exec(QString("select distinct %1 from %2 order by %1").arg(field, table));
            if (!query.isActive()) {
                qDebug() << "Query Error: " << query.lastError().text();
            } else {
                /* get all distinct values of field */
                QStringList values;
                while (query.next()) {
                    values.append(query.value(0).toString());
                }
                /* add values to buttonGroup and groupBox */
                if (primaryFieldArrangedValues.contains(field)) {
                    /* get list of arranged items of the field */
                    const QStringList &arrangedValues = primaryFieldArrangedValues.value(field);
                    for (int i = 0; i < arrangedValues.count(); i++) {
                        const QString &value = arrangedValues.at(i);
                        if (values.contains(value)) {
                            /* create buttons */
                            QAbstractButton *button;
                            if (pluralFields.contains(field)) {
                                button = new QCheckBox;
                            } else {
                                button = new QRadioButton;
                            }
                            button->setText(value);
                            buttonGroup->addButton(button);
                            groupBox->layout()->addWidget(button);
                            buttonMap[field].append(button);
                        }
                    }
                } else {
                    for (int i = 0; i < values.count(); i++) {
                        const QString &value = values.at(i);
                        QAbstractButton *button;
                        if (pluralFields.contains(field)) {
                            button = new QCheckBox;
                        } else {
                            button = new QRadioButton;
                        }
                        button->setText(value);
                        buttonGroup->addButton(button);
                        groupBox->layout()->addWidget(button);
                        buttonMap[field].append(button);
                    }
                }
                //buttonGroup->button(0)->setChecked(true);
                if (pluralFields.contains(field)) {
                    buttonGroup->setExclusive(false);
                    for (int i = 0; i < buttonMap[field].count(); i++) {
                        buttonMap[field][i]->setChecked(true);
                    }
                } else {
                    buttonMap[field][0]->setChecked(true);
                }
            }            
        }
        connect(buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(on_primaryFieldButtonGroup_buttonClicked(QAbstractButton*)));
                
    }
}

/* return first checked value */
const QString ParameterBoard::currentParameter(const QString &field) const
{
    const QVector<QAbstractButton *> &buttons = buttonMap[field];
    for (int i = 0; i < buttons.count(); i++) {
        if (buttons.at(i)->isChecked()) {
            return buttons.at(i)->text();
        }
    }
    return 0;
}

/* return a list of all checked values */
const QStringList ParameterBoard::currentParameters(const QString &field) const
{
    const QVector<QAbstractButton *> &buttons = buttonMap[field];
    QStringList checkedValues;
    for (int i = 0; i < buttons.count(); i++) {
        if (buttons.at(i)->isChecked()) {
            checkedValues.append(buttons.at(i)->text());
        }
    }
    return checkedValues;
}

/* return a list of all values */
const QStringList ParameterBoard::allParameters(const QString &field) const
{
    const QVector<QAbstractButton *> &buttons = buttonMap[field];
    QStringList values;
    for (int i = 0; i < buttons.count(); i++) {
        values.append(buttons.at(i)->text());
    }
    return values;
}

bool ParameterBoard::isValidField(const QString &field) const
{
    if (primaryFields.contains(field)) {
        return true;
    } else {
        return false;
    }
}
