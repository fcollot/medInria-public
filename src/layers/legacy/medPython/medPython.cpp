#include "medPython.h"

#include "medPythonModuleManager.h"
#include "medPythonUtils.h"

namespace
{

std::unique_ptr<medPython> _instance;

}

void medPython::initialize()
{
    _instance.reset(new medPython);
}

medPython::medPython()
{
    Py_Initialize();

    initTracebackModule();

    /* Enabling "dont_write_bytecode" forces modules to be compiled in-memory every time they are loaded.
     * This is needed to prevent .pyc files from being generated each time a temporary module is created.
     */
    medPythonUtils::runPythonString("import sys\nsys.dont_write_bytecode = True\n");

    medPythonModuleManager::initialize();
}

medPython::~medPython()
{
    Py_Finalize();
}

void medPython::initTracebackModule()
{
    // The traceback module is used to retrieve the error messages.
    PyObject* tracebackModule = PyImport_ImportModule("traceback");

    if (tracebackModule)
    {
        PyObject* mainModule = PyImport_AddModule("__main__");

        if (mainModule)
        {
            if (PyModule_AddObject(mainModule, "traceback", tracebackModule) != 0)
            {
                Py_DECREF(tracebackModule);
                THROW_METHOD_EXCEPTION("Cannot add traceback module");
            }
        }
    }
    else
    {
        THROW_METHOD_EXCEPTION("Cannot import traceback module");
    }
}
