#pragma once

#define THROW_PYTHON_RELATED_EXCEPTION(message) medPythonError::throwException(__FILE__, __LINE__, __func__, QString() + message)

#define PROPAGATE_PYTHON_EXCEPTION() THROW_PYTHON_RELATED_EXCEPTION(medPythonError::getErrorMessage())

#define CHECK_PYTHON_ERROR() if (medPythonError::checkError()) PROPAGATE_PYTHON_EXCEPTION()

#define CHECK_PYOBJECT_IS_NOT_NULL(variable) if ((variable) == nullptr)\
    THROW_PYTHON_RELATED_EXCEPTION(QString("Argument ") + #variable + " cannot be null")

#include "medPythonExport.h"
#include "medPythonModuleHandle.h"

class MEDPYTHON_EXPORT medPythonError
{
public:
    static bool checkError();

    static void clearError();

    static QString getErrorMessage();

    static void throwException(QString file, int line, QString function, QString message);

private:
    static medPythonModuleHandle getOrImportTracebackModule();

    static PyObject* getTracebackModule();

    static PyObject* importTracebackModule();

    static QString formatError(PyObject* tracebackModule, PyObject* errorType, PyObject* errorValue, PyObject* errorTraceback);

    static void pythonToCppConversionError(PyObject* input, QString outputType = "C++");
    static void cppToPythonConversionError(QString inputType, QString outputType = "Python");

    medPythonError() = default;
    ~medPythonError() = default;
};
