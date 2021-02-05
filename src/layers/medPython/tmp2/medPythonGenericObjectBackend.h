#pragma once

#include "medPython.h"

namespace med
{

class PythonGenericObjectBackend
{
public:
    PythonObject type(PythonObject& object);
    QString typeName(PythonObject& object);

    bool hasAttribute(PythonObject& object, QString name);
    PythonObject getAttribute(PythonObject& object, QString name);
    void setAttribute(PythonObject& object, QString name, PythonObject value);

    static QList<QString> dir(PythonObject& object);
};

}
