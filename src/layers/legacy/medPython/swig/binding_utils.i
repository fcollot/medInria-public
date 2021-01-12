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

%define SET_BINDING_NAME_PREFIX(prefix)

#undef BINDING_NAME_PREFIX
#define BINDING_NAME_PREFIX prefix

%enddef


%define BINDING_NAME(name)

BINDING_NAME_PREFIX ## name

%enddef


%define BINDING_MODULE(name)

SET_BINDING_NAME_PREFIX(name)

%module name

%module(directors="1") name

%feature("director:except")
{
    if ($error != nullptr)
    {
        throw Swig::DirectorMethodException();
    }
}

%enddef


%define ADD_BINDINGS(unprefixed_name)

%rename(unprefixed_name) BINDING_NAME(unprefixed_name)

%{
#include #class ## ".h"
%}

%include #class ## ".h"

%enddef


%define ADD_DIRECTOR_BINDINGS(unprefixed_name)

%feature("director") class;

ADD_BINDINGS(class)

%enddef
