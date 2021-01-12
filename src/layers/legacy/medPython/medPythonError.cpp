#include "medPythonError.h"

#include <stdexcept>

#include "medPythonTypeUtils.h"

medPythonModuleHandle medPythonError::getTracebackModule()
{
   PyObject* module = getLoadedTracebackModule();

   if (!module)
   {
       module = loadTracebackModule();
   }

   return module;
}

PyObject* medPythonError::getLoadedTracebackModule()
{
    PyObject* pythonName = medPythonTypeUtils::toPython("traceback");
    PyObject* module = PyImport_GetModule(pythonName);
    Py_XDECREF(module);
    Py_DECREF(pythonName);
    return (module != nullptr);
}

PyObject* medPythonError::loadTracebackModule()
{
    PyObject* tracebackModule = PyImport_ImportModule("traceback");

    if (tracebackModule)
    {
        PyObject* mainModule = PyImport_AddModule("__main__");

        if (mainModule)
        {
            if (PyModule_AddObject(mainModule, "traceback", tracebackModule) != 0)
            {
                Py_DECREF(tracebackModule);
                THROW_EXCEPTION("Cannot add traceback module");
            }
        }
    }
    else
    {
        THROW_EXCEPTION("Cannot import traceback module");
    }
}
