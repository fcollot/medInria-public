#include "ExceptionTypes.h"

#include "PythonModule.h"
#include "PythonTuple.h"

namespace medPython
{

class PythonBaseExceptionPrivate
{
public:
    static PythonModule tracebackModule;
    static bool alreadyAttemptedTracebackImport;

    QString message;
    PythonObject pythonInstance;
};

bool PythonBaseExceptionPrivate::alreadyAttemptedTracebackImport = false;

const PyObject* PythonBaseException::pythonType = PyExc_BaseException;

PythonBaseException::PythonBaseException()
{
    d->pythonInstance = const_cast<PyObject*>(pythonType);
}

PythonBaseException::~PythonBaseException()
{
    delete d;
}

const char* PythonBaseException::what() const throw()
{
    return TO_CSTRING(message);
}

void PythonBaseException::setMessage(QString message)
{
    d->message = message;
}

void PythonBaseException::setPythonInstance(PythonObject pythonInstance)
{
    d->pythonInstance = pythonInstance;
}

}
