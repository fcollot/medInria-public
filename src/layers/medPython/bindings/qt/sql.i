%{
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlField>
#include <QSqlIndex>
#include <QSqlQuery>
#include <QSqlRecord>
%}

%include "qsqlerror.h"
%include "qsqlfield.h"
%include "qsqlrecord.h"
%include "qsqlindex.h"
%include "qsqlquery.h"
%include "qsqldatabase.h"

%extend QSqlDatabase
{

    %pythoncode
    %{

        def exec(self, query):
           return self._exec(query)

    %}

}
