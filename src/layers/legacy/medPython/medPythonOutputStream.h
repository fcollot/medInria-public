#pragma once

#include <QObject>
#include <QString>

#include "medPythonExport.h"

class MEDPYTHON_EXPORT medPythonOutputStream : public QObject
{
    Q_OBJECT

signals:
    void recieved(QString text);
    void flushed();

public:
    void write(QString text);
    void flush();
};
