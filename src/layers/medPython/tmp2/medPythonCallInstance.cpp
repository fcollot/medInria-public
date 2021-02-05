#include "medPythonCallInstance.h"

#include "medPython.h"
#include "medPythonErrorManager.h"

namespace med
{

class PythonCallInstancePrivate
{
public:
    PythonObject callable;
    PythonObject args;
    PythonObject kwargs;
    bool evaluated;
};

PythonCallInstance::PythonCallInstance(PythonObjectBase& callable, PythonObjectBase& args) :
    d(new PythonCallInstancePrivate)
{
    d->callable = callable;
    d->args = args;
    d->kwargs = Python::list();
    d->evaluated = false;
}

PythonCallInstance::~PythonCallInstance()
{
    ensureEvaluated();
}

PythonCallInstance& PythonCallInstance::kwarg(QString name, PythonObjectBase& value)
{
    d->kwargs[name] = value;
    return *this;
}

PyObject* PythonCallInstance::getReference() const
{
    const_cast<PythonCallInstance*>(this)->ensureEvaluated();
    return PythonObjectBase::getReference();
}

void PythonCallInstance::ensureEvaluated()
{
    if (!d->evaluated)
    {
        evaluate();
        d->evaluated = true;
    }
}

void PythonCallInstance::evaluate()
{
    PyObject* result;
    PyObject* nativeCallable = *d->callable;
    PyObject* nativeArgs = *d->args;
    PyObject* nativeKwargs = *d->args;

    if (d->args.length() > 0)
    {
        if (d->kwargs.length() > 0)
        {
            result = PyObject_Call(nativeCallable, nativeArgs, nativeKwargs);
        }
        else
        {
            result = PyObject_CallObject(nativeCallable, nativeArgs);
        }
    }
    else
    {
        if (d->kwargs.length() > 0)
        {
            result = PyObject_Call(nativeCallable, *Python::tuple(), nativeKwargs);
        }
        else
        {
            result = PyObject_CallNoArgs(nativeCallable);
        }
    }

    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    setReference(result);
}

}
