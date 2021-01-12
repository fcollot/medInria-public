#pragma once

#include <QStringList>

#include "medPythonExport.h"
#include "medPythonUtils.h"

class MEDPYTHON_EXPORT medPythonTypeUtils
{
public:
    static QString fromString(PyObject* pythonString);
    static PyObject* toPython(QString string);

    static PyObject* toPython(QStringList list);

    static void pythonToCppConversionError(PyObject* input, QString outputType = "C++");
    static void cppToPythonConversionError(QString inputType, QString outputType = "Python");

    static QString getStringAttribute(PyObject* object, QString attributeName);

private:
    medPythonTypeUtils() = default;
    ~medPythonTypeUtils() = default;
};
