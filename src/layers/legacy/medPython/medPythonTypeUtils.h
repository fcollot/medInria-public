#pragma once

#include <QStringList>

#include "medPython.h"
#include "medPythonExport.h"

class MEDPYTHON_EXPORT medPythonTypeUtils
{
public:
    static QString fromString(PyObject* pythonString);
    static PyObject* toPython(QString string);

    static PyObject* toPython(QStringList list);

    static QString getStringAttribute(PyObject* object, QString attributeName);

private:
    medPythonTypeUtils() = default;
    ~medPythonTypeUtils() = default;
};
