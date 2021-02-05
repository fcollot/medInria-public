#include "medPythonConditionsBackend.h"

#include "medPythonExceptionManager.h"

namespace med
{

bool PythonConditionsBackend::isTrue(PythonObject& object)
{
    int result = PyObject_IsTrue(*object);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    return result;
}

bool PythonConditionsBackend::isNot(PythonObject& object)
{
    int result = PyObject_Not(*object);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    return result;
}

bool PythonConditionsBackend::equals(PythonObject& object, PythonObject& other)
{
    return internalCompare(object, other, Py_EQ);
}

bool PythonConditionsBackend::notEquals(PythonObject& object, PythonObject& other)
{
    return internalCompare(object, other, Py_NE);
}

bool PythonConditionsBackend::lessThan(PythonObject& object, PythonObject& other)
{
    return internalCompare(object, other, Py_LT);
}

bool PythonConditionsBackend::greaterThan(PythonObject& object, PythonObject& other)
{
    return internalCompare(object, other, Py_GT);
}

bool PythonConditionsBackend::lessThanEquals(PythonObject& object, PythonObject& other)
{
    return internalCompare(object, other, Py_LE);
}

bool PythonConditionsBackend::greaterThanEquals(PythonObject& object, PythonObject& other)
{
    return internalCompare(object, other, Py_GE);
}

bool PythonConditionsBackend::internalCompare(PythonObject& object, PythonObject& other, int operationId)
{
    bool result = PyObject_RichCompareBool(*object, *other, operationId);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    return result;
}

}
