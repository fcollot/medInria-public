#pragma once

#include <QHash>

#include "medPythonObject.h"

namespace med
{
namespace python
{

Object none();

Object integer(long value = 0);

Object integer(const Object& object);

Object floating(double value = 0);

Object floating(const Object& object);

Object string(QString string = QString());

Object string(const Object& object);

Object list();

template <class TYPE>
Object list(QList<TYPE> qList)
{
    return Object::create<QList<TYPE> >(qList);
}

Object list(const Object& object);

template <class TYPE>
Object tuple(QList<TYPE> qList)
{
    tuple(Object::create<QList<TYPE> >(qList));
}

Object tuple(const Object& object = Object::create(QList<Object>()));

Object dict();

template <class KEY_TYPE, class VALUE_TYPE>
Object dict(QHash<KEY_TYPE, VALUE_TYPE> qHash)
{
    return Object::create<QHash<KEY_TYPE, VALUE_TYPE> >(qHash);
}

}
}
