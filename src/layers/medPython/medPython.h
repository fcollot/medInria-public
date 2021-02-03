#pragma once

#include <QHash>
#include <QList>

#include "medPythonExport.h"
#include "medPythonObject.h"
#include "medPythonSystem.h"

namespace med
{
namespace python
{

Object import(QString name, QList<QString> fromList = {});

Object createModule(QString name);
Object createPackage(QString name);

Object getModule(QString name);

Object tuple(QList<Object> items = {});
Object tuple(QList<QString> items = {});
Object tuple(QList<long> items = {});
Object tuple(QList<double> items = {});
Object tuple(Object& object);

Object list(QList<Object> items = {});
Object list(QList<QString> items = {});
Object list(QList<long> items = {});
Object list(QList<double> items = {});
Object list(Object& object);

Object dict(QHash<Object, Object> items = QHash<Object, Object>());

Object none();

static size_t len(Object& object);

static QList<QString> dir(Object& object);

Object take(PyObject* nativeObject);
Object borrow(PyObject* nativeObject);
};

}
}
