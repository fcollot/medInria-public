#include "medPythonStandardObjects.h"

#include "medPythonCoreWrappers.h"
#include "medPythonPrimitiveConversions.h"

namespace med
{
namespace python
{

Object none()
{
    return Object::borrowed(Py_None);
}

Object integer(long value)
{
    return Object::create<long>(value);
}

Object integer(const Object& object)
{
    return coreFunction<PyObject*, &PyNumber_Long, PyObject*>(*object);
}

Object floating(double value)
{
    return Object::create<double>(value);
}

Object floating(const Object& object)
{
    return coreFunction<PyObject*, &PyNumber_Float, PyObject*>(*object);
}

Object string(QString string)
{
    return Object::create<QString>(string);
}

Object string(const Object& object)
{
    return coreFunction<PyObject*, &PyObject_Str, PyObject*>(*object);
}

Object list()
{
    return coreFunction<PyObject*, &PyList_New, ssize_t>(0);
}

Object list(const Object& object)
{
    return coreFunction<PyObject*, &PySequence_List, PyObject*>(*object);
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
