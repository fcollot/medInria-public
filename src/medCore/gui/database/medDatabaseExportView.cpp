#include "medDatabaseExportView.h"

#include <medAbstractDataFactory.h>
#include <medDataManager.h>
#include <medGlobalDefs.h>
#include <medMetaDataKeys.h>
#include <QtGui>

class medDatabaseExportViewPrivate
{
  public:
    QInputDialog* subFolderInputDialog;
    QDir exportFolder;
    QString topLevelName;
};


void medDatabaseExportView::exportItem(medAbstractDatabaseItem* databaseItem)
{
    medDatabaseExportView* exportView = new medDatabaseExportView();

    if (databaseItem->childCount() == 0)
    {
        exportView->exportSingleSeries(databaseItem);
    }
    else
    {
        exportView->exportMultipleSeries(databaseItem);
    }
}

medDatabaseExportView::medDatabaseExportView() :
    d(new medDatabaseExportViewPrivate)
{
}

void medDatabaseExportView::exportSingleSeries(medAbstractDatabaseItem* databaseItem)
{
    medDataIndex dataIndex = databaseItem->dataIndex();
    dtkSmartPointer<medAbstractData> data = medDataManager::instance()->retrieveData(dataIndex);

    QString outputPath, outputWriter;

    if (chooseSeriesFilenameAndFormat(data, &outputPath, &outputWriter))
    {
        medDataManager::instance()->exportData(data, outputPath, outputWriter);
    }
}

bool medDatabaseExportView::chooseSeriesFilenameAndFormat(medAbstractData* data, QString* outputPath, QString* outputWriter)
{
    QFileDialog fileDialog(0, tr("Please choose a file name:"));
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);

    QString defaultName = data->metadata(medMetaDataKeys::SeriesDescription.key());
    fileDialog.selectFile(defaultName);

    QList<dtkSmartPointer<dtkAbstractDataWriter> > writers = createWriters(data);
    QStringList filters = getWriterFilters(writers);
    fileDialog.setNameFilters(filters);

    if (fileDialog.exec())
    {
        *outputPath = fileDialog.selectedFiles().first().toUtf8();
        QString extension = QFileInfo(*outputPath).suffix();
        QString chosenFilter = fileDialog.selectedNameFilter();

        QString suitableFilter = findSuitableFilter(extension, chosenFilter, filters);

        if (!suitableFilter.isNull())
        {
            dtkAbstractDataWriter* writer = writers[filters.indexOf(suitableFilter)];
            *outputWriter = writer->identifier();

            return true;
        }
    }

    return false;
}

void medDatabaseExportView::exportMultipleSeries(medAbstractDatabaseItem* databaseItem)
{
    QDialog* selectionDialog = createDataSelectionDialog(databaseItem);

    if (selectionDialog->exec())
    {

    }

    delete selectionDialog;
}

QDialog* medDatabaseExportView::createDataSelectionDialog(medAbstractDatabaseItem* databaseItem)
{
    QDialog* dialog = new QDialog();
    dialog->setLayout(new QVBoxLayout());

    QProgressDialog* progressDialog = createProgressDialog(numLeafItems(databaseItem));
    dialog->layout()->addWidget(createSelectionWidget(databaseItem, progressDialog));
    delete progressDialog;

    QWidget* buttonsWidget = new QWidget();
    dialog->layout()->addWidget(buttonsWidget);
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsWidget->setLayout(buttonsLayout);

    QPushButton* cancelButton = new QPushButton("Cancel");
    buttonsLayout->addWidget(cancelButton, 0, Qt::AlignRight);
    connect(cancelButton, SIGNAL(clicked(bool)), dialog, SLOT(reject()));

    QPushButton* continueButton = new QPushButton("Continue");
    buttonsLayout->addWidget(continueButton, 0, Qt::AlignRight);
    connect(continueButton, SIGNAL(clicked(bool)), dialog, SLOT(accept()));

    return dialog;
}

QProgressDialog* medDatabaseExportView::createProgressDialog(int numSteps)
{
    QProgressDialog* progressDialog = new QProgressDialog("Loading information, please wait...", QString(), 0, numSteps);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setValue(progressDialog->minimum());
    progressDialog->setMinimumDuration(0);
    return progressDialog;
}

int medDatabaseExportView::numLeafItems(medAbstractDatabaseItem* databaseItem)
{
    if (databaseItem->childCount() == 0)
    {
        return 1;
    }
    else
    {
        int result = 0;

        for (int i = 0; i < databaseItem->childCount(); i++)
        {
            result += numLeafItems(databaseItem->child(i));
        }

        return result;
    }
}

