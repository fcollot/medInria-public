#pragma once

#include "medPythonObject.h"

namespace med
{
namespace python
{

struct ContainerAccessorPrivate;

class ContainerAccessor : public AbstractObject
{
public:
    ContainerAccessor(const AbstractObject& container, PyObject* key);

    using AbstractObject::operator=;

protected:
    PyObject* getReference() const override;
    void setReference(PyObject* reference) override;

private:
    ContainerAccessorPrivate* const d;
};

template <class ARG>
ContainerAccessor AbstractObject::operator[](const ARG& key)
{
    PyObject* pythonKey;

    if (!convertToPython(key, &pythonKey))
    {
        propagateCurrentError();
    }

    return ContainerAccessor(*this, pythonKey);
}

} // namespace python
} // namespace med
