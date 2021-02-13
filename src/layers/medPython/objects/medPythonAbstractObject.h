#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

// This is the main interface for the medPython wrapping API. The wrapper
// classes handle reference counting and provide a set of predefined methods to
// simplify the use of Python objects from C++. Most of the functions in this
// class are direct wrappers of Python functions, with additional error handling
// to propagate Python excpetions to C++. It is best to consult the Python
// documentation for their use. Any Python function not wrapped here can be
// called using callMethod.
// Note: Instances of this class should only be handled by value, except in the
// case of function arguments where they can be references, provided the object
// value is immediatly used in the function. Not doing so can lead to odd
// behviour with FunctionCall and ContainerAccessor instances.

#include <QList>
#include <QString>

#include "medPythonConversions.h"
#include "medPythonCoreForward.h"
#include "medPythonErrors.h"
#include "medPythonExport.h"

namespace med::python
{

class ContainerAccessor;
class FunctionCall;
class Object;

class MEDPYTHON_EXPORT AbstractObject
{
public:
    // Replaces the wrapped object with the one wrapped by 'other'.
    virtual AbstractObject& operator=(const AbstractObject& other);

    // Replaces the wrapped object. This function takes ownership of the
    // reference. If the reference is null then the Python None object is
    // wrapped instead.
    virtual AbstractObject& operator=(PyObject* reference);

    // Returns the raw python reference without increasing the reference count.
    PyObject* operator*() const;

    // Returns the raw python reference with an increased reference count. The
    // caller owns the returned reference and is responsible for decreasing the
    // reference count when it is no longer needed.
    PyObject* newReference() const;

    // Replaces the wrapped object. The reference is borrowed, which means the
    // caller must not increase the reference count before using this function.
    void borrow(const PyObject* reference);

    Object type() const;
    QString typeName() const;

    bool hasAttribute(QString name) const;
    Object getAttribute(QString name) const;
    void setAttribute(QString name, AbstractObject& value);

    // Equivalent to the Python expression 'dir(object)'
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

    // Equivalent to the Python expression 'len(Object)'
    ssize_t length() const;

    // Equivalent to the Python expression 'object in other'
    bool contains(const AbstractObject& object) const;

    void append(const AbstractObject& item);

    Object keys() const;
    Object values() const;

    void update(const AbstractObject& other);

    // Converts the Python object to a C++ type using the associated overload of
    // medPython_convert. The caller owns the returned object.
    template <class TYPE>
    TYPE convert();

    // Due to ciruclar dependency issues, the following three template functions
    // are defined in the header file of their respective return types.

    template <class ARG>
    ContainerAccessor operator[](const ARG& key);

    template <class... ARGS>
    FunctionCall operator()(ARGS... args);

    template <class... ARGS>
    FunctionCall callMethod(QString name, ARGS... args);

protected:
    virtual PyObject* getReference() const = 0;
    virtual void setReference(PyObject* reference) = 0;

    void unsupportedFunctionError(QString functionName);
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

} // namespace med::python
