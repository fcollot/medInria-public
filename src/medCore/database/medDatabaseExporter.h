/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <dtkCore/dtkSmartPointer>
#include <medCoreExport.h>
#include <QtCore>

#include <medJobItem.h>

class medAbstractData;
class medDatabaseExporterPrivate;

class MEDCORE_EXPORT medDatabaseExporter : public medJobItem
{
    Q_OBJECT

public:
     medDatabaseExporter(dtkSmartPointer<medAbstractData> data, QString path, QString writer);
     medDatabaseExporter(QList<dtkSmartPointer<medAbstractData> > data, QList<QString> paths, QList<QString> writers);
    ~medDatabaseExporter();

protected:
    void internalRun();

private:
    medDatabaseExporterPrivate *d;
};


