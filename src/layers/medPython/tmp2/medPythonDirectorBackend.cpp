#include "medPythonDirectorBackend.h"

#include "swig_runtime.h"

#include "medPythonExceptionManager.h"

void* internalConvert(PyObject* object, QString cppTypeName)
{
    swig_type_info* swigType = SWIG_TypeQuery(qUtf8Printable(cppTypeName));

    if (!swigType)
    {
        throw PythonRuntimeError(MEDPYTHON_CODE_LOCATION, QString("No SWIG wrapping found for ") + cppTypeName);
    }

    void* castedInstance;
    int conversionResult = SWIG_Python_ConvertPtr(object, &castedInstance, swigType, SWIG_POINTER_DISOWN);

    if (SWIG_IsOK(conversionResult))
    {
        return castedInstance;
    }
    else
    {
        QString message = QString("Cannot cast ") + PyObject_Str(object) + " to C++ type " + cppTypeName;
        throw PythonRuntimeError(MEDPYTHON_CODE_LOCATION, message);
    }
}
