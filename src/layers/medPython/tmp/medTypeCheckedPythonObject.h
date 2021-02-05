#pragma once

#include "medPythonExceptionTypes.h"
#include "medPythonObject.h"

namespace med
{

typedef int (*TypeCheckFunction)(PyObject* object);

template <TypeCheckFunction TYPE_CHECK>
class MEDPYTHON_EXPORT TypeCheckedPythonObject : public PythonObject
{
public:
    static TypeCheckedPythonObject<TYPE_CHECK> borrowed(PyObject* object)
    {
        TypeCheckedPythonObject<TYPE_CHECK> borrowedObject;
        borrowedObject.borrow(object);
        return borrowedObject;
    }

    TypeCheckedPythonObject(PyObject* object = nullptr) :
        PythonObject(object)
    {
        if ((object != Py_None) && !TYPE_CHECK(object))
        {
            setObject(nullptr);
            PythonObject message = PythonObject::create(typeErrorMessage(self()));
            throw PythonRuntimeError::create(MEDPYTHON_CODE_LOCATION, {message});
        }
    }

    TypeCheckedPythonObject(const PythonObject& other) :
        PythonObject(other)
    {
        if ((self() != Py_None) && !TYPE_CHECK(self()))
        {
            clear();
            PythonObject message = PythonObject::create(typeErrorMessage(self()));
            throw PythonRuntimeError::create(MEDPYTHON_CODE_LOCATION, {message});
        }
    }


    void borrow(PyObject* object) override
    {
        if ((object != Py_None) && !TYPE_CHECK(object))
        {
            PythonObject message = PythonObject::create(typeErrorMessage(self()));
            throw PythonRuntimeError::create(MEDPYTHON_CODE_LOCATION, {message});
        }

        PythonObject::borrow(object);
    }

private:
    QString typeErrorMessage(PyObject* object)
    {
        return QString("Incompatible PyObject of type ") + Py_TYPE(object)->tp_name;
    }
};


}
