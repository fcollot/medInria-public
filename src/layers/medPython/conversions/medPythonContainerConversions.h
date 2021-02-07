#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <QHash>
#include <QList>

#include "medPythonCoreForward.h"
#include "medPythonError.h"
#include "medPythonPrimitiveConversions.h"

namespace med
{
namespace python
{
namespace conversion_utils
{

template <class TYPE>
void clearList(QList<TYPE> list)
{
    list.clear();
}

template <class TYPE>
void clearList(QList<TYPE*> list)
{
    while (!list.isEmpty())
    {
        delete list.takeFirst();
    }
}

void clearList(QList<PyObject*> list);

template <class KEY_TYPE, class VALUE_TYPE>
void clearHash(QHash<KEY_TYPE, VALUE_TYPE> hash)
{
    QList<KEY_TYPE> keys = hash.keys();
    QList<VALUE_TYPE> values = hash.values();
    hash.clear();
    clearList(keys);
    clearList(values);
}

} // conversion_utils

bool convertToPython(QList<PyObject*> qList, PyObject** output);

template <class TYPE>
bool convertToPython(QList<TYPE> qList, PyObject** output)
{
    bool success = true;
    QList<PyObject*> objectList;

    foreach (TYPE item, qList)
    {
        PyObject* pythonItem;

        if (convertToPython(item, &pythonItem))
        {
            objectList.append(pythonItem);
        }
        else
        {
            success = false;
            break;
        }
    }

    success = success && convertToPython(objectList, output);
    conversion_utils::clearList(objectList);
    return success;
}

bool convertFromPython(PyObject* object, QList<PyObject*>* output);

template <class TYPE>
bool convertFromPython(PyObject* object, QList<TYPE>* output)
{
    bool success = true;
    QList<PyObject*> objectList;

    if (convertFromPython(object, &objectList))
    {
        output->clear();

        foreach (PyObject* pythonItem, objectList)
        {
            TYPE item;

            if (convertFromPython(pythonItem, &item))
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

        conversion_utils::clearList(objectList);
    }

    return success;
}

bool convertToPython(QHash<PyObject*, PyObject*> qHash, PyObject** output);

template <class KEY_TYPE, class VALUE_TYPE>
bool convertToPython(QHash<KEY_TYPE, VALUE_TYPE> qHash, PyObject** output)
{
    bool success = true;
    QHash<PyObject*, PyObject*> objectHash;

    foreach (KEY_TYPE key, qHash.keys())
    {
        PyObject* pythonKey;

        if (convertToPython(key, &pythonKey))
        {
            PyObject* pythonValue;

            if (convertToPython(qHash.value(key), &pythonValue))
            {
                objectHash[pythonKey] = pythonValue;
                continue;
            }

            core_forward::decref(pythonKey);
        }

        success = false;
        break;
    }

    success = success && convertToPython(objectHash, output);
    conversion_utils::clearHash(objectHash);
    return success;
}

bool convertFromPython(PyObject* object, QHash<PyObject*, PyObject*>* output);

template <class KEY_TYPE, class VALUE_TYPE>
bool convertFromPython(PyObject* object, QHash<KEY_TYPE, VALUE_TYPE>* output)
{
    bool success = true;
    QHash<PyObject*, PyObject*> objectHash;

    if (convertFromPython(object, &objectHash))
    {
        output->clear();
        QHash<PyObject*, PyObject*>::iterator hashItem;

        for (hashItem = objectHash.begin(); hashItem != objectHash.end(); hashItem++)
        {
            KEY_TYPE key;

            if (convertFromPython(hashItem.key(), &key))
            {
                VALUE_TYPE value;

                if (convertFromPython(hashItem.value(), &value))
                {
                    (*output)[key] = value;
                    continue;
                }
                else
                {
                    // add the key with a default value to the hash so that
                    // clearHash takes care of deleting it properly.
                    (*output)[key];
                }
            }

            success = false;
            break;
        }

        conversion_utils::clearHash(objectHash);

        if (!success)
        {
            conversion_utils::clearHash(*output);
        }
    }

    return success;
}

} // namespace python
} // namespace med
