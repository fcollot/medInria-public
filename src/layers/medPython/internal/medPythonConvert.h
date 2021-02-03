#pragma once

#include "medPythonCore.h"
#include "medPythonReference.h"

namespace med
{
namespace python
{
namespace internal
{

template <class CTYPE>
class Convert
{
public:
    static Reference toPython(CTYPE object);

    static long fromPython(const AbstractReference& object);
};

}
}
}
