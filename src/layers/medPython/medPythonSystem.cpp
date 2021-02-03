#include "medPythonSystem.h"

#include <QApplication>

#include "medPythonCore.h"
#include "medPythonError.h"

namespace med
{
namespace python
{

bool initialize()
{
    if (!Py_IsInitialized())
    {
        Py_Initialize();
        initializeErrorPropagation();
        QApplication::connect(qApp, &QApplication::aboutToQuit, &finalize);
    }
}

void finalize()
{
    if (Py_IsInitialized())
    {
        Py_Finalize();
    }
}

}
}
