#pragma once

#include "medPythonReference.h"

namespace med
{
namespace python
{

struct SequenceAccessorPrivate;

class SequenceAccessor : public Reference
{
public:
    SequenceAccessor(const Reference& sequence, ssize_t index);

protected:
    PyObject* getReference() override;

    void setReference(PyObject* reference) override;

private:
    SequenceAccessorPrivate* const d;
};

} // namespace python
} // namespace med
