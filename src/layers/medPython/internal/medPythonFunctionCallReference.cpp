#include "medPythonFunctionCallReference.h"

#include "medPythonContainerFunctions.h"
#include "medPythonCoreWrappers.h"
#include "medPythonExceptions.h"

namespace med
{
namespace python
{
namespace internal
{

FunctionCallReference::FunctionCallReference(const AbstractReference& callable, const AbstractReference& args) :
    callable(callable), args(args)
{
    kwargs = createDict();
}

FunctionCallReference& FunctionCallReference::operator=(const AbstractReference& other)
{
    throw SyntaxError(MEDPYTHON_CODE_LOCATION, "Cannot assign to a function call");
}

PyObject* FunctionCallReference::get() const
{
    ensureEvaluated();
    return result.get();
}

void FunctionCallReference::ensureEvaluated() const
{
    if (!result)
    {
        result = evaluate();
    }
}

PyObject* FunctionCallReference::evaluate() const
{
    size_t numArgs = getSize(args);
    size_t numKwargs = getSize(kwargs);

    if (numKwargs > 0)
    {
        return coreFunction<PyObject*, &PyObject_Call, PyObject*, PyObject*, PyObject*>(callable.get(), args.get(), kwargs.get());
    }
    else
    {
        if (numArgs > 0)
        {
            return coreFunction<PyObject*, &PyObject_CallObject, PyObject*, PyObject*>(callable.get(), args.get());
        }
        else
        {
            return coreFunction<PyObject*, &PyObject_CallNoArgs, PyObject*>(callable.get());
        }
    }
}

}
}
}
