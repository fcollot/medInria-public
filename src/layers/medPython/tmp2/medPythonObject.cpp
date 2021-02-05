#include "medPythonObject.h"

#include "medPythonErrorManager.h"

namespace med
{

PythonObject PythonObject::take(PyObject* nativeObject)
{
    return nativeObject;
}

PythonObject PythonObject::borrow(PyObject* nativeObject)
{
    Py_XINCREF(nativeObject);
    return nativeObject;
}

PythonObject::PythonObject(PyObject* nativeObject) :
    PythonObjectBase(nativeObject)
{
}

PythonObject::PythonObject(const PythonObjectBase& other) :
    PythonObjectBase(other)
{
}

PythonObject& PythonObject::operator=(const PythonObjectBase& other)
{
    PythonObjectBase::operator=(other);
    return *this;
}



PythonMappingAccessor PythonObject::operator[](const PythonObject& key)
{
    return PythonMappingAccessor(*this, key);
}

PythonSequenceAccessor PythonObject::operator[](size_t i)
{
    return PythonSequenceAccessor(*this, i);
}

}
