#pragma once

#include "medPythonObject.h"
#include "medPythonStandardObjects.h"

namespace med
{
namespace python
{

struct FunctionCallPrivate;

class FunctionCall : public AbstractObject
{
public:
    FunctionCall(const AbstractObject& callable, const AbstractObject& args);
    virtual ~FunctionCall();

    FunctionCall& kwarg(QString name, const AbstractObject& value);

    template <class TYPE>
    FunctionCall& kwarg(QString name, const TYPE& value);

protected:
    PyObject* getReference() const override;
    void setReference(PyObject* reference) override;

private:
    FunctionCallPrivate* const d;

    PyObject* evaluate() const;
};

template <class TYPE>
FunctionCall& FunctionCall::kwarg(QString name, const TYPE& value)
{
    return kwarg(name, Object::create(value));
}

template <class... ARGS>
FunctionCall AbstractObject::operator()(ARGS... args)
{
    QList<Object> argsList;
    (argsList << ... << Object::create(args));
    return FunctionCall(*this, tuple(argsList));
}

template <class... ARGS>
FunctionCall AbstractObject::callMethod(QString name, ARGS... args)
{
    return getAttribute(name).operator()(args...);
}

} // namespace python
} // namespace med
