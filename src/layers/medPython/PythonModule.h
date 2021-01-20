#pragma once

#include <QStringList>

#include "PythonDict.h"
#include "PythonList.h"

namespace medPython
{

int isModuleObject(PyObject* object)
{
    return PyModule_Check(object);
}

class MEDPYTHON_EXPORT PythonModule : public TypeCheckedPythonObject<&isModuleObject>
{
public:
    static PythonModule createModule(QString name);
    static PythonModule createPackage(QString name);

    static PythonModule getModule(QString name);

    static PythonModule import(QString name, PythonList fromList = nullptr);
    static PythonModule importIntoMain(QString name, PythonList fromList = nullptr);

    PythonModule(PyObject* object = nullptr);
    PythonModule(const PythonObject& other);

    bool isPackage();

    QString getName();

    PythonDict getDict();

    void runCode(QString pythonCode);

    PythonModule createSubModule(QString name);

    PythonModule localImport(QString name, PythonList fromList = nullptr, bool relative = false);

private:
    static PythonModule internalImport(QString name, PythonDict globals, PythonList fromList, bool relative = false);
};

}
