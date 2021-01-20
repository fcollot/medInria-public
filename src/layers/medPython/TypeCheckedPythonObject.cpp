#include "TypeCheckedPythonObject.h"

namespace medPython
{

template <TypeCheckFunction TYPE_CHECK>
TypeCheckedPythonObject<TYPE_CHECK> TypeCheckedPythonObject<TYPE_CHECK>::borrowed(PyObject* object)
{
    TypeCheckedPythonObject<TYPE_CHECK> borrowedObject;
    borrowedObject.borrow(object);
    return borrowedObject;
}

template <TypeCheckFunction TYPE_CHECK>
TypeCheckedPythonObject<TYPE_CHECK>::TypeCheckedPythonObject(PyObject* object) :
    PythonObject(object)
{
    if ((object != Py_None) && !TYPE_CHECK(object))
    {
        setObject(nullptr);
        THROW_PYTHON_RELATED_EXCEPTION(typeErrorMessage(object));
    }
}

template <TypeCheckFunction TYPE_CHECK>
TypeCheckedPythonObject<TYPE_CHECK>::TypeCheckedPythonObject(const PythonObject& other):
    PythonObject(other)
{
    if ((data() != Py_None) && !TYPE_CHECK(data()))
    {
        clear();
        THROW_PYTHON_RELATED_EXCEPTION(typeErrorMessage(other.data()));
    }
}

template <TypeCheckFunction TYPE_CHECK>
void TypeCheckedPythonObject<TYPE_CHECK>::borrow(PyObject* object)
{
    if ((object != Py_None) && !TYPE_CHECK(object))
    {
        THROW_PYTHON_RELATED_EXCEPTION(typeErrorMessage(object));
    }

    PythonObject::borrow(object);
}

template<TypeCheckFunction TYPE_CHECK>
QString TypeCheckedPythonObject<TYPE_CHECK>::typeErrorMessage(PyObject* object)
{
    return QString("Incompatible PyObject of type ") + Py_TYPE(object)->tp_name;
}

}
