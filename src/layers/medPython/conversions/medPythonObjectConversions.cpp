#include "medPythonObjectConversions.h"

#include "medPythonObject.h"

namespace med
{
namespace python
{

bool convertToPython(const AbstractObject& object, PyObject** output)
{
    *output = object.newReference();
    return true;
}

bool convertFromPython(const PyObject* nativeObject, Object* output)
{
    (*output).borrow(nativeObject);
    return true;
}

} // namespace python
} // namespace med
