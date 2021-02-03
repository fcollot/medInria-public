#pragma once

#include <QList>

#include "medPythonContainerFunctions.h"
#include "medPythonConvert.h"
#include "medPythonCoreWrappers.h"
#include "medPythonReference.h"

namespace med
{
namespace python
{
namespace internal
{

template <class CTYPE>
class Convert<QList<CTYPE> >
{
public:
    static Reference toPython(QList<CTYPE> qList)
    {
        Reference pythonList = createList(qList.length());

        for (size_t i = 0; i < qList.length(); i++)
        {
            Reference pythonItem = Convert<CTYPE>::toPython(qList[i]);
            setItem(pythonList, i, pythonItem);
        }

        return pythonList;
    }

    static QList<CTYPE> fromPython(const AbstractReference& sequence)
    {
        QList<CTYPE> qList;
        size_t numItems = getSize(sequence);

        for (size_t i = 0; i < numItems; i++)
        {
            Reference item = getItem(sequence, i);
            qList.append(Convert<CTYPE>::fromPython(item));
        }

        return qList;
    }
};

template <class KEY_TYPE, class VALUE_TYPE>
class Convert<QHash<KEY_TYPE, VALUE_TYPE> >
{
public:
    static Reference toPython(QHash<KEY_TYPE, VALUE_TYPE> qHash)
    {
        Reference pythonDict = createDict();

        foreach (const KEY_TYPE& key, qHash.keys())
        {
            Reference pythonKey = Convert<KEY_TYPE>::toPython(key);
            Reference pythonValue = Convert<VALUE_TYPE>::toPython(qHash[key]);
            setItem(pythonDict, pythonKey, pythonValue);
        }

        return pythonDict;
    }

    static QHash<KEY_TYPE, VALUE_TYPE> fromPython(const AbstractReference& mapping)
    {
        QHash<KEY_TYPE, VALUE_TYPE> qHash;
        Reference keys = mappingKeys(mapping);
        QList<Reference> keysList = Convert<QList<Reference> >::fromPython(keys);

        foreach (const Reference& pythonKey, keysList)
        {
            Reference pythonValue = getItem(mapping, pythonKey);
            KEY_TYPE key = Convert<KEY_TYPE>::fromPython(pythonKey);
            VALUE_TYPE value = Convert<VALUE_TYPE>::fromPython(pythonValue);
            qHash[key] = value;
        }

        return qHash;
    }
};

}
}
}
