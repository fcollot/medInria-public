%{
#include <QString>

QString convertPythonStringToQString(PyObject* pythonString)
{
    PyObject* stringAsPyBytes = nullptr;

    if (PyBytes_Check(pythonString))
    {
        stringAsPyBytes = pythonString;
    }
    else if (PyUnicode_Check(pythonString))
    {
        stringAsPyBytes = PyUnicode_AsEncodedString(pythonString, "UTF-8", "strict");
    }

    if (stringAsPyBytes)
    {
        return strdup(PyBytes_AS_STRING(stringAsPyBytes));
    }
    else
    {
        return QString();
    }
}
%}

%typemap(typecheck) QString = char *;

%typemap(in) QString
{
    $1 = convertPythonStringToQString($input);
}

%typemap(directorout) QString
{
    $result = convertPythonStringToQString($input);
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
    temp = convertPythonStringToQString($input);
    $1 = &temp;
}

%typemap(directorout) const QString& (QString temp)
{
    temp = convertPythonStringToQString($input);
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
