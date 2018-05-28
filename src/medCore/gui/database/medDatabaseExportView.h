#pragma once

#include <medAbstractDatabaseItem.h>
#include <medCoreExport.h>

class medDatabaseExportViewPrivate;
class QGridLayout;
class QLayout;

class MEDCORE_EXPORT medDatabaseExportView : public QObject
{
    Q_OBJECT

public:
    static void exportItem(medAbstractDatabaseItem& item);

signals:


private:
    medDatabaseExportViewPrivate* const d;

    medDatabaseExportView(medAbstractDatabaseItem& databaseItem);

    void exportSingleSeries();
    void exportMultipleSeries();

    void addSelectionWidgets(medAbstractDatabaseItem* item, QLayout* layout);
    void addPatientSelectionWidgets(medAbstractDatabaseItem* item, QLayout* layout);
    void addStudySelectionWidgets(medAbstractDatabaseItem* item, QLayout* layout);
    void addSeriesSelectionWidgets(medAbstractDatabaseItem* item, QGridLayout* layout);

    int numLeafItems(medAbstractDatabaseItem* item);

private slots:
    void selectDataToExport();
    void updateFilenameSuffix(int index);
};
