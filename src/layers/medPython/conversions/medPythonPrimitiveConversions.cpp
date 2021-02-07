/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonPrimitiveConversions.h"

#include "medPythonContainerConversions.h"
#include "medPythonCore.h"
#include "medPythonCoreWrappers.h"
#include "medPythonError.h"

namespace med
{
namespace python
{

bool convertToPython(bool value, PyObject** output)
{
    *output = PyBool_FromLong(value);
    return *output;
}

bool convertFromPython(PyObject* object, bool* output)
{
    *output = PyObject_IsTrue(object);
    return !PyErr_Occurred();
}

bool convertToPython(long value, PyObject** output)
{
    *output = PyLong_FromLong(value);
    return *output;
}

bool convertFromPython(PyObject* object, long* output)
{
    *output = PyLong_AsLong(object);
    return !PyErr_Occurred();
}

bool convertToPython(double value, PyObject** output)
{
    *output = PyFloat_FromDouble(value);
    return *output;
}

bool convertFromPython(PyObject* object, double* output)
{
    *output = PyFloat_AsDouble(object);
    return !PyErr_Occurred();
}

bool convertToPython(QString value, PyObject** output)
{
    *output = PyUnicode_FromString(qUtf8Printable(value));
    return *output;
}

bool convertFromPython(PyObject* object, QString* output)
{
    PyObject* stringAsPyBytes = nullptr;

    if (PyBytes_Check(object))
    {
        stringAsPyBytes = object;
        Py_INCREF(stringAsPyBytes);
    }
    else if (PyUnicode_Check(object))
    {
        stringAsPyBytes = PyUnicode_AsEncodedString(object, "UTF-8", "strict");
    }

    if (!PyErr_Occurred())
    {
        if (stringAsPyBytes)
        {
            *output = strdup(PyBytes_AS_STRING(stringAsPyBytes));
            Py_CLEAR(stringAsPyBytes);
            return true;
        }
        else
        {
            // The object is not a string, so apply str() and retry
            PyObject* objectAsString = PyObject_Str(object);

            if (!PyErr_Occurred())
            {
                convertFromPython(objectAsString, output);
                Py_CLEAR(objectAsString);
                return PyErr_Occurred();
            }
        }
    }

    return false;
}

bool convertToPython(QVariant value, PyObject** output)
{
    switch (static_cast<int>(value.type()))
    {
    case QMetaType::Bool:
        return convertToPython(value.toBool(), output);
    case QMetaType::Int:
        return convertToPython(static_cast<long>(value.toInt()), output);
    case QMetaType::Float:
    case QMetaType::Double:
        return convertToPython(value.toDouble(), output);
    case QMetaType::QString:
        return convertToPython(value.toString(), output);
    case QMetaType::QVariantList:
        return convertToPython(value.toList(), output);
    case QMetaType::QStringList:
        return convertToPython(value.toStringList(), output);
    case QMetaType::QVariantHash:
        return convertToPython(value.toHash(), output);
    default:
        QString message = QString("Cannot convert QVariant of type %1 to Python")
                          .arg(value.typeName());
        raiseError<TypeError>(message);
        return false;
    }
}

template <class TYPE>
bool convertFromPython(PyObject* object, QVariant* output)
{
    TYPE value;

    if (convertFromPython(object, &value))
    {
        *output = QVariant::fromValue<TYPE>(value);
        return true;
    }
    else
    {
        return false;
    }
}

bool convertFromPython(PyObject* object, QVariant* output)
{
    if (PyBool_Check(object))
    {
        return convertFromPython<bool>(object, output);
    }
    else if (PyLong_Check(object))
    {
        return convertFromPython<long>(object, output);
    }
    else if (PyFloat_Check(object))
    {
        return convertFromPython<double>(object, output);
    }
    else if (PyUnicode_Check(object))
    {
        return convertFromPython<QString>(object, output);
    }
    else if (PyList_Check(object))
    {
        return convertFromPython<QList<QVariant> >(object, output);
    }
    else if (PyDict_Check(object))
    {
        return convertFromPython<QHash<QString, QVariant> >(object, output);
    }
    else
    {
        QString message = QString("Cannot convert Python object of type %1 to"
                                  " QVariant").arg(Py_TYPE(object)->tp_name);
        raiseError<TypeError>(message);
        return false;
    }
}

} // namespace python
} // namespace med
