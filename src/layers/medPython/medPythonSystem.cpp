#include "medPythonSystem.h"

#include <QApplication>

#include "medPythonCore.h"
#include "medPythonExceptions.h"

namespace med
{
namespace python
{

void initialize()
{
    if (!Py_IsInitialized())
    {
        Py_Initialize();
        internal::initializeErrorHandling();
        QApplication::connect(qApp, &QApplication::aboutToQuit, &finalize);
    }
}

void finalize()
{
    if (Py_IsInitialized())
    {
        internal::finalizeErrorHandling();
        Py_Finalize();
    }
}

}
}
