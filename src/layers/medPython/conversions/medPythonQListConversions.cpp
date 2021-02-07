/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonQListConversions.h"

#include "medPythonCore.h"

using namespace med::python;

namespace med::python::qlist_conversions
{

void clearList(QList<PyObject*> list)
{
    while (!list.isEmpty())
    {
        PyObject* item = list.takeFirst();
        Py_CLEAR(item);
    }
}

} // namespace med::python::qlist_conversions

bool medPython_convert(const QList<PyObject*>& qList, PyObject** output)
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

bool medPython_convert(const PyObject* object, QList<PyObject*>* output)
{
    bool success = true;
    ssize_t numItems = PySequence_Length(const_cast<PyObject*>(object));

    if (numItems != -1)
    {
        for (ssize_t i = 0; i < numItems; i++)
        {
            PyObject* item = PySequence_GetItem(const_cast<PyObject*>(object), i);

            if (item)
            {
                output->append(item);
            }
            else
            {
                qlist_conversions::clearList(*output);
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
