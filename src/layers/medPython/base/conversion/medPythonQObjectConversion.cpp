/*==============================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include "medPythonQObjectConversion.h"

#include "medPythonError.h"
#include "medPythonSWIGCore.h"

//bool medPythonConvert(const QObject& qObject, PyObject** output)
//{
//    return true;
//}

//bool medPythonConvert(const PyObject* object, QObject** output)
//{
//    swig_type_info* swigType = SWIG_TypeQuery("QObject");

//    if (!swigType)
//    {
//        med::python::raiseError<med::python::TypeError>("No SWIG type info found for QObject");
//    }

//    void* result;
//    int success = SWIG_Python_ConvertPtr(object, &result, swigType, 0);

//    if (SWIG_IsOK(success))
//    {
//        *output = (QObject*)result;
//    }
//    else
//    {
//        QString message = QString("Cannot convert %1 to QObject").arg(Py_TYPE(object)->tp_name);
//        med::python::raiseError<med::python::TypeError>(message);
//    }

//    return true;
//}
