#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <QList>

#include "medPythonCoreForward.h"
#include "medPythonExport.h"

namespace med::python::qlist_conversions
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

} // namespace med::python::qlist_conversions

MEDPYTHON_EXPORT bool medPython_convert(const QList<PyObject*>& qList, PyObject** output);

template <class TYPE>
MEDPYTHON_EXPORT bool medPython_convert(const QList<TYPE>& qList, PyObject** output)
{
    bool success = true;
    QList<PyObject*> objectList;

    foreach (TYPE item, qList)
    {
        PyObject* pythonItem;

        if (medPython_convert(item, &pythonItem))
        {
            objectList.append(pythonItem);
        }
        else
        {
            success = false;
            break;
        }
    }

    success = success && medPython_convert(objectList, output);
    med::python::qlist_conversions::clearList(objectList);
    return success;
}

MEDPYTHON_EXPORT bool medPython_convert(const PyObject* object, QList<PyObject*>* output);

template <class TYPE>
MEDPYTHON_EXPORT bool medPython_convert(const PyObject* object, QList<TYPE>* output)
{
    bool success = true;
    QList<PyObject*> objectList;

    if (medPython_convert(object, &objectList))
    {
        output->clear();

        foreach (PyObject* pythonItem, objectList)
        {
            TYPE item;

            if (medPython_convert(pythonItem, &item))
            {
                output->append(item);
            }
            else
            {
                med::python::qlist_conversions::clearList(*output);
                success = false;
                break;
            }
        }

        med::python::qlist_conversions::clearList(objectList);
    }

    return success;
}
