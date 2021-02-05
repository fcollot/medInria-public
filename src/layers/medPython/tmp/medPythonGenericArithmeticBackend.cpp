#include "medPythonGenericArithmeticBackend.h"

#include "medPythonExceptionManager.h"

namespace med
{

PythonObject PythonGenericArithmeticBackend::add(PythonObject& object1, PythonObject& object2)
{
    PyObject* result = PyNumber_Add(*object1, *object2);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    return result;
}

PythonObject PythonGenericArithmeticBackend::subtract(PythonObject& object1, PythonObject& object2)
{
    PyObject* result = PyNumber_Subtract(*object1, *object2);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    return result;
}

PythonObject PythonGenericArithmeticBackend::multiply(PythonObject& object1, PythonObject& object2)
{
    PyObject* result = PyNumber_Multiply(*object1, *object2);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    return result;
}

PythonObject PythonGenericArithmeticBackend::divide(PythonObject& object1, PythonObject& object2)
{
    PyObject* result = PyNumber_TrueDivide(*object1, *object2);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    return result;
}

void PythonGenericArithmeticBackend::addInPlace(PythonObject& object, PythonObject& other)
{
    PyObject* result = PyNumber_InPlaceAdd(*object, *other);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    object = result;
}

void PythonGenericArithmeticBackend::subtractInPlace(PythonObject& object, PythonObject& other)
{
    PyObject* result = PyNumber_InPlaceSubtract(*object, *other);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    object = result;
}

void PythonGenericArithmeticBackend::multiplyInPlace(PythonObject& object, PythonObject& other)
{
    PyObject* result = PyNumber_InPlaceMultiply(*object, *other);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    object = result;
}

void PythonGenericArithmeticBackend::divideInPlace(PythonObject& object, PythonObject& other)
{
    PyObject* result = PyNumber_InPlaceTrueDivide(*object, *other);
    PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    object = result;
}

}
