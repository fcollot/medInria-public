#pragma once

#include <QList>

#include "medPythonError.h"
#include "medPythonPrimitiveConversions.h"
#include "medPythonUtils.h"

namespace med
{
namespace python
{

template <class CTYPE>
class Convert<QList<CTYPE> >
{
public:
    static PyObject* toPython(QList<CTYPE> qList)
    {
        PyObject* pythonList = utils::createList(qList.length());
        propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);

        for (ssize_t i = 0; i < qList.length(); i++)
        {
            try
            {
                PyObject* pythonItem = Convert<CTYPE>::toPython(qList[i]);
                utils::setItem(pythonList, i, pythonItem);
                utils::decref(pythonItem);
                propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);
            }
            catch (...)
            {
                utils::decref(pythonList);
                throw;
            }
        }

        return pythonList;
    }

    static QList<CTYPE> fromPython(PyObject* sequence)
    {
        ssize_t numItems = utils::length(sequence);
        propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);

        QList<CTYPE> qList;

        for (ssize_t i = 0; i < numItems; i++)
        {
            PyObject* pythonItem = utils::getItem(sequence, i);
            propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);

            try
            {
                CTYPE item = Convert<CTYPE>::fromPython(pythonItem);
                utils::decref(pythonItem);
                qList.append(item);
            }
            catch (...)
            {
                utils::decref(pythonItem);
                throw;
            }
        }

        return qList;
    }
};

template <class KEY_TYPE, class VALUE_TYPE>
class Convert<QHash<KEY_TYPE, VALUE_TYPE> >
{
public:
    static PyObject* toPython(QHash<KEY_TYPE, VALUE_TYPE> qHash)
    {
        PyObject* pythonDict = utils::createDict();
        propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);

        foreach (const KEY_TYPE& key, qHash.keys())
        {
            PyObject* pythonKey = nullptr;
            PyObject* pythonValue = nullptr;

            try
            {
                pythonKey = Convert<KEY_TYPE>::toPython(key);
                pythonValue = Convert<VALUE_TYPE>::toPython(qHash[key]);
                utils::setItem(pythonDict, pythonKey, pythonValue);
                propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);
                utils::decref(pythonKey);
                utils::decref(pythonValue);
            }
            catch (...)
            {
                utils::decref(pythonKey);
                utils::decref(pythonValue);
                utils::decref(pythonDict);
                throw;
            }
        }

        return pythonDict;
    }

    static QHash<KEY_TYPE, VALUE_TYPE> fromPython(PyObject* mapping)
    {
        PyObject* keys = utils::keys(mapping);
        propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);

        try
        {
            ssize_t numKeys = utils::length(keys);
            propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);

            QHash<KEY_TYPE, VALUE_TYPE> qHash;

            for (ssize_t i = 0; i < numKeys; i++)
            {
                PyObject* pythonKey = nullptr;
                PyObject* pythonValue = nullptr;

                try
                {
                    pythonKey = utils::getItem(keys, i);
                    propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);

                    pythonValue = utils::getItem(mapping, pythonKey);
                    propagateCurrentError(MEDPYTHON_CODE_LOCATION);

                    KEY_TYPE key = Convert<KEY_TYPE>::fromPython(pythonKey);
                    VALUE_TYPE value = Convert<VALUE_TYPE>::fromPython(pythonValue);
                    qHash[key] = value;
                }
                catch (...)
                {
                    utils::decref(pythonKey);
                    utils::decref(pythonValue);
                }
            }
        }
        catch (...)
        {
            utils::decref(keys);
            throw;
        }

        return qHash;
    }
};

}
}
