#pragma once

#include "medPython.h"

namespace med
{

class MEDPYTHON_EXPORT PythonGenericArithmeticBackend
{
public:
    template <auto FUNCTION>
    static PythonObject wrappedBinaryOperation(PythonObject& object1, PythonObject& object2)
    {
        PyObject* result = FUNCTION(*object1, *object2);
        PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
        return result;
    }


    PythonObject add(PythonObject& object1, PythonObject& object2);
    PythonObject subtract(PythonObject& object1, PythonObject& object2);
    PythonObject multiply(PythonObject& object1, PythonObject& object2);
    PythonObject divide(PythonObject& object1, PythonObject& object2);

    void addInPlace(PythonObject& object, PythonObject& other);
    void subtractInPlace(PythonObject& object, PythonObject& other);
    void multiplyInPlace(PythonObject& object, PythonObject& other);
    void divideInPlace(PythonObject& object, PythonObject& other);
};

}
