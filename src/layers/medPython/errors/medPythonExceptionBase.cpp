#include "medPythonExceptionBase.h"

#include "medPythonCore.h"

namespace med
{
namespace python
{

class ExceptionBasePrivate
{
public:
    SourceCodeLocation throwSite;
    QString message;
};

PyObject* ExceptionBase::nativeType = PyExc_BaseException;

ExceptionBase::ExceptionBase(const SourceCodeLocation& throwSite, QString message) :
    d(new ExceptionBasePrivate)
{
    d->throwSite = throwSite;
    PyObject* args = Py_BuildValue("s", qUtf8Printable(message));
    PyObject* nativeException = utils::call(nativeType, args);
    d->message = formatMessage(nativeException);
    utils::decref(args);
    utils::decref(nativeException);
}

ExceptionBase::ExceptionBase(const SourceCodeLocation& throwSite, PyObject* nativeException) :
    d(new ExceptionBasePrivate)
{
    d->throwSite = throwSite;
    d->message = formatMessage(nativeException);
    utils::decref(nativeException);
}

ExceptionBase::~ExceptionBase()
{
    delete d;
}

const char* ExceptionBase::what() const throw()
{
    return qUtf8Printable(d->message);
}

QString ExceptionBase::formatMessage(PyObject* nativeException)
{
    if (getTracebackModule())
    {
        return formatMessageWithTracebackModule(nativeException);
    }
    else
    {
        return "Unkown error";
    }
}

QString ExceptionBase::formatMessageWithTracebackModule(PyObject* nativeException)
{
    PyObject* tracebackModule = getTracebackModule();
    PyObject* traceback = PyObject_GetAttrString(nativeException, "__traceback__");
    QString formatMethod;
    PyObject* formatArgs;

    if (traceback)
    {
        formatMethod = "format_exception";
        formatArgs = Py_BuildValue("NNN", Py_TYPE(nativeException), nativeException, traceback);
    }
    else
    {
        formatMethod = "format_exception_only";
        formatArgs = Py_BuildValue("NN", Py_TYPE(nativeException), nativeException);
    }

    QString result;

    if (formatArgs)
    {
        PyObject* errorMessageList = utils::callMethod(tracebackModule, formatMethod, formatArgs);

        if (errorMessageList)
        {
            ssize_t numErrorMessages = utils::length(errorMessageList);

            if (numErrorMessages >= 0)
            {
                for (ssize_t i = 0; i < numErrorMessages; i++)
                {
                    PyObject* messageItem = utils::getItem(errorMessageList, i);

                    if (messageItem)
                    {
                        QString messageString = utils::toString(messageItem);
                        utils::decref(messageItem);

                        if (!messageString.isEmpty())
                        {
                            result.append(messageString);
                        }
                    }
                }
            }

            utils::decref(errorMessageList);
        }

        utils::decref(formatArgs);
    }

    utils::decref(traceback);

    return result.isEmpty() ? "Unkown error" : result;
}

}
}
