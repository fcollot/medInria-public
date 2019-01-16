#pragma once

#include <medAbstractDatabaseItem.h>
#include <medCoreExport.h>
#include <dtkCore/dtkSmartPointer>
#include <dtkCore/dtkAbstractDataWriter>

class medAbstractData;
class medDatabaseExportViewPrivate;
class QCheckBox;
class QDialog;
class QGridLayout;
class QLayout;
class QLineEdit;
class QProgressDialog;

class MEDCORE_EXPORT medDatabaseExportView : public QObject
{
    Q_OBJECT

public:
    static void exportItem(medAbstractDatabaseItem* databaseItem);

private:
    medDatabaseExportViewPrivate* const d;

    medDatabaseExportView();

    void exportSingleSeries(medAbstractDatabaseItem* databaseItem);

    bool chooseSeriesFilenameAndFormat(medAbstractData* data, QString* outputPath, QString* outputWriter);

    void exportMultipleSeries(medAbstractDatabaseItem* databaseItem);

    QDialog* createDataSelectionDialog(medAbstractDatabaseItem* databaseItem);

    QProgressDialog* createProgressDialog(int numSteps);

    int numLeafItems(medAbstractDatabaseItem* databaseItem);

    QWidget* createSelectionWidget(medAbstractDatabaseItem* databaseItem, QProgressDialog* progressDialog, int checkable = false);

    QString selectionName(medDataIndex& dataIndex);

    QWidget* createSeriesWriterCombo(medDataIndex& dataIndex, QLineEdit* nameEdit);

    void extractChosenSeriesFilenameAndWriter(QWidget* seriesWidget, QString* filename, QString writer);

    //void selectSubfolderName();

    QList<dtkSmartPointer<dtkAbstractDataWriter> > createWriters(medAbstractData* data);

    QStringList getWriterFilters(QList<dtkSmartPointer<dtkAbstractDataWriter> > writers);

    QString getWriterFilter(dtkAbstractDataWriter* writer);

    QString findSuitableFilter(QString extension, QString preferedFilter, QStringList filters);

    bool filterContainsExtension(QString extension, QString filter);
//    void checkSubfolderName(QString name);
    bool chooseExportFilename(QString* outputPath);

private slots:
    void updateFilenameSuffix(int writerIndex);
};
