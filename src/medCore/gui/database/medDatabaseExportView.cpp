#include "medDatabaseExportView.h"

#include <medAbstractDataFactory.h>
#include <medDataManager.h>
#include <medGlobalDefs.h>
#include <medMetaDataKeys.h>
#include <QtGui>

class medDatabaseExportViewPrivate
{
  public:
    medAbstractDatabaseItem& databaseItem;
    QProgressDialog* progressDialog;

    medDatabaseExportViewPrivate(medAbstractDatabaseItem& databaseItem) :
        databaseItem(databaseItem),
        progressDialog(nullptr)
    {
    }
};


void medDatabaseExportView::exportItem(medAbstractDatabaseItem& item)
{
    medDatabaseExportView* exportView = new medDatabaseExportView(item);

    if (item.childCount() == 0)
    {

    }
    else
    {
        exportView->exportMultipleSeries();
    }
}

medDatabaseExportView::medDatabaseExportView(medAbstractDatabaseItem &databaseItem) :
    d(new medDatabaseExportViewPrivate(databaseItem))
{
}

void medDatabaseExportView::exportSingleSeries()
{

}

void medDatabaseExportView::exportMultipleSeries()
{
    d->progressDialog = new QProgressDialog("Please wait...", QString(), 0, numLeafItems(&d->databaseItem));
    d->progressDialog->setWindowModality(Qt::WindowModal);
    d->progressDialog->setValue(1);
    QThread* thread = new QThread();
    connect(thread, SIGNAL(started()), this, SLOT(selectDataToExport()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    thread->start();
}

void medDatabaseExportView::addSelectionWidgets(medAbstractDatabaseItem* item, QLayout* layout)
{
    medDataIndex dataIndex = item->dataIndex();

    if (dataIndex.isValidForSeries())
    {
        addSeriesSelectionWidgets(item, static_cast<QGridLayout*>(layout));
        d->progressDialog->setValue(d->progressDialog->value() + 1);
    }
    else
    {
        if (dataIndex.isValidForStudy())
        {
            addStudySelectionWidgets(item, layout);
        }
        else
        {
            if (dataIndex.isValidForPatient())
            {
                addPatientSelectionWidgets(item, layout);
            }
        }
    }
}

void medDatabaseExportView::addPatientSelectionWidgets(medAbstractDatabaseItem* item, QLayout* layout)
{
    QCheckBox* checkBox = new QCheckBox();
    layout->addWidget(checkBox);
    checkBox->setChecked(true);

    QString patientName = medDataManager::instance()->getMetaData(item->dataIndex(), medMetaDataKeys::PatientName.key());
    QLineEdit* patientNameLineEdit = new QLineEdit(patientName);
    layout->addWidget(patientNameLineEdit);
    connect(checkBox, SIGNAL(toggled(bool)), patientNameLineEdit, SLOT(setEnabled(bool)));

    QWidget* studiesWidget = new QWidget();
    layout->addWidget(studiesWidget);
    QVBoxLayout* studiesLayout = new QVBoxLayout();
    studiesWidget->setLayout(studiesLayout);
    connect(checkBox, SIGNAL(toggled(bool)), studiesWidget, SLOT(setEnabled(bool)));

    for (int i = 0; i < item->childCount(); i++)
    {
        addSelectionWidgets(item->child(i), studiesLayout);
    }
}

void medDatabaseExportView::addStudySelectionWidgets(medAbstractDatabaseItem* item, QLayout* layout)
{
    QCheckBox* checkBox = new QCheckBox();
    layout->addWidget(checkBox);
    checkBox->setChecked(true);

    QString studyDescription = medDataManager::instance()->getMetaData(item->dataIndex(), medMetaDataKeys::StudyDescription.key());
    QLineEdit* folderNameLineEdit = new QLineEdit(studyDescription);
    layout->addWidget(folderNameLineEdit);
    connect(checkBox, SIGNAL(toggled(bool)), folderNameLineEdit, SLOT(setEnabled(bool)));

    QWidget* seriesWidget = new QWidget();
    layout->addWidget(seriesWidget);
    QGridLayout* seriesLayout = new QGridLayout();
    seriesWidget->setLayout(seriesLayout);
    seriesLayout->setColumnMinimumWidth(1, 300);
    connect(checkBox, SIGNAL(toggled(bool)), seriesWidget, SLOT(setEnabled(bool)));

    for (int i = 0; i < item->childCount(); i++)
    {
        addSelectionWidgets(item->child(i), seriesLayout);
    }
}

void medDatabaseExportView::addSeriesSelectionWidgets(medAbstractDatabaseItem* item, QGridLayout* layout)
{
    int gridIndex = layout->rowCount();

    QCheckBox* checkBox = new QCheckBox();
    layout->addWidget(checkBox, gridIndex, 0);
    checkBox->setChecked(true);

    QString seriesDescription = medDataManager::instance()->getMetaData(item->dataIndex(), medMetaDataKeys::SeriesDescription.key());
    QLineEdit* filenameLineEdit = new QLineEdit(seriesDescription);
    layout->addWidget(filenameLineEdit, gridIndex, 1);
    connect(checkBox, SIGNAL(toggled(bool)), filenameLineEdit, SLOT(setEnabled(bool)));

    dtkSmartPointer<medAbstractData> data = medDataManager::instance()->retrieveData(item->dataIndex());
    QHash<QString, dtkAbstractDataWriter*> possibleWriters = medDataManager::instance()->getPossibleWriters(data);
    data = nullptr;
    medDataManager::instance()->garbageCollect();

    if (!possibleWriters.isEmpty())
    {
        QComboBox* writersComboBox = new QComboBox();
        layout->addWidget(writersComboBox, gridIndex, 2);
        connect(checkBox, SIGNAL(toggled(bool)), writersComboBox, SLOT(setEnabled(bool)));

        // we use allWriters as the list of keys to make sure we traverse possibleWriters
        // in the order specified by the writers priorities.
        QList<QString> allWriters = medAbstractDataFactory::instance()->writers();

        foreach(QString writer, allWriters)
        {
            if (possibleWriters.contains(writer))
            {
                QStringList extensions = possibleWriters[writer]->supportedFileExtensions();
                QString extensionsLabel = possibleWriters[writer]->description() + " (" + extensions.join(", ") + ")";
                writersComboBox->addItem(extensionsLabel, extensions.first());
                writersComboBox->setItemData(writersComboBox->count() - 1, QVariant::fromValue<QObject*>(filenameLineEdit), Qt::UserRole + 1);
            }
        }

        connect(writersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilenameSuffix(int)));
        writersComboBox->setCurrentIndex(-1);
        writersComboBox->setCurrentIndex(0);
    }
    else
    {
        layout->addWidget(new QLabel(QString("(No writers found for ") + data->identifier() + ")"),  gridIndex, 2);
        checkBox->setChecked(false);
        checkBox->setEnabled(false);
    }
}

int medDatabaseExportView::numLeafItems(medAbstractDatabaseItem* item)
{
    if (item->childCount() == 0)
    {
        return 1;
    }
    else
    {
        int result = 0;

        for (int i = 0; i < item->childCount(); i++)
        {
            result += numLeafItems(item->child(i));
        }

        return result;
    }
}

void medDatabaseExportView::selectDataToExport()
{
    QDialog dialog;
    dialog.setLayout(new QVBoxLayout());

    addSelectionWidgets(&d->databaseItem, dialog.layout());

    QWidget* buttonsWidget = new QWidget();
    dialog.layout()->addWidget(buttonsWidget);
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsWidget->setLayout(buttonsLayout);

    QPushButton* continueButton = new QPushButton("Continue");
    QPushButton* cancelButton = new QPushButton("Cancel");
    buttonsLayout->addWidget(cancelButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(continueButton, 0, Qt::AlignRight);

    delete d->progressDialog;
    d->progressDialog = nullptr;
    dialog.exec();
}

void medDatabaseExportView::updateFilenameSuffix(int index)
{
    if (index >= 0)
    {
        QComboBox* writersCombo = dynamic_cast<QComboBox*>(sender());
        QString extension = writersCombo->itemData(index, Qt::UserRole).toString();
        QLineEdit* filenameLineEdit = dynamic_cast<QLineEdit*>(writersCombo->itemData(index, Qt::UserRole + 1).value<QObject*>());

        QString filename = med::smartBaseName(filenameLineEdit->text());
        filename += extension;
        filenameLineEdit->setText(filename);
    }
}
