#pragma once

#include <QString>

typedef struct _object PyObject;

namespace med
{
namespace python
{
namespace utils
{

void incref(PyObject* object);
void decref(PyObject* object);

PyObject* createList(ssize_t length);
PyObject* createDict();

PyObject* getItem(PyObject* sequence, ssize_t index);
void setItem(PyObject* sequence, ssize_t index, PyObject* value);

PyObject* getItem(PyObject* mapping, PyObject* key);
void setItem(PyObject* mapping, PyObject* key, PyObject* value);

ssize_t length(PyObject* container);

PyObject* keys(PyObject* mapping);
PyObject* values(PyObject* mapping);

PyObject* call(PyObject* callable, PyObject* args = nullptr, PyObject* kwargs = nullptr);
PyObject* callMethod(PyObject* object, QString method, PyObject* args = nullptr, PyObject* kwargs = nullptr);

QString toString(PyObject* object);
PyObject* fromString(QString string);

}
}
}
