#pragma once

#include "medPythonExceptions.h"
#include "medPythonUtils.h"

namespace med
{
namespace python
{

template <class OBJECT>
class FunctionCall : public OBJECT
{
public:
    FunctionCall(const OBJECT& callable, const OBJECT& args) :
        callable(callable), args(args)
    {
        kwargs = utils::createDict();
        evaluated = false;
    }

    ~FunctionCall()
    {
        if (!evaluated)
        {
            utils::decref(evaluate());
        }
    }

protected:
    PyObject* getReference() override
    {
        if (!evaluated)
        {
            PyObject* result = evaluate();
            evaluated = true;

            if (result)
            {
                OBJECT::setReference(result);
            }
            else
            {
                propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);
            }
        }

        return OBJECT::getReference();
    }

    void setReference(PyObject* reference) override
    {
        OBJECT::setReference(reference); // to avoid memory leaks
        throw SyntaxError(MEDPYTHON_CODE_LOCATION, "Cannot assign to a function call");
    }

private:
    OBJECT callable;
    OBJECT args;
    OBJECT kwargs;
    bool evaluated;

    PyObject* evaluate()
    {
        return utils::call(*callable, *args, *kwargs);
    }
};

}
}
