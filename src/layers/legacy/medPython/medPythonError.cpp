#include "medPythonError.h"

#include <stdexcept>

#include "medPythonTypeUtils.h"

bool medPythonError::checkError()
{
    return PyErr_Occurred();
}

void medPythonError::clearError()
{
    PyErr_Clear();
}

QString medPythonError::getErrorMessage()
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
        medPythonModuleHandle tracebackModule = getOrImportTracebackModule();
        errorMessage = formatError(tracebackModule.getModuleObject(), errorType, errorValue, errorTraceback);
    }

    Py_XDECREF(errorType);
    Py_XDECREF(errorValue);
    Py_XDECREF(errorTraceback);

    return errorMessage;
}

void medPythonError::throwException(QString file, int line, QString function, QString message)
{
    clearError();
    message.prepend(file + ", line " + QString::number(line) + " ("+ function + ")" + ": ");
    throw std::runtime_error(qUtf8Printable(message));
}

medPythonModuleHandle medPythonError::getOrImportTracebackModule()
{
   PyObject* module = getTracebackModule();

   if (!module)
   {
       module = importTracebackModule();
   }

   return module;
}

PyObject* medPythonError::getTracebackModule()
{
    PyObject* pythonName = medPythonTypeUtils::toPython("traceback");
    PyObject* module = PyImport_GetModule(pythonName);
    Py_DECREF(pythonName);
    return module;
}

PyObject* medPythonError::importTracebackModule()
{
    PyObject* tracebackModule = PyImport_ImportModule("traceback");

    if (!tracebackModule)
    {
        THROW_PYTHON_RELATED_EXCEPTION("Cannot import traceback module");
    }

    PyObject* mainModule = PyImport_AddModule("__main__");

    if (PyModule_AddObject(mainModule, "traceback", tracebackModule) != 0)
    {
        Py_DECREF(tracebackModule);
        THROW_PYTHON_RELATED_EXCEPTION("Cannot add traceback module to __main__");
    }

    Py_INCREF(tracebackModule);
    return tracebackModule;
}

QString medPythonError::formatError(PyObject* tracebackModule, PyObject* errorType, PyObject* errorValue, PyObject* errorTraceback)
{
    PyObject* formatFunction;
    PyObject* errorMessageList;

    PyErr_NormalizeException(&errorType, &errorValue, &errorTraceback);

    if (errorTraceback == nullptr)
    {
        formatFunction = PyObject_GetAttrString(tracebackModule, "format_exception_only");
        errorMessageList = PyObject_CallFunctionObjArgs(formatFunction, errorType, errorValue, nullptr);
    }
    else
    {
        PyException_SetTraceback(errorValue, errorTraceback);
        formatFunction = PyObject_GetAttrString(tracebackModule, "format_exception");
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

void medPythonError::pythonToCppConversionError(PyObject* input, QString outputType)
{
    QString message = "Could not convert python object of type \"";
    message += Py_TYPE(input)->tp_name;
    message += "\" to ";
    message += outputType;
    message += "\n";
    PyErr_SetString(PyExc_RuntimeError, message.toStdString().c_str());
}

void medPythonError::cppToPythonConversionError(QString inputType, QString outputType)
{
    QString message = "Could not convert cpp object of type \"";
    message += inputType;
    message += "\" to ";
    message += outputType;
    message += "\n";
    PyErr_SetString(PyExc_RuntimeError, message.toStdString().c_str());
}
