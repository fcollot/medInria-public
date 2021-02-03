#pragma once

#include <QString>
#include <QList>

#include "medPythonAbstractReference.h"
#include "medPythonContainerConversions.h"
#include "medPythonCore.h"
#include "medPythonExport.h"
#include "medPythonFunctionCallReference.h"

namespace med
{
namespace python
{

class MEDPYTHON_EXPORT Object
{
public:
//    static Object take(PyObject* nativeObject);
//    static Object borrow(PyObject* nativeObject);

    Object(PyObject* nativeObject = nullptr);
    Object(const Object& other);
    Object& operator=(const Object& other);
    virtual ~Object();

    Object(long value);
    Object(double value);
    Object(QString value);

    PyObject* operator*();
    PyObject* operator*() const;

    PyObject* takeReference();
    PyObject* newReference() const;

    Object type();
    QString typeName();

    bool hasAttribute(QString name);
    Object getAttribute(QString name);
    void setAttribute(QString name, Object value);

    QStringList dir();

    operator bool();
    bool operator!();

    bool operator==(Object& other);
    bool operator!=(Object& other);
    bool operator<(Object& other);
    bool operator>(Object& other);
    bool operator<=(Object& other);
    bool operator>=(Object& other);

    Object operator+(Object& other);
    Object operator-(Object& other);
    Object operator*(Object& other);
    Object operator/(Object& other);

    Object& operator+=(Object& other);
    Object& operator-=(Object& other);
    Object& operator*=(Object& other);
    Object& operator/=(Object& other);

    void append(const Object& item);

    size_t length() const;

    bool contains(Object& item) const;

    Object keys() const;
    Object values() const;

    Object operator[](const Object& key);
    Object operator[](size_t i);

    template <class... ARGS>
    Object operator()(ARGS... args);

    template <class... ARGS>
    Object callMethod(QString name, ARGS... args);

private:
    internal::AbstractReference reference;

    Object(internal::AbstractReference* reference);
};

template<class... ARGS>
Object Object::operator()(ARGS... args)
{
    Object argsList = Convert<QList<Object> >::toPython({args...});
    return new FunctionCallReference(**this, *argsList);
}

template<class... ARGS>
Object Object::callMethod(QString name, ARGS... args)
{
    return getAttribute(name).operator()(args...);
}

}
}
