#pragma once

#include "medPythonCore.h"
#include "medPythonError.h"

namespace med
{
namespace python
{

template <class RETURN_TYPE, auto FUNCTION, class... ARGS>
inline RETURN_TYPE coreFunction(ARGS... args)
{
    auto result = FUNCTION(args...);
    propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);
    return result;
}

template <auto FUNCTION, class... ARGS>
inline void coreFunctionNoReturn(ARGS... args)
{
    FUNCTION(args...);
    propagateErrorIfOccured(MEDPYTHON_CODE_LOCATION);
}

}
}
