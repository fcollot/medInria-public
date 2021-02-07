/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonContainerConversions.h"

#include "medPythonCore.h"
#include "medPythonCoreWrappers.h"

namespace med
{
namespace python
{
namespace conversion_utils
{

void clearList(QList<PyObject*> list)
{
    while (!list.isEmpty())
    {
        PyObject* item = list.takeFirst();
        Py_CLEAR(item);
    }
}

} // conversion_utils

bool convertToPython(QList<PyObject*> qList, PyObject** output)
{
    *output = PyList_New(qList.length());

    if (*output)
    {
        for (ssize_t i = 0; i < qList.length(); i++)
        {
            PyObject* item = qList.at(i);
            Py_INCREF(item);
            PyList_SET_ITEM(*output, i, item);
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool convertFromPython(PyObject* object, QList<PyObject*>* output)
{
    bool success = true;
    ssize_t numItems = PySequence_Length(object);

    if (numItems != -1)
    {
        for (ssize_t i = 0; i < numItems; i++)
        {
            PyObject* item = PySequence_GetItem(object, i);

            if (item)
            {
                output->append(item);
            }
            else
            {
                conversion_utils::clearList(*output);
                success = false;
                break;
            }
        }
    }
    else
    {
        success = false;
    }

    return success;
}

bool convertToPython(QHash<PyObject*, PyObject*> qHash, PyObject** output)
{
    bool success = true;
    *output = PyDict_New();

    if (*output)
    {
        foreach (PyObject* key, qHash.keys())
        {
            PyObject* value = qHash.value(key);

            if (PyDict_SetItem(*output, key, value) == -1)
            {
                Py_CLEAR(*output);
                success = false;
                break;
            }
        }
    }
    else
    {
        success = false;
    }

    return success;
}

bool convertFromPython(PyObject* object, QHash<PyObject*, PyObject*>* output)
{
    bool success = true;
    PyObject* keys = PyMapping_Keys(object);

    if (keys)
    {
        ssize_t numItems = PySequence_Length(keys);

        if (numItems != -1)
        {
            for (ssize_t i = 0; i < numItems; i++)
            {
                PyObject* key = PySequence_GetItem(keys, i);

                if (key)
                {
                    PyObject* value = PyObject_GetItem(object, key);

                    if (value)
                    {
                        (*output)[key] = value;
                        continue;
                    }

                    Py_CLEAR(key);
                }

                success = false;
                break;
            }
        }
        else
        {
            success = false;
        }

        Py_CLEAR(keys);
    }
    else
    {
        success = false;
    }

    return success;
}

} // python
} // med
