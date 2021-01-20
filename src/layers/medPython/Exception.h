#pragma once

#include <QString>

#include "ExceptionTypes.h"
#include "Export.h"
#include "Python.h"

#define THROW_PYTHON_EXCEPTION(message) \

Error::throwException(__FILE__, __LINE__, __func__, QString() + message)

#define PROPAGATE_PYTHON_EXCEPTION() THROW_PYTHON_EXCEPTION(Error::getErrorMessage())

#define CHECK_PYTHON_ERROR() if (Error::errorOccurred()) PROPAGATE_PYTHON_EXCEPTION()

#define ENSURE_NON_NULL_ARGUMENT(argument) if (!((argument)))\
    THROW_PYTHON_RELATED_EXCEPTION(QString("Argument ") + #argument + " cannot be null")

namespace medPython
{

class PythonModule;
class PythonObject;

class MEDPYTHON_EXPORT Error
{
public:
    static bool errorOccurred();

    static void clearError();

    static QString getErrorMessage();

    static QString formatExceptionMessage(PyObject* exception, QString file, int line, QString function);

    static void convertAndThrow(PyObject* exception, QString message);

    template <class CPP_EXCEPTION>
    static void throwIfMatches(PyObject* exception, QString message);

private:
    static PythonModule getOrImportTracebackModule();

    static PythonModule getTracebackModule();

    static PythonModule importTracebackModule();

    static QString formatError(PythonModule tracebackModule, PyObject* errorType, PyObject* errorValue, PyObject* errorTraceback);

    static void pythonToCppConversionError(PythonObject input, QString outputType = "C++");
    static void cppToPythonConversionError(QString inputType, QString outputType = "Python");

    Error() = default;
    ~Error() = default;
};

}
