#pragma once

#include "medPythonCoreForward.h"

namespace med
{
namespace python
{

struct ReferencePrivate;

class Reference
{
public:
    Reference(PyObject* nativeReference);
    virtual ~Reference();

    PyObject* get() const;
    void set(PyObject* nativeReference);

private:
    ReferencePrivate* const d;
};

} // namespace python
} // namespace med
