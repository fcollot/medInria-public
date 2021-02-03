#pragma once

#include <QObject>
#include <QString>

#include "medPythonExport.h"

namespace med
{

class MEDPYTHON_EXPORT PythonOutputStream : public QObject
{
    Q_OBJECT

signals:
    void recieved(QString text);
    void flushed();

public:
    void write(QString text);
    void flush();
};

}
