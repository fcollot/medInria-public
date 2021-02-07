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

#include "medPythonCoreForward.h"
#include "medPythonExport.h"
#include "medPythonQListConversions.h"

namespace med::python::qhash_conversions
{

template <class KEY_TYPE, class VALUE_TYPE>
void clearHash(QHash<KEY_TYPE, VALUE_TYPE> hash)
{
    QList<KEY_TYPE> keys = hash.keys();
    QList<VALUE_TYPE> values = hash.values();
    hash.clear();
    qlist_conversions::clearList(keys);
    qlist_conversions::clearList(values);
}

} // namespace med::python::qhash_conversions

MEDPYTHON_EXPORT bool medPython_convert(const QHash<PyObject*, PyObject*>& qHash, PyObject** output);

template <class KEY_TYPE, class VALUE_TYPE>
MEDPYTHON_EXPORT bool medPython_convert(const QHash<KEY_TYPE, VALUE_TYPE>& qHash, PyObject** output)
{
    bool success = true;
    QHash<PyObject*, PyObject*> objectHash;

    foreach (KEY_TYPE key, qHash.keys())
    {
        PyObject* pythonKey;

        if (medPython_convert(key, &pythonKey))
        {
            PyObject* pythonValue;

            if (medPython_convert(qHash.value(key), &pythonValue))
            {
                objectHash[pythonKey] = pythonValue;
                continue;
            }

            med::python::decref(pythonKey);
        }

        success = false;
        break;
    }

    success = success && medPython_convert(objectHash, output);
    med::python::qhash_conversions::clearHash(objectHash);
    return success;
}

MEDPYTHON_EXPORT bool medPython_convert(const PyObject* object, QHash<PyObject*, PyObject*>* output);

template <class KEY_TYPE, class VALUE_TYPE>
MEDPYTHON_EXPORT bool medPython_convert(const PyObject* object, QHash<KEY_TYPE, VALUE_TYPE>* output)
{
    bool success = true;
    QHash<PyObject*, PyObject*> objectHash;

    if (medPython_convert(object, &objectHash))
    {
        output->clear();
        QHash<PyObject*, PyObject*>::iterator hashItem;

        for (hashItem = objectHash.begin(); hashItem != objectHash.end(); hashItem++)
        {
            KEY_TYPE key;

            if (medPython_convert(hashItem.key(), &key))
            {
                VALUE_TYPE value;

                if (medPython_convert(hashItem.value(), &value))
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

        med::python::qhash_conversions::clearHash(objectHash);

        if (!success)
        {
            med::python::qhash_conversions::clearHash(*output);
        }
    }

    return success;
}
