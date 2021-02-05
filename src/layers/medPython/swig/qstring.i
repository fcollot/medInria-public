%{
#include <medPython.h>
%}

%typemap(typecheck) QString = char *;

%typemap(in) QString
{
    $1 = med::python::Object::borrowed($input).convert<QString>();
}

%typemap(directorout) QString
{
    $result = med::python::Object::borrowed($input).convert<QString>();
}

%typemap(out) QString
{
    $result = PyUnicode_FromString(qUtf8Printable($1));
}

%typemap(directorin) QString
{
    $input = PyUnicode_FromString(qUtf8Printable($1));
}

%apply QString { const QString }


%typemap(in) const QString& (QString temp)
{
    temp = med::python::Object::borrowed($input).convert<QString>();
    $1 = &temp;
}

%typemap(directorout) const QString& (QString temp)
{
    temp = med::python::Object::borrowed($input).convert<QString>();
    $result = &temp;
}

%typemap(out) const QString&
{
    $result = PyUnicode_FromString(qUtf8Printable($1);
}

%typemap(directorin) const QString&
{
    $input = PyUnicode_FromString(qUtf8Printable($1);
}
