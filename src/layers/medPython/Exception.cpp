#include "Exception.h"

#include "Convert.h"
#include "PythonObject.h"
#include "PythonModule.h"

namespace medPython
{

bool Error::errorOccurred()
{
    return PyErr_Occurred();
}

void Error::clearError()
{
    PyErr_Clear();
}

QString Error::getErrorMessage()
{
    QString errorMessage;
    PyObject* errorType;
    PyObject* errorValue;
    PyObject* errorTraceback;

    PyErr_Fetch(&errorType, &errorValue, &errorTraceback);

    if (errorValue == nullptr)
    {
        errorMessage = "(undefined Python error)";
    }
    else
    {
        PythonModule tracebackModule = getOrImportTracebackModule();
        errorMessage = formatError(tracebackModule.data(), errorType, errorValue, errorTraceback);
    }

    Py_XDECREF(errorType);
    Py_XDECREF(errorValue);
    Py_XDECREF(errorTraceback);

    return errorMessage;
}

QString Error::formatExceptionMessage(PyObject* exception, QString file, int line, QString function)
{

    message.prepend(file + ", line " + QString::number(line) + " ("+ function + ")" + ": ");
}

void Error::convertAndThrow(PyObject* exception, QString message)
{
    throwIfMatches<TypeError>(exception, message);
    throwIfMatches<SystemError>(exception, message);
    throwIfMatches<StopIteration>(exception, message);
    throwIfMatches<StopAsyncIteration>(exception, message);
    throwIfMatches<ReferenceError>(exception, message);
    throwIfMatches<MemoryError>(exception, message);
    throwIfMatches<EOFError>(exception, message);
    throwIfMatches<BufferError>(exception, message);
    throwIfMatches<AttributeError>(exception, message);
    throwIfMatches<AssertionError>(exception, message);

    throwIfMatches<UnicodeTranslateError>(exception, message);
    throwIfMatches<UnicodeEncodeError>(exception, message);
    throwIfMatches<UnicodeDecodeError>(exception, message);
    throwIfMatches<UnicodeError>(exception, message);
    throwIfMatches<ValueError>(exception, message);

    throwIfMatches<TabError>(exception, message);
    throwIfMatches<IndentationError>(exception, message);
    throwIfMatches<SyntaxError>(exception, message);

    throwIfMatches<RecursionError>(exception, message);
    throwIfMatches<NotImplementedError>(exception, message);
    throwIfMatches<RuntimeError>(exception, message);

    throwIfMatches<TimeoutError>(exception, message);
    throwIfMatches<ProcessLookupError>(exception, message);
    throwIfMatches<PermissionError>(exception, message);
    throwIfMatches<NotADirectoryError>(exception, message);
    throwIfMatches<IsADirectoryError>(exception, message);
    throwIfMatches<InterruptedError>(exception, message);
    throwIfMatches<FileNotFoundError>(exception, message);
    throwIfMatches<FileExistsError>(exception, message);
    throwIfMatches<ConnectionResetError>(exception, message);
    throwIfMatches<ConnectionRefusedError>(exception, message);
    throwIfMatches<ConnectionAbortedError>(exception, message);
    throwIfMatches<BrokenPipeError>(exception, message);
    throwIfMatches<ConnectionError>(exception, message);
    throwIfMatches<ChildProcessError>(exception, message);
    throwIfMatches<BlockingIOError>(exception, message);
    throwIfMatches<OSError>(exception, message);

    throwIfMatches<UnboundLocalError>(exception, message);
    throwIfMatches<NameError>(exception, message);

    throwIfMatches<KeyError>(exception, message);
    throwIfMatches<IndexError>(exception, message);
    throwIfMatches<LookupError>(exception, message);

    throwIfMatches<ModuleNotFoundError>(exception, message);
    throwIfMatches<ImportError>(exception, message);

    throwIfMatches<ZeroDivisionError>(exception, message);
    throwIfMatches<OverflowError>(exception, message);
    throwIfMatches<FloatingPointError>(exception, message);
    throwIfMatches<ArithmeticError>(exception, message);

    throwIfMatches<SystemExit>(exception, message);
    throwIfMatches<KeyboardInterrupt>(exception, message);
    throwIfMatches<GeneratorExit>(exception, message);
    throwIfMatches<PythonException>(exception, message);
    throwIfMatches<PythonBaseException>(exception, message);
}

template <class CPP_EXCEPTION>
void Error::throwIfMatches(PyObject* exception, QString message)
{
    if (PyErr_GivenExceptionMatches(exception, const_cast<PyObject*>(CPP_EXCEPTION::pythonType)))
    {
        CPP_EXCEPTION cppException;
        cppException.setPythonException(exception);
        cppException.setMessage(message);
        throw cppException;
    }
}

QString Error::formatError(PythonModule tracebackModule, PyObject* errorType, PyObject* errorValue, PyObject* errorTraceback)
{
    PythonObject formatFunction;
    PythonObject errorMessageList;

    PyErr_NormalizeException(&errorType, &errorValue, &errorTraceback);

    if (errorTraceback == nullptr)
    {
        formatFunction = tracebackModule.getAttribute("format_exception_only");
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
        result.append(Convert::fromString(PyList_GET_ITEM(errorMessageList, i)));
    }

    Py_XDECREF(formatFunction);
    Py_XDECREF(errorMessageList);

    return result;
}

void Error::pythonToCppConversionError(PythonObject input, QString outputType)
{
    QString message = "Could not convert python object of type \"";
    message += Py_TYPE(input)->tp_name;
    message += "\" to ";
    message += outputType;
    message += "\n";
    PyErr_SetString(PyExc_RuntimeError, message.toStdString().c_str());
}

void Error::cppToPythonConversionError(QString inputType, QString outputType)
{
    QString message = "Could not convert cpp object of type \"";
    message += inputType;
    message += "\" to ";
    message += outputType;
    message += "\n";
    PyErr_SetString(PyExc_RuntimeError, message.toStdString().c_str());
}

}
