#pragma once

#include "medPythonCore.h"

namespace med
{
namespace python
{
namespace internal
{

class AbstractReference
{
public:
    virtual AbstractReference& operator=(const AbstractReference& other) = 0;

    virtual PyObject* get() const = 0;

    PyObject* newReference() const;

    operator bool();
};

}
}
}
