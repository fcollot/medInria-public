#pragma once

#include "PythonDict.h"
#include "PythonTuple.h"
#include "TypeCheckedPythonObject.h"

namespace medPython
{

int isTypeObject(PyObject* object)
{
    return PyType_Check(object);
}

class MEDPYTHON_EXPORT PythonType : public TypeCheckedPythonObject<&isTypeObject>
{
public:
    PythonType(PyObject* typeObject = nullptr);
    PythonType(const PythonObject& other);

    QString getName();

    PythonObject createInstance(PythonTuple args = nullptr, PythonDict kwargs = nullptr);

    template<class TYPE>
    TYPE* createCastedInstance(PythonTuple args = nullptr, PythonDict kwargs = nullptr);
};

}
