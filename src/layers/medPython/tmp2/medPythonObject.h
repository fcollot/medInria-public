#pragma once

#include "medPythonMappingAccessor.h"
#include "medPythonObjectBase.h"
#include "medPythonSequenceAccessor.h"

namespace med
{

class PythonObject : public PythonObjectBase
{
public:
    static PythonObject take(PyObject* nativeObject);
    static PythonObject borrow(PyObject* nativeObject);

    PythonObject(PyObject* nativeObject = nullptr);
    PythonObject(const PythonObjectBase& other);
    PythonObject& operator=(const PythonObjectBase& other);

    PythonObject(long value);
    PythonObject(double value);
    PythonObject(QString value);

    PythonMappingAccessor operator[](const PythonObject& key);
    PythonSequenceAccessor operator[](size_t i);

    template <class... ARGS>
    PythonObject operator()(ARGS... args);

    template <class... ARGS>
    PythonObject callMethod(QString name, ARGS... args);
};

template<class... ARGS>
PythonObject PythonObject::operator()(ARGS... args)
{

}

template<class... ARGS>
PythonObject PythonObject::callMethod(QString name, ARGS... args)
{
    getAttribute(name).operator()(args...);
}

}
