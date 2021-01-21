#pragma once

#include <QString>

#include "PythonExceptionTypes.h"

#define MEDPYTHON_PROPAGATE_EXCEPTION() do{ medPython::ExceptionManager::propagateCurrentException(MEDPYTHON_CODE_LOCATION); }while(0)

#define MEDPYTHON_CHECK_ERROR() do{ if (medPython::ExceptionManager::exceptionOccured()) MEDPYTHON_PROPAGATE_EXCEPTION(); }while(0)

namespace medPython
{

class ExceptionManager
{
public:
    static void initialize();

    static bool exceptionOccured();

    static void clear();

    static void propagateCurrentException(SourceCodeLocation codeLocation);

private:
    static PyObject* retrieveCurrentException();

    static void castToProperExceptionAndThrow(PythonExceptionBase& exception);

    static void initializeExceptionPropagation();

    template <class EXCEPTION_TYPE>
    static void initializeExceptionPropagation();
};

}
