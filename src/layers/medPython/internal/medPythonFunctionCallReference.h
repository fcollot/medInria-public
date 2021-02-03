#pragma once

#include "medPythonCore.h"
#include "medPythonReference.h"

namespace med
{
namespace python
{
namespace internal
{

class FunctionCallReference : public AbstractReference
{
public:
    FunctionCallReference(const AbstractReference& callable, const AbstractReference& args);
    virtual ~FunctionCallReference();

    FunctionCallReference& operator=(const AbstractReference& other) override;

    PyObject* get() const override;

    void ensureEvaluated() const;

private:
    Reference callable;
    Reference args;
    Reference kwargs;
    mutable Reference result;

    PyObject* evaluate() const;
};

}
}
}
