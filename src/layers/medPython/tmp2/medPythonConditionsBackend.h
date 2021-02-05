#pragma once

#include "medPython.h"

namespace med
{

class PythonConditionsBackend
{
public:
    bool isTrue(PythonObject& object);

    bool isNot(PythonObject& object);

    bool equals(PythonObject& object, PythonObject& other);

    bool notEquals(PythonObject& object, PythonObject& other);

    bool lessThan(PythonObject& object, PythonObject& other);

    bool greaterThan(PythonObject& object, PythonObject& other);

    bool lessThanEquals(PythonObject& object, PythonObject& other);

    bool greaterThanEquals(PythonObject& object, PythonObject& other);

private:
    bool internalCompare(PythonObject& object, PythonObject& other, int operationId);
};

}
