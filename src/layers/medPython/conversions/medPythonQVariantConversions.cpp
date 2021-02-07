/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonQVariantConversions.h"

#include "medPythonCore.h"
#include "medPythonErrors.h"
#include "medPythonConversions.h"

using namespace med::python;

bool medPython_convert(const QVariant& value, PyObject** output)
{
    switch (static_cast<int>(value.type()))
    {
    case QMetaType::Bool:
        return medPython_convert(value.toBool(), output);
    case QMetaType::Int:
        return medPython_convert(value.toInt(), output);
    case QMetaType::Float:
    case QMetaType::Double:
        return medPython_convert(value.toDouble(), output);
    case QMetaType::QString:
        return medPython_convert(value.toString(), output);
    case QMetaType::QVariantList:
        return medPython_convert(value.toList(), output);
    case QMetaType::QStringList:
        return medPython_convert(value.toStringList(), output);
    case QMetaType::QVariantHash:
        return medPython_convert(value.toHash(), output);
    default:
        QString message = QString("Cannot convert QVariant of type %1 to Python")
                          .arg(value.typeName());
        raiseError<TypeError>(message);
        return false;
    }
}

template <class TYPE>
bool medPython_convert(const PyObject* object, QVariant* output)
{
    TYPE value;

    if (medPython_convert(object, &value))
    {
        *output = QVariant::fromValue<TYPE>(value);
        return true;
    }
    else
    {
        return false;
    }
}

bool medPython_convert(const PyObject* object, QVariant* output)
{
    if (PyBool_Check(object))
    {
        return medPython_convert<bool>(object, output);
    }
    else if (PyLong_Check(object))
    {
        return medPython_convert<long>(object, output);
    }
    else if (PyFloat_Check(object))
    {
        return medPython_convert<double>(object, output);
    }
    else if (PyUnicode_Check(object))
    {
        return medPython_convert<QString>(object, output);
    }
    else if (PyList_Check(object))
    {
        return medPython_convert<QList<QVariant> >(object, output);
    }
    else if (PyDict_Check(object))
    {
        return medPython_convert<QHash<QString, QVariant> >(object, output);
    }
    else
    {
        QString message = QString("Cannot convert Python object of type %1 to"
                                  " QVariant").arg(Py_TYPE(object)->tp_name);
        raiseError<TypeError>(message);
        return false;
    }
}