QWidget* medDatabaseExportView::createSelectionWidget(medAbstractDatabaseItem* databaseItem, QProgressDialog* progressDialog, int checkable)
{
    medDataIndex dataIndex = databaseItem->dataIndex();

    QWidget* widget = new QWidget();
    QGridLayout* layout = new QGridLayout();
    widget->setLayout(layout);
    //layout->setColumnMinimumWidth(1, 300);
    layout->setColumnStretch(3, 1);

    QCheckBox* checkBox = new QCheckBox();
    layout->addWidget(checkBox, 0, 0);
    checkBox->setChecked(true);

    QString name = selectionName(dataIndex);

    QLabel* nameLabel = new QLabel(name);
    layout->addWidget(nameLabel, 0, 1);
    nameLabel->setFixedWidth(300);
    //nameLabel->setAlignment(Qt::AlignLeft);
    connect(checkBox, SIGNAL(toggled(bool)), nameLabel, SLOT(setEnabled(bool)));

    QLineEdit* filename = new QLineEdit(name);
    layout->addWidget(filename, 0, 2);
    filename->setFixedWidth(300);
    //filename->setAlignment(Qt::AlignLeft);
    connect(checkBox, SIGNAL(toggled(bool)), filename, SLOT(setEnabled(bool)));

    if (dataIndex.isValidForSeries())
    {
        QWidget* writerWidget = createSeriesWriterCombo(dataIndex, filename);
        layout->addWidget(writerWidget, 0, 3);
        connect(checkBox, SIGNAL(toggled(bool)), writerWidget, SLOT(setEnabled(bool)));
        QComboBox* writerCombo = dynamic_cast<QComboBox*>(writerWidget);

        if (writerCombo)
        {
            writerCombo->setCurrentIndex(-1);
            writerCombo->setCurrentIndex(0);
        }
        else
        {
            checkBox->setChecked(false);
            checkBox->setEnabled(false);
        }

        progressDialog->setValue(progressDialog->value() + 1);
    }
    else
    {
        for (int i = 0; i < databaseItem->childCount(); i++)
        {
            QWidget* childSelectionWidget = createSelectionWidget(databaseItem->child(i), progressDialog, true);
            layout->addWidget(childSelectionWidget, i + 1, 1, 1, 3);
            connect(checkBox, SIGNAL(toggled(bool)), childSelectionWidget, SLOT(setEnabled(bool)));
        }
    }

    return widget;
}

QString medDatabaseExportView::selectionName(medDataIndex& dataIndex)
{
    QString nameKey;

    if (dataIndex.isValidForSeries())
    {
        nameKey = medMetaDataKeys::SeriesDescription.key();
    }
    else if (dataIndex.isValidForStudy())
    {
        nameKey = medMetaDataKeys::StudyDescription.key();
    }
    else
    {
        nameKey = medMetaDataKeys::PatientName.key();
    }

    return medDataManager::instance()->getMetaData(dataIndex, nameKey);
}

QWidget* medDatabaseExportView::createSeriesWriterCombo(medDataIndex& dataIndex, QLineEdit* nameEdit)
{
    dtkSmartPointer<medAbstractData> data = medDataManager::instance()->retrieveData(dataIndex);

    if (data == nullptr)
    {
        return new QLabel(QString("Unable to load file"));
    }
    else
    {
        QList<dtkSmartPointer<dtkAbstractDataWriter> > writers = createWriters(data);
        data = nullptr;
        medDataManager::instance()->garbageCollect();

        if (!writers.isEmpty())
        {
            QComboBox* writersComboBox = new QComboBox();

            foreach (dtkAbstractDataWriter* writer, writers)
            {
                QString extension = writer->supportedFileExtensions().first();
                QString filter = getWriterFilter(writer);
                writersComboBox->addItem(filter, extension);
                writersComboBox->setItemData(writersComboBox->count() - 1, QVariant::fromValue<QObject*>(nameEdit), Qt::UserRole + 1);
                writersComboBox->setItemData(writersComboBox->count() - 1, writer->identifier(), Qt::UserRole + 2);
            }

            connect(writersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilenameSuffix(int)));
            return writersComboBox;
        }
        else
        {
            return new QLabel(QString("No writers found"));
        }
    }
}

void medDatabaseExportView::extractChosenSeriesFilenameAndWriter(QWidget* seriesWidget, QString* filename, QString writer)
{
    QLineEdit* filenameWidget = seriesWidget->findChild<QLineEdit*>();
    *filename = filenameWidget->text();

    QComboBox* writerCombo = seriesWidget->findChild<QComboBox*>();
    QString extension = QFileInfo(*filename).suffix();
    QString chosenFilter = writerCombo->currentText();

    QString suitableFilter = findSuitableFilter(extension, chosenFilter, filters);

    if (!suitableFilter.isNull());
}

//void medDatabaseExportView::selectSubfolderName()
//{
//    d->subFolderInputDialog = new QInputDialog();
//    d->subFolderInputDialog->setInputMode(QInputDialog::TextInput);
//    d->subFolderInputDialog->setTextValue(d->topLevelName);
//    connect(d->subFolderInputDialog, SIGNAL(textValueChanged(QString)), this, SLOT(checkSubfolderName(QString)));

//    if (d->subFolderInputDialog->exec())
//    {

//    }
//}

