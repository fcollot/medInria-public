#include "medPythonUtils.h"

#include "medPythonError.h"

void medPythonUtils::runCode(QString pythonCode, PyObject* globals, PyObject** output)
{
    if (!globals)
    {
        globals = PyModule_GetDict(PyImport_AddModule("__main__"));
    }

    PyObject* result = PyRun_String(TO_CSTRING(pythonCode), Py_file_input, globals, nullptr);

    CHECK_PYTHON_ERROR();

    if (output)
    {
        *output = result;
    }
    else
    {
        Py_XDECREF(result);
    }
}

void medPythonUtils::addSysPath(QString path)
{
    runCode(QString("import sys\nsys.path.append(\"") + path + "\")");
}

void medPythonUtils::removeSysPath(QString path)
{
    runCode(QString("import sys\nsys.path.remove(\"") + path + "\")");
}
