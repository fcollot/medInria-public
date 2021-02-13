#include "medPythonStandardObjects.h"

#include "medPythonCore.h"
#include "medPythonCoreWrappers.h"
#include "medPythonPrimitiveConversions.h"

namespace med
{
namespace python
{

Object none()
{
    return Object();
}

Object list()
{
    return coreFunction<PyObject*, &PyList_New, ssize_t>(0);
}

Object list(const Object& object)
{
    return coreFunction<PyObject*, &PySequence_List, PyObject*>(*object);
}

Object tuple()
{
    return coreFunction<PyObject*, &PyTuple_New, ssize_t>(0);
}

Object tuple(const Object& object)
{
    return coreFunction<PyObject*, &PySequence_Tuple, PyObject*>(*object);
}

Object dict()
{
    return coreFunction<PyObject*, &PyDict_New>();
}

}
}
