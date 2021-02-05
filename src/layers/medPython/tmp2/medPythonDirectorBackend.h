#pragma once

#include "QObject"

#include "medPythonObjectBase.h"

namespace
{

void* internalConvert(PyObject* object, QString cppTypeName);

}

namespace med
{

template <class PYTHON_OBJECT>
class MEDPYTHON_EXPORT PythonObjectDirectors : public virtual PythonObjectBase
{
public:
    template<class CPP_TYPE>
    CPP_TYPE* directorConvert()
    {
        QString cppTypeName = CPP_TYPE::staticMetaObject.className();
        return static_cast<CPP_TYPE*>((QObject*)internalConvert(getReference(), cppTypeName));
    }
};

}
