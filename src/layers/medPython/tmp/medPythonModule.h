#pragma once

#include <QStringList>

#include "medPythonDict.h"
#include "medPythonList.h"

namespace med
{

int isModuleObject(PyObject* object);

class MEDPYTHON_EXPORT PythonModule : public TypeCheckedPythonObject<&isModuleObject>
{
public:
    static PythonModule createModule(QString name);
    static PythonModule createPackage(QString name);

    static PythonModule get(QString name);

    static PythonObject import(QString name, QList<QString> fromList = {});
    static PythonModule importIntoMain(QString name, QList<QString> fromList = {});

    PythonModule(PyObject* object = nullptr);
    PythonModule(const PythonObject& other);

    void runCode(QString pythonCode);

    PythonModule createSubModule(QString name);

    PythonModule localImport(QString name, QList<QString> fromList = {}, bool relative = false);

private:
    static PythonModule internalImport(QString name, PythonDict globals, QList<QString> fromList, bool relative = false);
};

}
