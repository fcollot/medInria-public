#pragma once

#include <QString>

#include "medPythonCore.h"
#include "medPythonExport.h"

namespace med
{

class PythonObjectBasePrivate;

class MEDPYTHON_EXPORT PythonObjectBase
{
public:
    PythonObjectBase(const PythonObjectBase& other);
    virtual PythonObjectBase& operator=(const PythonObjectBase& other);

    PyObject* getReference();
    PyObject* takeReference();
    PyObject* newReference();

protected:
    PythonObjectBase(PyObject* object);
    virtual ~PythonObjectBase();

    void setReference(PyObject* object);
    void replaceReference(PyObject* object);

private:
    PythonObjectBasePrivate* const d;

    void internalBorrow(PyObject* object);
};

}
