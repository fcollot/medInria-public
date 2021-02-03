#pragma once

#include "medPythonCore.h"
#include "medPythonReference.h"

namespace med
{
namespace python
{
namespace internal
{

class SequenceItemReference : public AbstractReference
{
public:
    SequenceItemReference(const Reference& sequence, size_t index);

    SequenceItemReference& operator=(const AbstractReference& other) override;

    PyObject* get() const override;

private:
    Reference sequence;
    size_t index;
    mutable Reference value;
};

}
}
}
