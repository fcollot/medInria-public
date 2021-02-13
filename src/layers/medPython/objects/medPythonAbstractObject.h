#pragma once

#include <QList>
#include <QString>

#include "medPythonConversions.h"
#include "medPythonCoreForward.h"
#include "medPythonError.h"
#include "medPythonExport.h"

namespace med
{
namespace python
{

class ContainerAccessor;
class FunctionCall;
class Object;

class MEDPYTHON_EXPORT AbstractObject
{
public:
    virtual AbstractObject& operator=(const AbstractObject& other);
    virtual AbstractObject& operator=(PyObject* reference);

    PyObject* operator*() const;
    PyObject* newReference() const;

    void borrow(const PyObject* reference);

    Object type() const;
    QString typeName() const;

    bool hasAttribute(QString name) const;
    Object getAttribute(QString name) const;
    void setAttribute(QString name, AbstractObject& value);

    QList<QString> dir() const;

    operator bool() const;
    bool operator!() const;

    bool operator==(const AbstractObject& other) const;
    bool operator!=(const AbstractObject& other) const;
    bool operator<(const AbstractObject& other) const;
    bool operator>(const AbstractObject& other) const;
    bool operator<=(const AbstractObject& other) const;
    bool operator>=(const AbstractObject& other) const;

    Object operator+(const AbstractObject& other) const;
    Object operator-(const AbstractObject& other) const;
    Object operator*(const AbstractObject& other) const;
    Object operator/(const AbstractObject& other) const;

    AbstractObject& operator+=(const AbstractObject& other);
    AbstractObject& operator-=(const AbstractObject& other);
    AbstractObject& operator*=(const AbstractObject& other);
    AbstractObject& operator/=(const AbstractObject& other);

    ssize_t length() const;

    bool contains(const AbstractObject& object) const;

    void append(const AbstractObject& item);

    Object keys() const;
    Object values() const;

    void update(const AbstractObject& other);

    template <class TYPE>
    TYPE convert();

    template <class ARG>
    ContainerAccessor operator[](const ARG& key);

    template <class... ARGS>
    FunctionCall operator()(ARGS... args);

    template <class... ARGS>
    FunctionCall callMethod(QString name, ARGS... args);

protected:
    virtual PyObject* getReference() const = 0;
    virtual void setReference(PyObject* reference) = 0;
};

template <class TYPE>
TYPE AbstractObject::convert()
{
    TYPE result;

    if (!convertFromPython(**this, &result))
    {
        propagateCurrentError();
    }

    return result;
}

} // namespace python
} // namespace med
