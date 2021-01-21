#pragma once

#include <QObject>
#include <QString>

#include "Export.h"

namespace medPython
{

class MEDPYTHON_EXPORT OutputStream : public QObject
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
