#pragma once

#include "medPythonObjectBase.h"
#include "medPythonExceptionManager.h"

namespace med
{

template <class PYTHON_OBJECT>
class MEDPYTHON_EXPORT PythonModuleMixin : public virtual PythonObjectBase
{
public:
    bool isModule()
    {
        return PyModule_Check(getReference());
    }

    bool isPackage()
    {
        return isModule() && hasAttribute("__path__");
    }

    void runCode(QString pythonCode)
    {
        if (!isModule())
        {
            throw PythonTypeError(MEDPYTHON_CODE_LOCATION, QString("'%1' object is not a module").arg(typeName()));
        }

        PYTHON_OBJECT dict = getModuleDict();
        Py_XDECREF(PyRun_String(qUtf8Printable(pythonCode), Py_file_input, dict.getReference(), nullptr));
        PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
    }

    PYTHON_OBJECT createSubModule(QString name)
    {
        if (!isPackage())
        {
            throw PythonTypeError(MEDPYTHON_CODE_LOCATION, QString("'%1' object is not a package").arg(typeName()));
        }

        QString qualifiedName = getModuleName() + "." + name;
        return createModule(qualifiedName);
    }

    PYTHON_OBJECT localImport(QString name, QList<QString> fromList = {}, bool relative = false)
    {
        PYTHON_OBJECT dict = getModuleDict();
        PYTHON_OBJECT importedModule = internalImport(name, dict, fromList, relative);

        if (fromList.isEmpty())
        {
            dict[importedModule.getModuleName()] = importedModule;
        }
        else
        {
            PYTHON_OBJECT otherDict = importedModule.getDict();

            if ((fromList.length() == 1) && fromList[0] == "*")
            {
                retrieveAllModuleObjects(otherDict, dict);
            }
            else
            {
                retrieveModuleObjects(otherDict, dict, fromList);
            }

        }

        return importedModule;
    }

private:
    QString getModuleName()
    {
        QString name = QString(PyModule_GetName(getReference()));
        PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
        return name;
    }

    PYTHON_OBJECT getModuleDict()
    {
        PyObject* dict = PYTHON_OBJECT::borrowed(PyModule_GetDict(getReference()));
        PythonErrorManager::propagateIfErrorOccured(MEDPYTHON_CODE_LOCATION);
        return dict;
    }
};

}
