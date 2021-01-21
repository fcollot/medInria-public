#include "PythonExceptionBase.h"

namespace medPython
{

class PythonExceptionBasePrivate
{
public:
    SourceCodeLocation throwSite;
};

PythonExceptionBase PythonExceptionBase::create(SourceCodeLocation throwSite, PythonTuple args, PythonDict kwargs)
{
    PythonObject nativeException = PythonType(nativeType).createInstance(args, kwargs);
    return PythonExceptionBase(throwSite, nativeException.takeReference());
}

PythonExceptionBase::PythonExceptionBase(SourceCodeLocation throwSite, PyObject* nativeException) :
    PythonObject(nativeException), d(new PythonExceptionBasePrivate)
{
    d->throwSite = throwSite;

    if (!nativeException)
    {
        PythonType exceptionType = PythonType::borrowed(nativeType);
        setObject(exceptionType.createInstance().takeReference());
    }
}

PythonExceptionBase::PythonExceptionBase(const PythonExceptionBase& other) :
    PythonObject(other), d(new PythonExceptionBasePrivate)
{
    *d = *other.d;
}

PythonExceptionBase& PythonExceptionBase::operator=(const PythonExceptionBase& other)
{
    *d = *other.d;
    return *this;
}

PythonExceptionBase::~PythonExceptionBase()
{
    delete d;
}

const char* PythonExceptionBase::what() const throw()
{
    return qUtf8Printable(const_cast<PythonExceptionBase*>(this)->formatMessage());
}

QString PythonExceptionBase::formatMessage()
{
    QString message;

    try
    {
        PythonModule tracebackModule = PythonModule::import("traceback");

        if (tracebackModule)
        {
            message = formatMessageWithTracebackModule(tracebackModule);
        }
    }
    catch (...)
    {
    }

    return message.isEmpty() ? toString() : message;
}

QString PythonExceptionBase::formatMessageWithTracebackModule(PythonModule& tracebackModule)
{
    PythonObject formatFunction;
    PythonTuple formatArguments;

    if (hasAttribute("__traceback__"))
    {
        PythonObject traceback = getAttribute("__traceback__");
        formatFunction = tracebackModule.getAttribute("format_exception");
        formatArguments = PythonTuple({getType(), *this, traceback});
    }
    else
    {
        formatFunction = tracebackModule.getAttribute("format_exception_only");
        formatArguments = PythonTuple({getType(), *this});
    }

    PythonList errorMessageList = formatFunction.call(formatArguments);
    int numErrorMessages = errorMessageList.getSize();
    QString result;

    for (int i = 0; i < numErrorMessages; i++)
    {
        result.append(errorMessageList.getItem(i).toString());
    }

    return result;
}

}
