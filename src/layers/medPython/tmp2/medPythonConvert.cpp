#include "medPythonConvert.h"

#include "medPythonExceptionTypes.h"

namespace med
{
namespace python
{

template<class CTYPE>
PyObject* Convert::toPython(const CTYPE object)
{
    throw TypeError(MEDPYTHON_CODE_LOCATION, "Unsupported conversion");
}

template<class CTYPE>
CTYPE Convert::fromPython(const PyObject* object)
{
    throw TypeError(MEDPYTHON_CODE_LOCATION, "Unsupported conversion");
}

}
}
