/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medDatabaseExporter.h>

#include <medAbstractData.h>
#include <dtkCore/dtkAbstractDataWriter.h>
#include <medAbstractDataFactory.h>
#include <medAbstractDataWriter.h>

class medDatabaseExporterPrivate
{
public:
    QList<dtkSmartPointer<medAbstractData> > data;
    QList<QString> paths;
    QList<QString> writers;

    medDatabaseExporterPrivate(QList<dtkSmartPointer<medAbstractData> > data, QList<QString> paths, QList<QString> writers) :
        data(data), paths(paths), writers(writers)
    {
    }
};

medDatabaseExporter::medDatabaseExporter(dtkSmartPointer<medAbstractData> data, QString path, QString writer) :
    d(new medDatabaseExporterPrivate({data}, {path}, {writer}))
{
}

medDatabaseExporter::medDatabaseExporter(QList<dtkSmartPointer<medAbstractData> > data, QList<QString> paths, QList<QString> writers) :
    d(new medDatabaseExporterPrivate(data, paths, writers))
{
}

medDatabaseExporter::~medDatabaseExporter()
{
    delete d;
}

void medDatabaseExporter::internalRun()
{
    for (int i = 0; i < d->data.length(); i++)
    {
        dtkSmartPointer<dtkAbstractDataWriter> dataWriter = medAbstractDataFactory::instance()->writer(d->writers[i]);
        dataWriter->setData(d->data[i]);
        QString path = d->paths[i];

        if (!dataWriter->canWrite(path) || !dataWriter->write(path))
        {
            emit showError(QString(tr("Writing to file \"%1\" with exporter \"%2\" failed.")).arg(path).arg(dataWriter->description()), 3000);
            emit failure(this);
        }
        else
        {
            emit success(this);
        }
    }


}
