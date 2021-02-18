#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

// This is the wrapper class for concrete Python objects. See the AbstractObject
// interface for more information.

#include "medPythonAbstractObject.h"
#include "medPythonExport.h"

namespace med::python
{

struct ObjectPrivate;

class MEDPYTHON_EXPORT Object : public AbstractObject
{
public:
    // Convert a C++ object to a wrapped Python object using the associated
    // overload of the medPython_convert function.
    template <class TYPE>
    static Object create(const TYPE& value);

    // Wrap a Python object and increase its reference count (the caller keeps
    // ownership of the reference).
    static Object borrowed(const PyObject* reference);

    // Wrap a python object. This takes ownership of the reference. If the
    // reference is null then the Python None object is wrapped instead.
    Object(PyObject* reference = nullptr);

    // Share the object wrapped by 'other'.
    Object(const Object& other);

    // Share the object wrapped by 'other'.
    Object(const AbstractObject& other);

    // Creates a Python integer
    Object(int value);

    // Creates a Python float.
    Object(double value);

    // Creates a Python string.
    Object(QString value);

    virtual ~Object();

    using AbstractObject::operator=;

protected:
    ObjectPrivate* const d;

    PyObject* getReference() const override;
    void setReference(PyObject* reference) override;

private:
    void internalSetReference(PyObject* reference);
};

template <class TYPE>
Object Object::create(const TYPE& value)
{
    PyObject* reference;

    if (!convertToPython(value, &reference))
    {
        propagateCurrentError();
    }

    return reference;
}

} // namespace med::python
