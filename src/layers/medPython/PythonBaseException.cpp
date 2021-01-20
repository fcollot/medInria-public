#include "PythonBaseException.h"

#include "Convert.h"
#include "PythonTuple.h"

namespace medPython
{



void PythonBaseException::retrieveExceptionData(PythonObject& type, PythonObject& value, PythonObject& traceback)
{
    PyObject* typeObject;
    PyObject* valueObject;
    PyObject* tracebackObject;

    PyErr_Fetch(&typeObject, &valueObject, &tracebackObject);
    PyErr_NormalizeException(&typeObject, &valueObject, &tracebackObject);

    if (tracebackObject)
    {
        PyException_SetTraceback(exceptionValue, exceptionTraceback);
    }

    type = typeObject;
    value = valueObject;
    traceback = tracebackObject;
}

QString PythonBaseException::formatExceptionMessage(PythonObject type, PythonObject value, PythonObject traceback)
{
    QString result;
    PythonModule tracebackModule = getTracebackModule();

    if (!tracebackModule)
    {
        result = type.getAttribute("__name__") + ": " + value.toString();
    }
    else
    {
        PythonObject formatFunction;
        PythonList errorMessageList;

        if (!traceback)
        {
            formatFunction = tracebackModule.getAttribute("format_exception_only");
            errorMessageList = formatFunction.call(PythonTuple({type, value}));
        }
        else
        {
            formatFunction = tracebackModule.getAttribute("format_exception");
            errorMessageList = formatFunction.call(PythonTuple({type, value, traceback}));
        }

        int numErrorMessages = errorMessageList.getSize();

        for (int i = 0; i < numErrorMessages; i++)
        {
            result.append(Convert::fromString(errorMessageList.getItem(i)));
        }
    }

    return result;
}

PythonModule PythonBaseException::getTracebackModule()
{
    if (!d->tracebackModule && !d->attemptedTracebackImport)
    {
        try
        {
            d->tracebackModule = PythonModule::import("traceback", {"*"});
        }
        catch (PythonException&)
        {
        }

        d->attemptedTracebackImport = true;
    }

    return d->tracebackModule;
}

}
