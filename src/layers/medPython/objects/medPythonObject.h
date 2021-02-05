#pragma once

#include <QList>
#include <QString>

#include "medPythonConvert.h"
#include "medPythonExport.h"
#include "medPythonFunctionCall.h"
#include "medPythonMappingAccessor.h"
#include "medPythonSequenceAccessor.h"

namespace med
{
namespace python
{

class ObjectPrivate;

class MEDPYTHON_EXPORT Object
{
public:
    template <class CTYPE>
    static Object create(const CTYPE& value);

    static Object borrowed(const PyObject* nativeObject);

    Object(double value);
    Object(QString value);

    Object(const Object& other);
    virtual Object& operator=(const Object& other);

    Object(PyObject* nativeObject = nullptr);
    Object& operator=(PyObject* nativeObject);

    virtual ~Object();

    PyObject* operator*() const;

    void borrow(const PyObject* nativeObject);
    PyObject* newReference() const;

    Object type() const;
    QString typeName() const;

    bool hasAttribute(QString name) const;
    Object getAttribute(QString name) const;
    void setAttribute(QString name, Object& value);

    QList<QString> dir() const;

    operator bool() const;
    bool operator!() const;

    bool operator==(Object& other) const;
    bool operator!=(Object& other) const;
    bool operator<(Object& other) const;
    bool operator>(Object& other) const;
    bool operator<=(Object& other) const;
    bool operator>=(Object& other) const;

    Object operator+(Object& other) const;
    Object operator-(Object& other) const;
    Object operator*(Object& other) const;
    Object operator/(Object& other) const;

    Object& operator+=(Object& other);
    Object& operator-=(Object& other);
    Object& operator*=(Object& other);
    Object& operator/=(Object& other);

    void append(Object& item);

    ssize_t length() const;

    bool contains(Object& item) const;

    Object keys() const;
    Object values() const;

    void update(Object& other);

    template <class CTYPE>
    CTYPE convert();

    template <class ARG>
    MappingAccessor<Object> operator[](ARG& key);

    SequenceAccessor<Object> operator[](ssize_t index);

    template <class... ARGS>
    FunctionCall<Object> operator()(ARGS... args);

    template <class... ARGS>
    FunctionCall<Object> callMethod(QString name, ARGS... args);

protected:
    PyObject* getReference() const;
    virtual PyObject* getReference();
    virtual void setReference(PyObject* reference);

private:
    ObjectPrivate* const d;

    Object createArgsList(QList<Object> args);
};

// Awkward conversion class needed for containers of type Object (i.e. Qlist<Object>)
template <>
class Convert<Object>
{
public:
    static PyObject* toPython(Object object)
    {
        return object.newReference();
    }

    static Object fromPython(PyObject* object)
    {
        return object;
    }
};

template<class CTYPE>
Object Object::create(const CTYPE& value)
{
    return Convert<CTYPE>::toPython(value);
}

template<class CTYPE>
CTYPE Object::convert()
{
    return Convert<CTYPE>::fromPython(**this);
}

template <class ARG>
MappingAccessor<Object> Object::operator[](ARG& key)
{
    Object pythonKey = Convert<ARG>::toPython(key);
    return MappingAccessor(*this, pythonKey);
}

template<class... ARGS>
FunctionCall<Object> Object::operator()(ARGS... args)
{
    std::initializer_list<Object> argsList = {args...};
    Object argsObject = createArgsList(argsList);
    return FunctionCall<Object>(*this, *argsObject);
}

template<class... ARGS>
FunctionCall<Object> Object::callMethod(QString name, ARGS... args)
{
    return getAttribute(name).operator()(args...);
}

}
}
