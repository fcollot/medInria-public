#include "PythonType.h"

namespace medPython
{

PythonType::PythonType(PyObject* typeObject) :
    TypeCheckedPythonObject(typeObject)
{
}

PythonType::PythonType(const PythonObject& other) :
    TypeCheckedPythonObject(other)
{
}

QString PythonType::getName()
{
    return getAttribute("__name__").toString();
}

PythonObject PythonType::createInstance(PythonTuple args, PythonDict kwargs)
{
    return call(args, kwargs);
}

template<class TYPE>
TYPE* PythonType::createCastedInstance(PythonTuple args, PythonDict kwargs)
{
    return createInstance(args, kwargs).internalCast<TYPE>(true);
}

}
