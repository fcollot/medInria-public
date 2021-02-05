#pragma once

#include "medPythonExport.h"
#include "medPythonObject.h"

namespace med
{

class PythonCallInstancePrivate;

class MEDPYTHON_EXPORT PythonCallInstance : public PythonObjectBase
{
public:
    PythonCallInstance(PythonObjectBase& callable, PythonObjectBase& args);
    ~PythonCallInstance();

    PythonCallInstance& kwarg(QString name, PythonObjectBase& value);

protected:
    PyObject* getReference() const override;

private:
    PythonCallInstancePrivate* const d;

    void ensureEvaluated();

    void evaluate();
};

}
