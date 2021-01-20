#include "PythonType.h"

#include "Convert.h"

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
    return Convert::fromString(getAttribute("__name__"));
}

PythonObject PythonType::newInstance(PythonTuple args, PythonDict kwargs)
{
    return call(args, kwargs);
}

template<class TYPE>
TYPE* PythonType::newCastedInstance(PythonTuple args, PythonDict kwargs)
{
    return newInstance(args, kwargs).internalCast<TYPE>(true);
}

}
