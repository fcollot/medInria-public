#include "medPythonErrorChecker.h"

#include <stdexcept>

#include "medPythonTypeUtils.h"

class medPythonErrorCheckerPrivate
{
public:
    PyObject* tracebackModule;
};

medPythonErrorChecker::medPythonErrorChecker() :
    d(new medPythonErrorCheckerPrivate)
{
    d->tracebackModule = medPythonError::getTracebackModule();
}

void medPythonErrorChecker::clear()
{
    PyErr_Clear();
}

void medPythonErrorChecker::check(QString file, int line, QString function)
{
    if (PyErr_Occurred())
    {
        QString message = file + ", line " + QString::number(line) + " ("+ function + ")" + ": "
                          + getPythonErrorMessage();
        throw std::runtime_error(qUtf8Printable(message));
    }
}

QString medPythonErrorChecker::getPythonErrorMessage()
{
    QString errorMessage;
    PyObject *errorType, *errorValue, *errorTraceback;

    PyErr_Fetch(&errorType, &errorValue, &errorTraceback);

    if (errorValue == nullptr)
    {
        errorMessage = "(undefined error)";
    }
    else
    {
        errorMessage = formatError(errorType, errorValue, errorTraceback);
    }

    Py_XDECREF(errorType);
    Py_XDECREF(errorValue);
    Py_XDECREF(errorTraceback);

    PyErr_Clear();

    return errorMessage;
}

QString medPythonErrorChecker::formatError(PyObject* errorType, PyObject* errorValue, PyObject* errorTraceback)
{
    PyObject* formatFunction;
    PyObject* errorMessageList;

    PyErr_NormalizeException(&errorType, &errorValue, &errorTraceback);

    if (errorTraceback == nullptr)
    {
        formatFunction = PyObject_GetAttrString(d->tracebackModule, "format_exception_only");
        errorMessageList = PyObject_CallFunctionObjArgs(formatFunction, errorType, errorValue, nullptr);
    }
    else
    {
        PyException_SetTraceback(errorValue, errorTraceback);
        formatFunction = PyObject_GetAttrString(d->tracebackModule, "format_exception");
        errorMessageList = PyObject_CallFunctionObjArgs(formatFunction, errorType, errorValue, errorTraceback, nullptr);
    }

    QString result;
    int numErrorMessages = PyList_Size(errorMessageList);

    for (int i = 0; i < numErrorMessages; i++)
    {
        result.append(medPythonTypeUtils::fromString(PyList_GET_ITEM(errorMessageList, i)));
    }

    Py_XDECREF(formatFunction);
    Py_XDECREF(errorMessageList);

    return result;
}
