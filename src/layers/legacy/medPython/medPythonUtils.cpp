#include "medPythonUtils.h"

void medPythonUtils::runPythonString(QString pythonString)
{
    if (PyRun_SimpleString(pythonString.toStdString().c_str()) != 0)
    {
        throwException("Error running python string: " + pythonString);
    }
}

QString medPythonUtils::getPythonErrorMessage()
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

void medPythonUtils::addSysPath(QString path)
{
    runPythonString(QString("import sys\nsys.path.insert(0, \"") + path + "\")");
}

void medPythonUtils::removeSysPath(QString path)
{
    runPythonString(QString("import sys\nsys.path.remove(\"") + path + "\")");
}

QString medPythonUtils::convertPythonStringToQString(PyObject* pythonString)
{
    PyObject* stringAsPyBytes = nullptr;

    if (PyBytes_Check(pythonString))
    {
        stringAsPyBytes = pythonString;
    }
    else if (PyUnicode_Check(pythonString))
    {
        stringAsPyBytes = PyUnicode_AsEncodedString(pythonString, "UTF-8", "strict");

        if (stringAsPyBytes == nullptr)
        {
            throwException("Unhandled encoding");
        }
    }

    QString result = strdup(PyBytes_AS_STRING(stringAsPyBytes));
    Py_DECREF(stringAsPyBytes);
    return result;
}

void medPythonUtils::pythonToCppConversionError(PyObject* input, QString outputType)
{
    QString message = "Could not convert python object of type \"";
    message += Py_TYPE(input)->tp_name;
    message += "\" to ";
    message += outputType;
    message += "\n";
    PyErr_SetString(PyExc_RuntimeError, message.toStdString().c_str());
}

void medPythonUtils::cppToPythonConversionError(QString inputType, QString outputType)
{
    QString message = "Could not convert cpp object of type \"";
    message += inputType;
    message += "\" to ";
    message += outputType;
    message += "\n";
    PyErr_SetString(PyExc_RuntimeError, message.toStdString().c_str());
}

QString medPythonUtils::formatError(PyObject* errorType, PyObject* errorValue, PyObject* errorTraceback)
{
    PyObject* tracebackModule = PyImport_AddModule("traceback");
    PyObject* formatFunction;
    PyObject* errorMessageList;

    if (errorTraceback == nullptr)
    {
        formatFunction = PyObject_GetAttrString(tracebackModule, "format_exception_only");
        errorMessageList = PyObject_CallFunctionObjArgs(formatFunction, errorType, errorValue, nullptr);
    }
    else
    {
        formatFunction = PyObject_GetAttrString(tracebackModule, "format_exception");
        errorMessageList = PyObject_CallFunctionObjArgs(formatFunction, errorType, errorValue, errorTraceback, nullptr);
    }

    QString result;

    for (int i = 0; i < PyList_Size(errorMessageList); i++)
    {
        result.append(convertPythonStringToQString(PyList_GET_ITEM(errorMessageList, i)));
    }

    Py_XDECREF(formatFunction);
    Py_XDECREF(errorMessageList);

    return result;
}

void medPythonUtils::throwException(QString message)
{
    throw std::runtime_error((QString("medPythonUtils::") + __func__ + ": " + message).toStdString().c_str());
}
