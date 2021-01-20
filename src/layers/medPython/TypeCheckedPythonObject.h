#pragma once

#include "PythonObject.h"

namespace medPython
{

typedef int (*TypeCheckFunction)(PyObject* object);

template <TypeCheckFunction TYPE_CHECK>
class MEDPYTHON_EXPORT TypeCheckedPythonObject : public PythonObject
{
public:
    static TypeCheckedPythonObject<TYPE_CHECK> borrowed(PyObject* object);

    TypeCheckedPythonObject(PyObject* object = nullptr);
    TypeCheckedPythonObject(const PythonObject& other);
    TypeCheckedPythonObject& operator=(const PythonObject& other);

    void borrow(PyObject* object) override;

private:
    QString typeErrorMessage(PyObject* object);
};


}
