#include "medPythonExceptionBase.h"

namespace med
{
namespace python
{

class ExceptionBasePrivate
{
public:
    SourceCodeLocation throwSite;
};

Reference ExceptionBase::nativeType()
{
    return PyExc_BaseException;
}

ExceptionBase::ExceptionBase(const SourceCodeLocation& throwSite, QString message) :
    Object(nativeType()(message)), d(new ExceptionBasePrivate)
{
    d->throwSite = throwSite;
}

ExceptionBase::ExceptionBase(const SourceCodeLocation& throwSite, const Reference& nativeException) :
    Object(nativeException), d(new ExceptionBasePrivate)
{
    d->throwSite = throwSite;
}

ExceptionBase::ExceptionBase(const ExceptionBase& other) :
    Object(other), d(new ExceptionBasePrivate)
{
    *d = *other.d;
}

ExceptionBase& ExceptionBase::operator=(const ExceptionBase& other)
{
    Object::operator=(other);
    *d = *other.d;
    return *this;
}

ExceptionBase::~ExceptionBase()
{
    delete d;
}

const char* ExceptionBase::what() const throw()
{
    return qUtf8Printable(const_cast<ExceptionBase*>(this)->formatMessage());
}

QString ExceptionBase::formatMessage()
{
    QString message;

    try
    {
        Object tracebackModule = import("traceback");

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

QString ExceptionBase::formatMessageWithTracebackModule(Object& tracebackModule)
{
    Object errorMessageList;

    if (hasAttribute("__traceback__"))
    {
        Object traceback = getAttribute("__traceback__");
        errorMessageList = tracebackModule.callMethod("format_exception", type(), *this, traceback);
    }
    else
    {
        errorMessageList = tracebackModule.callMethod("format_exception_only", type(), *this);
    }

    int numErrorMessages = errorMessageList.length();
    QString result;

    for (size_t i = 0; i < numErrorMessages; i++)
    {
        result.append(errorMessageList[i].toString());
    }

    return result;
}

}
}
