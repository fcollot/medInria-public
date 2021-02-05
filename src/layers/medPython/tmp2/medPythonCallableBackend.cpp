#include "medPythonCallableBackend.h"

#include "medPythonExceptionManager.h"

namespace med
{

PythonCallInstance PythonCallableBackend::call(const PythonObject& callable, const PythonObject& arg1, const PythonObject& arg2,
                                               const PythonObject& arg3, const PythonObject& arg4, const PythonObject& arg5,
                                               const PythonObject& arg6, const PythonObject& arg7, const PythonObject& arg8)
{
    QList<PythonObject> potentialArgs= {arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8};
    QList<PythonObject> args;

    foreach (const PythonObject& arg, potentialArgs)
    {
        if (arg.isValid())
        {
            args.append(arg);
        }
        else
        {
            break;
        }
    }

    return PythonCallInstance(callable, Python::list(args));
}

PythonCallInstance PythonCallableBackend::callMethod(const PythonObject& object, QString name, const PythonObject& arg1,
                                                     const PythonObject& arg2, const PythonObject& arg3, const PythonObject& arg4,
                                                     const PythonObject& arg5, const PythonObject& arg6, const PythonObject& arg7,
                                                     const PythonObject& arg8)
{
    return call(object.getAttribute(name), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

}