QList<dtkSmartPointer<dtkAbstractDataWriter> > medDatabaseExportView::createWriters(medAbstractData* data)
{
    QList<dtkSmartPointer<dtkAbstractDataWriter> > writers;

    foreach (QString writerType, medDataManager::instance()->getPossibleWriters(data))
    {
        writers.append(medAbstractDataFactory::instance()->writer(writerType));
    }

    return writers;
}

QStringList medDatabaseExportView::getWriterFilters(QList<dtkSmartPointer<dtkAbstractDataWriter> > writers)
{
    QStringList filters;

    foreach (dtkAbstractDataWriter* writer, writers)
    {
        filters.append(getWriterFilter(writer));
    }

    return filters;
}

QString medDatabaseExportView::getWriterFilter(dtkAbstractDataWriter* writer)
{
    QStringList extensions;

    foreach (QString extension, writer->supportedFileExtensions())
    {
        extension.replace(QRegExp("^\\*?\\.?"), "*.");
        extensions.append(extension);
    }

    return writer->description() + " (" + extensions.join(" ") + ")";
}

QString medDatabaseExportView::findSuitableFilter(QString extension, QString preferedFilter, QStringList filters)
{
    if (filterContainsExtension(extension, preferedFilter))
    {
        return preferedFilter;
    }
    else
    {
        foreach (QString filter, filters)
        {
            if (filterContainsExtension(extension, filter))
            {
                return filter;
            }
        }

        return QString();
    }
}

bool medDatabaseExportView::filterContainsExtension(QString extension, QString filter)
{
    QRegExp regExp("(?:\\*\\.([^\\s\\)]+))");
    int position = 0;

    while ((position = regExp.indexIn(filter, position)) != -1)
    {
        if (extension == regExp.cap(1))
        {
            return true;
        }

        position += regExp.matchedLength();
    }

    return false;
}

//void medDatabaseExportView::checkSubfolderName(QString name)
//{
//    QString labelText = "Enter the name of the subfolder to create:";
//    QDir subFolder = QDir(d->exportFolder);

//    if (d->exportFolder.exists(name) || subFolder.cd(name))
//    {
//        labelText += "\n(This name already exists)";
//    }

//    d->subFolderInputDialog->setLabelText(labelText);
//}

void medDatabaseExportView::extractDataToTemporaryFolder()
{
    QString tempPath = QDir::tempPath();
    QString exportFolder;

    do
    {
        exportFolder = tempPath + "/" + QUuid::createUuid().toString().replace("{", "").replace("}", "");
    }
    while (QFileInfo(exportFolder).exists());


}

bool medDatabaseExportView::chooseExportFilename(QString* outputPath)
{
//    QFileDialog* fileDialog = new QFileDialog(0, tr("Select the directory:"));
//    //fileDialog->setFileMode(QFileDialog::Directory);
//    fileDialog->setFileMode(QFileDialog::AnyFile);
//    fileDialog->setAcceptMode(QFileDialog::AcceptSave);

//    if (fileDialog->exec())
//    {
//        d->exportFolder = fileDialog->selectedFiles().first();
//        selectSubfolderName();
//    }




    QFileDialog fileDialog(0, tr("Please choose a file name:"));
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);

//        QString defaultName = data->metadata(medMetaDataKeys::SeriesDescription.key());
//        fileDialog.selectFile(defaultName);

//        QList<dtkSmartPointer<dtkAbstractDataWriter> > writers = createWriters(data);
//        QStringList filters = getWriterFilters(writers);
//        fileDialog.setNameFilters(filters);

//        if (fileDialog.exec())
//        {
//            *outputPath = fileDialog.selectedFiles().first().toUtf8();
//            QString extension = QFileInfo(*outputPath).suffix();
//            QString chosenFilter = fileDialog.selectedNameFilter();

//            QString suitableFilter = findSuitableFilter(extension, chosenFilter, filters);

//            if (!suitableFilter.isNull())
//            {
//                dtkAbstractDataWriter* writer = writers[filters.indexOf(suitableFilter)];
//                *outputWriter = writer->identifier();

//                return true;
//            }
//        }

        return false;
}

void medDatabaseExportView::updateFilenameSuffix(int writerIndex)
{
//    QRegExp regExp(".*\\(\\*(\\.\\S+)");

//    if (regExp.indexIn(filter) > -1)
//    {
//        QString extension = regExp.cap(1);
//        QFileDialog* fileDialog = static_cast<QFileDialog*>(sender());
//        //fileDialog->setDefaultSuffix(extension);
//        qDebug() << fileDialog->selectedFiles();
//        QString filename = QFileInfo(fileDialog->selectedFiles().first()).baseName();
//        fileDialog->selectFile(filename + extension);
//    }

    if (writerIndex >= 0)
    {
        QComboBox* writersComboBox = static_cast<QComboBox*>(sender());
        QString extension = writersComboBox->itemData(writerIndex).toString();
        QLineEdit* nameEdit = static_cast<QLineEdit*>(writersComboBox->itemData(writerIndex, Qt::UserRole + 1).value<QObject*>());
        QString filename = QFileInfo(nameEdit->text()).baseName();
        nameEdit->setText(filename + extension);
    }
}
