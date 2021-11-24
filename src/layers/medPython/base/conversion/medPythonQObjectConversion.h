#pragma once
/*==============================================================================

 medInria

 Copyright (c) INRIA 2021. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include <type_traits>

#include <QObject>

#include "medPythonCore.h"
#include "medPythonError.h"
#include "medPythonExport.h"

MEDPYTHON_EXPORT bool medPythonConvert(const QObject* object, PyObject** output);
MEDPYTHON_EXPORT bool medPythonConvert(const PyObject* nativeObject, QObject** output);

//template <class TYPE, typename = typename std::enable_if<std::is_base_of<QObject, TYPE>::value>::type>
//bool medPythonConvert(const PyObject* nativeObject, TYPE** output)
//{
//    QObject* tempOutput;
//    bool success = medPythonConvert(nativeObject, &tempOutput);

//    if (success && tempOutput)
//    {
//        *output = dynamic_cast<TYPE*>(tempOutput);

//        if (!output)
//        {
//            QString message = QString("Object is not of type %1").arg(typeid(TYPE).name());
//            med::python::raiseError<med::python::TypeError>(message);
//            success = false;
//        }
//    }

//    return success;
//}

#include <medAbstractData.h>

//template bool medPythonConvert<medAbstractData, true>(const PyObject* nativeObject, medAbstractData** output);
