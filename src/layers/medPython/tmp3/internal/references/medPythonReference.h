#pragma once

#include "medPythonAbstractReference.h"
#include "medPythonCore.h"

namespace med
{
namespace python
{
namespace internal
{

class Reference : public AbstractReference
{
public:
    static Reference borrowed(PyObject* object);

    Reference(PyObject* nativeReference = nullptr);
    virtual ~Reference();

    Reference& operator=(const AbstractReference& other) override;
    Reference& operator=(PyObject* object);

    PyObject* get() const override;

    void borrow(PyObject* object);

private:
    PyObject* nativeReference;
};

}
}
}
