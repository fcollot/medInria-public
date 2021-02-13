#pragma once

#include <QHash>

#include "medPythonObject.h"

namespace med
{
namespace python
{

Object none();

Object list();

Object list(const Object& object);

template <class TYPE>
Object list(QList<TYPE> qList)
{
    return Object::create(qList);
}

Object tuple();

Object tuple(const Object& object);

template <class TYPE>
Object tuple(QList<TYPE> qList)
{
    return tuple(Object::create(qList));
}

Object dict();

template <class KEY_TYPE, class VALUE_TYPE>
Object dict(QHash<KEY_TYPE, VALUE_TYPE> qHash)
{
    return Object::create<QHash<KEY_TYPE, VALUE_TYPE> >(qHash);
}

}
}
