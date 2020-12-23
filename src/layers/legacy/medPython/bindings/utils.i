%module utils

%ignore operator=;
%ignore operator>>;
%ignore operator<<;
%ignore operator==;
%ignore operator[];
%ignore operator!=;
%ignore operator*=;
%ignore operator/=;
%ignore operator bool;
%ignore operator int;
%ignore operator float;
%ignore operator double;
%ignore operator double*;

#undef Q_OBJECT
#define Q_OBJECT

#undef Q_INVOKABLE
#define Q_INVOKABLE

#undef signals
#define signals public

#undef slots
#define slots

#undef  Q_PROPERTY(Type type MODE mode)
#define Q_PROPERTY(Type type MODE mode)

#undef  Q_DECLARE_INTERFACE(IFace, IId)
#define Q_DECLARE_INTERFACE(IFace, IId)

#undef  Q_DECLARE_METATYPE(Type type)
#define Q_DECLARE_METATYPE(Type type)

#undef  DTKCORESUPPORT_EXPORT
#define DTKCORESUPPORT_EXPORT

#undef  DTK_DECLARE_PRIVATE(type)
#define DTK_DECLARE_PRIVATE(type)

#undef  DTK_IMPLEMENT_PRIVATE(type, parent)
#define DTK_IMPLEMENT_PRIVATE(type, parent)

#undef  MEDCORELEGACY_EXPORT
#define MEDCORELEGACY_EXPORT

#undef  MEDPYTHON_EXPORT
#define MEDPYTHON_EXPORT

%{
#include "medPythonUtils.h"
%}

%include "medPythonUtils.h"
