#include "medPythonFunctionCall.h"

#include "medPythonContainerAccessor.h"
#include "medPythonCoreWrappers.h"
#include "medPythonError.h"
#include "medPythonExceptions.h"

namespace med
{
namespace python
{

struct FunctionCallPrivate
{
    Object callable;
    Object args;
    Object kwargs;
    Object result;
    bool evaluated;
};

FunctionCall::FunctionCall(const AbstractObject& callable, const AbstractObject& args) :
    d(new FunctionCallPrivate)
{
    d->callable = callable;
    d->args = args;
    d->kwargs = coreFunction<PyObject*, &PyDict_New>();
    d->evaluated = false;
}

FunctionCall::~FunctionCall()
{
    if (!d->evaluated)
    {
        PyObject* result = evaluate();
        Py_CLEAR(result);
    }
}

FunctionCall& FunctionCall::kwarg(QString name, const AbstractObject& value)
{
    d->kwargs[name] = value;
    return *this;
}

PyObject* FunctionCall::getReference() const
{
    if (!d->evaluated)
    {
        PyObject* result = evaluate();
        d->evaluated = true;

        if (result)
        {
            d->result = result;
        }
        else
        {
            propagateCurrentError();
        }
    }

    return *d->result;
}

void FunctionCall::setReference(PyObject* Object)
{
    Py_CLEAR(Object);
    throw SyntaxError("Cannot assign to a function call");
}

PyObject* FunctionCall::evaluate() const
{
    PyObject* result = nullptr;
    ssize_t numArgs = PyObject_Length(*d->args);

    if (!PyErr_Occurred())
    {
        ssize_t numKwargs = PyObject_Length(*d->kwargs);

        if (!PyErr_Occurred())
        {
            if (numKwargs > 0)
            {
                result = PyObject_Call(*d->callable, *d->args, *d->kwargs);
            }
            else
            {
                if (numArgs > 0)
                {
                    result = PyObject_CallObject(*d->callable, *d->args);
                }
                else
                {
                    result = PyObject_CallNoArgs(*d->callable);
                }
            }
        }
    }

    return result;
}

} // namespace python
} // namespace med
