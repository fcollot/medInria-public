#include "medPythonTypeUtils.h"

#include "medPythonError.h"

QString medPythonTypeUtils::fromString(PyObject* pythonString)
{
    CHECK_PYOBJECT_IS_NOT_NULL(pythonString);

    PyObject* stringAsPyBytes = nullptr;

    if (PyBytes_Check(pythonString))
    {
        stringAsPyBytes = pythonString;
    }
    else if (PyUnicode_Check(pythonString))
    {
        stringAsPyBytes = PyUnicode_AsEncodedString(pythonString, "UTF-8", "strict");

        CHECK_PYTHON_ERROR();
    }

    QString result = strdup(PyBytes_AS_STRING(stringAsPyBytes));
    Py_DECREF(stringAsPyBytes);
    return result;
}

PyObject* medPythonTypeUtils::toPython(QString string)
{
    PyObject* pythonString = PyUnicode_FromString(TO_CSTRING(string));
    CHECK_PYTHON_ERROR();
    return pythonString;
}

PyObject* medPythonTypeUtils::toPython(QStringList list)
{
    PyObject* pythonList = PyList_New(list.length());
    CHECK_PYTHON_ERROR();

    foreach (QString item, list)
    {
        PyObject* pythonItem = toPython(item);
        PyList_SET_ITEM(pythonList, 0, pythonItem);
    }

    return pythonList;
}

QString medPythonTypeUtils::getStringAttribute(PyObject* object, QString attributeName)
{
    PyObject* value = PyObject_GetAttrString(object, attributeName.toStdString().c_str());
    CHECK_PYTHON_ERROR();

    QString valueAsString = fromString(value);
    Py_DECREF(value);
    return valueAsString;
}
