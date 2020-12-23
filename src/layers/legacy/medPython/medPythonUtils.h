#pragma once

#include <stdexcept>

#include <QString>

#undef slots
#define slots _slots

#undef _LARGEFILE_SOURCE
#include <Python.h>

#undef slots
#define slots Q_SLOTS

#include "medPythonExport.h"

#define THROW_METHOD_EXCEPTION(message) throw std::runtime_error(QString(QString(typeid(*this).name()) + "::" + __func__ + ": " + message).toStdString().c_str())

class MEDPYTHON_EXPORT medPythonUtils
{
public:
    static void runPythonString(QString pythonString);

    static QString getPythonErrorMessage();

    static void addSysPath(QString path);
    static void removeSysPath(QString path);

    static QString convertPythonStringToQString(PyObject* pythonString);

    static void pythonToCppConversionError(PyObject* input, QString outputType = "C++");
    static void cppToPythonConversionError(QString inputType, QString outputType = "Python");

private:
    static QString formatError(PyObject* errorType, PyObject* errorValue, PyObject* errorTraceback);

    static void throwException(QString message);

    medPythonUtils() = default;
    ~medPythonUtils() = default;
};
