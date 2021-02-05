#pragma once

#include <QScopedPointer>
#include <QString>
#include <QList>

#include "medPythonContainerConversions.h"
#include "medPythonCore.h"
#include "medPythonExport.h"
#include "medPythonFunctionCall.h"
#include "medPythonMappingAccessor.h"
#include "medPythonObjectBase.h"
#include "medPythonSequenceAccessor.h"

namespace med
{
namespace python
{

class MEDPYTHON_EXPORT Object : public ObjectBase<Object>
{
public:
    using ObjectBase::ObjectBase;

    Object(long value);
    Object(double value);
    Object(QString value);

    Object operator[](const Object& key);
    Object operator[](size_t i);

    template <class... ARGS>
    FunctionCall<Object> operator()(ARGS... args);

    template <class... ARGS>
    FunctionCall<Object> callMethod(QString name, ARGS... args);
};

template<class... ARGS>
FunctionCall<Object> Object::operator()(ARGS... args)
{
    Object argsList = Convert<QList<Object> >::toPython({args...});
    return FunctionCall<Object>(**this, *argsList);
}

template<class... ARGS>
FunctionCall<Object> Object::callMethod(QString name, ARGS... args)
{
    return getAttribute(name).operator()(args...);
}

}
}
