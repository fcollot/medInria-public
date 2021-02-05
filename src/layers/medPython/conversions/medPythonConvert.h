#pragma once

#include "medPythonExport.h"
#include "medPythonUtils.h"

namespace med
{
namespace python
{

template <class CTYPE>
class MEDPYTHON_EXPORT Convert
{
public:
    static PyObject* toPython(CTYPE object)
    {
        static_assert(true, "Foo");
    }

    static CTYPE fromPython(PyObject* object)
    {

    }
};

}
}
