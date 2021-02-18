/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonModule.h"

#include "medPythonCoreWrappers.h"
#include "medPythonErrors.h"

namespace med::python
{

namespace
{

QString packageNameToResourcePath(QString name)
{
    return name.prepend(":/python/").replace('.', "/");
}

QString moduleNameToResourcePath(QString name)
{
    return packageNameToResourcePath(name).append(".py");
}

} // namespace

Module Module::get(QString name)
{
    Object pythonName = name;
    return coreFunction<PyObject*, &PyImport_GetModule, PyObject*>(*pythonName);
}

Module Module::main()
{
    return get("__main__");
}

Object Module::create(QString name, bool isPackage)
{
    if (!get(name))
    {
        throw RuntimeError(QString("Module %1 already exists").arg(name));
    }

    PyObject* moduleObject = coreFunction<PyObject*, &PyImport_AddModule, const char*>(qUtf8Printable(name));
    Module module = Object::borrowed(moduleObject);

    if (isPackage)
    {
        Object resourcePath = packageNameToResourcePath(name);
        Object libraryPath = qEnvironmentVariable("MEDINRIA_LIBRARY_DIR");
        Object path = list<Object>({resourcePath, libraryPath});
        package.setAttribute("__path__", path);
        return package;
    }

    return module;
}

} // namespace med::python

