/*=========================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medPythonInit.h"

#include <QApplication>
#include <QDebug>

#include "medPythonCore.h"

namespace med::python
{

void initialize()
{
    if (!Py_IsInitialized())
    {
//        auto path = Py_DecodeLocale("/Users/florent.collot/Work/MUSICardio/medInria-public/build/python/dist/foo/foo", nullptr);
//        Py_SetProgramName(path);
        Py_Initialize();
        QApplication::connect(qApp, &QApplication::aboutToQuit, &finalize);
        qInfo() << "Python initialized: " << Py_GetVersion();
    }
}

void finalize()
{
    if (Py_IsInitialized())
    {
        Py_Finalize();
    }
}

} // namespace med::python
