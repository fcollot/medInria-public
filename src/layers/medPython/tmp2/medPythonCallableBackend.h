#pragma once

#include "medPython.h"
#include "medPythonCallInstance.h"

namespace med
{

class PythonCallableBackend
{
public:
    static PythonCallInstance call(const PythonObject& callable, const PythonObject& arg1, const PythonObject& arg2,
                                   const PythonObject& arg3, const PythonObject& arg4, const PythonObject& arg5,
                                   const PythonObject& arg6, const PythonObject& arg7, const PythonObject& arg8);

    static PythonCallInstance callMethod(const PythonObject& object, QString name, const PythonObject& arg1,
                                         const PythonObject& arg2, const PythonObject& arg3, const PythonObject& arg4,
                                         const PythonObject& arg5, const PythonObject& arg6, const PythonObject& arg7,
                                         const PythonObject& arg8);
};

}
