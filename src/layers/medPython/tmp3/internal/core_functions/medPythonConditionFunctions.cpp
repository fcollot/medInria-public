#include "medPythonConditionFunctions.h"

#include "medPythonCoreWrappers.h"

namespace med
{
namespace python
{
namespace internal
{

namespace
{

inline bool internalCompare(const AbstractReference& object, const AbstractReference& other, int operationId)
{
    return coreFunction<bool, &PyObject_RichCompareBool, PyObject*, PyObject*, int&>(object.get(), other.get(), operationId);
}

}

bool isTrue(const AbstractReference& object)
{
    return coreFunction<bool, &PyObject_IsTrue, PyObject*>(object.get());
}

bool isNot(const AbstractReference& object)
{
    return coreFunction<int, &PyObject_Not, PyObject*>(object.get());
}

bool equals(const AbstractReference& object, const AbstractReference& other)
{
    return internalCompare(object, other, Py_EQ);
}

bool notEquals(const AbstractReference& object, const AbstractReference& other)
{
    return internalCompare(object, other, Py_NE);
}

bool lessThan(const AbstractReference& object, const AbstractReference& other)
{
    return internalCompare(object, other, Py_LT);
}

bool greaterThan(const AbstractReference& object, const AbstractReference& other)
{
    return internalCompare(object, other, Py_GT);
}

bool lessThanEquals(const AbstractReference& object, const AbstractReference& other)
{
    return internalCompare(object, other, Py_LE);
}

bool greaterThanEquals(const AbstractReference& object, const AbstractReference& other)
{
    return internalCompare(object, other, Py_GE);
}

}
}
}
