#pragma once

#include "medPythonAbstractObject.h"
#include "medPythonExport.h"

namespace med
{
namespace python
{

struct ObjectPrivate;

class MEDPYTHON_EXPORT Object : public AbstractObject
{
public:
    template <class TYPE>
    static Object create(const TYPE& value);

    Object(PyObject* reference = nullptr);
    Object(const Object& other);
    Object(const AbstractObject& other);

    Object(int value);
    Object(double value);
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

} // namespace python
} // namespace med
