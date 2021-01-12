#include "medPythonTypeUtils.h"

#include "medPythonErrorChecker.h"

QString medPythonTypeUtils::fromString(PyObject* pythonString)
{
    PyObject* stringAsPyBytes = nullptr;

    if (PyBytes_Check(pythonString))
    {
        stringAsPyBytes = pythonString;
    }
    else if (PyUnicode_Check(pythonString))
    {
        stringAsPyBytes = PyUnicode_AsEncodedString(pythonString, "UTF-8", "strict");

        if (stringAsPyBytes == nullptr)
        {
            THROW_EXCEPTION("Unhandled encoding");
        }
    }

    QString result = strdup(PyBytes_AS_STRING(stringAsPyBytes));
    Py_DECREF(stringAsPyBytes);
    return result;
}

PyObject* medPythonTypeUtils::toPython(QString string)
{
    return PyUnicode_FromString(qUtf8Printable(string));
}

PyObject* medPythonTypeUtils::toPython(QStringList list)
{
    medPythonErrorChecker errorChecker;
    errorChecker.clear();

    PyObject* pythonList = PyList_New(list.length());
    CHECK_PYTHON_ERROR(errorChecker);

    foreach (QString item, list)
    {
        PyObject* pythonItem = toPython(qUtf8Printable(item));
        PyList_SET_ITEM(pythonList, 0, pythonItem);
    }

    return pythonList;
}

void medPythonTypeUtils::pythonToCppConversionError(PyObject* input, QString outputType)
{
    QString message = "Could not convert python object of type \"";
    message += Py_TYPE(input)->tp_name;
    message += "\" to ";
    message += outputType;
    message += "\n";
    PyErr_SetString(PyExc_RuntimeError, message.toStdString().c_str());
}

void medPythonTypeUtils::cppToPythonConversionError(QString inputType, QString outputType)
{
    QString message = "Could not convert cpp object of type \"";
    message += inputType;
    message += "\" to ";
    message += outputType;
    message += "\n";
    PyErr_SetString(PyExc_RuntimeError, message.toStdString().c_str());
}

QString medPythonTypeUtils::getStringAttribute(PyObject* object, QString attributeName)
{
    PyObject* value = PyObject_GetAttrString(object, attributeName.toStdString().c_str());

    if (!value)
    {
        if (PyErr_Occurred())
        {
            // todo;
        }
        THROW_EXCEPTION("Unhandled encoding");
    }

    QString valueAsString = fromString(value);
    Py_DECREF(value);
    return valueAsString;
}
