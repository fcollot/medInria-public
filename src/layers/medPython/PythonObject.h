#pragma once

#include <QString>

#include "Export.h"
#include "Python.h"

namespace medPython
{

class PythonObjectPrivate;
class PythonType;

class MEDPYTHON_EXPORT PythonObject
{
public:
    static PythonObject borrowed(PyObject* object);

    PythonObject(PyObject* object = nullptr);
    PythonObject(QString string);
    PythonObject(long integer);
    PythonObject(const PythonObject& other);
    PythonObject& operator=(const PythonObject& other);
    virtual ~PythonObject();

    void clear();

    operator bool() const;
    bool operator!() const;

    virtual void borrow(PyObject* object);

    PyObject* data();
    const PyObject* data() const;

    PyObject* takeReference();
    PyObject* newReference();

    PythonType getType();

    template<class TYPE>
    TYPE cast();

    bool hasAttribute(QString name);
    PythonObject getAttribute(QString name);
    void setAttribute(QString name, PythonObject value);

    PythonObject callMethod(QString name, PythonObject args = nullptr, PythonObject kwargs = nullptr);

    PythonObject call(PythonObject args = nullptr, PythonObject kwargs = nullptr);

    QString toString();
    long toInteger();

protected:
    void setObject(PyObject* object);

private:
    PythonObjectPrivate* const d;

    void internalBorrow(PyObject* object);

    template<class TYPE>
    TYPE* internalCast(bool disown);
};


}
