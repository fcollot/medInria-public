#pragma once

#include <QDir>

#include "medPythonExport.h"
#include "medPythonUtils.h"

class medPythonModuleHandlePrivate;

class MEDPYTHON_EXPORT medPythonModuleHandle
{
public:
    medPythonModuleHandle(PyObject* module = nullptr);
    medPythonModuleHandle(const medPythonModuleHandle& other);
    medPythonModuleHandle& operator=(const medPythonModuleHandle& other);
    ~medPythonModuleHandle();

    bool isValid();

    QString getName();

    PyObject* getModuleObject();

    void runString(QString sourceCode);

    medPythonModuleHandle import(QString name, QStringList fromList = QStringList(), bool relative = false);

    void importObjectsFrom(medPythonModuleHandle& other, QStringList objectNames = QStringList());

private:
    medPythonModuleHandlePrivate* const d;
};
