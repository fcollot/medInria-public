#include "medPythonUtils.h"

#include "medPythonError.h"

void medPythonUtils::runPythonString(QString pythonString, PyObject* globals, PyObject** output)
{
    PyErr_Clear();

    if (!globals)
    {
        globals = PyModule_GetDict(PyImport_AddModule("__main__"));
    }

    PyObject* result = PyRun_String(pythonString.toStdString().c_str(), Py_file_input, globals, nullptr);

    if (!PyErr_Occurred())
    {
        if (output)
        {
            *output = result;
        }
        else
        {
            Py_XDECREF(result);
        }
    }
    else
    {
        THROW_PYTHON_EXCEPTION();
    }
}

void medPythonUtils::addSysPath(QString path)
{
    runPythonString(QString("import sys\nsys.path.append(\"") + path + "\")");
}

void medPythonUtils::removeSysPath(QString path)
{
    runPythonString(QString("import sys\nsys.path.remove(\"") + path + "\")");
}
