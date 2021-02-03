#pragma once

#include "medPythonCore.h"
#include "medPythonReference.h"

namespace med
{
namespace python
{
namespace internal
{

class MappingItemReference : public AbstractReference
{
public:
    MappingItemReference(const Reference& mapping, const Reference& key);

    MappingItemReference& operator=(const AbstractReference& other) override;

    PyObject* get() const override;

private:
    Reference mapping;
    Reference key;
    mutable Reference value;
};

}
}
}
