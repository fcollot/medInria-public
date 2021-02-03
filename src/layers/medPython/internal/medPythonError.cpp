#include "medPythonError.h"

#include <QHash>

#include "medPythonExceptions.h"

namespace med
{
namespace python
{
namespace internal
{

namespace
{

typedef void (*PropagationFunction)(SourceCodeLocation&, PyObject*);

QHash<PyObject*, PropagationFunction> propagationFunctions;

template <class EXCEPTION_TYPE>
void initializeErrorPropagation()
{
    PropagationFunction propagationFunction = [](SourceCodeLocation& propagationSite, const Reference& nativeException)
    {
        throw EXCEPTION_TYPE(propagationSite, nativeException);
    };

    PyObject* nativeType = *EXCEPTION_TYPE::nativeType();
    propagationFunctions[nativeType] = propagationFunction;
}

}

void initializeErrorPropagation()
{
    if (propagationFunctions.isEmpty())
    {
        initializeErrorPropagation<ExceptionBase>();
        initializeErrorPropagation<Exception>();
        initializeErrorPropagation<GeneratorExit>();
        initializeErrorPropagation<KeyboardInterrupt>();
        initializeErrorPropagation<SystemExit>();
        initializeErrorPropagation<ArithmeticError>();
        initializeErrorPropagation<FloatingPointError>();
        initializeErrorPropagation<OverflowError>();
        initializeErrorPropagation<ZeroDivisionError>();
        initializeErrorPropagation<ImportError>();
        initializeErrorPropagation<ModuleNotFoundError>();
        initializeErrorPropagation<LookupError>();
        initializeErrorPropagation<IndexError>();
        initializeErrorPropagation<KeyError>();
        initializeErrorPropagation<NameError>();
        initializeErrorPropagation<UnboundLocalError>();
        initializeErrorPropagation<OSError>();
        initializeErrorPropagation<BlockingIOError>();
        initializeErrorPropagation<ChildProcessError>();
        initializeErrorPropagation<ConnectionError>();
        initializeErrorPropagation<BrokenPipeError>();
        initializeErrorPropagation<ConnectionAbortedError>();
        initializeErrorPropagation<ConnectionRefusedError>();
        initializeErrorPropagation<ConnectionResetError>();
        initializeErrorPropagation<FileExistsError>();
        initializeErrorPropagation<FileNotFoundError>();
        initializeErrorPropagation<InterruptedError>();
        initializeErrorPropagation<IsADirectoryError>();
        initializeErrorPropagation<NotADirectoryError>();
        initializeErrorPropagation<PermissionError>();
        initializeErrorPropagation<TimeoutError>();
        initializeErrorPropagation<RuntimeError>();
        initializeErrorPropagation<NotImplementedError>();
        initializeErrorPropagation<RecursionError>();
        initializeErrorPropagation<SyntaxError>();
        initializeErrorPropagation<IndentationError>();
        initializeErrorPropagation<TabError>();
        initializeErrorPropagation<ValueError>();
        initializeErrorPropagation<UnicodeError>();
        initializeErrorPropagation<UnicodeDecodeError>();
        initializeErrorPropagation<UnicodeEncodeError>();
        initializeErrorPropagation<UnicodeTranslateError>();
        initializeErrorPropagation<AssertionError>();
        initializeErrorPropagation<AttributeError>();
        initializeErrorPropagation<BufferError>();
        initializeErrorPropagation<EOFError>();
        initializeErrorPropagation<MemoryError>();
        initializeErrorPropagation<ReferenceError>();
        initializeErrorPropagation<StopAsyncIteration>();
        initializeErrorPropagation<StopIteration>();
        initializeErrorPropagation<SystemError>();
        initializeErrorPropagation<TypeError>();
    }
}

bool errorOccured()
{
    return PyErr_Occurred();
}

void clearError()
{
    PyErr_Clear();
}

Reference retrieveException()
{
    PyObject* exceptionType;
    PyObject* exceptionInstance;
    PyObject* traceback;

    PyErr_Fetch(&exceptionType, &exceptionInstance, &traceback);
    PyErr_NormalizeException(&exceptionType, &exceptionInstance, &traceback);

    if (traceback)
    {
        PyException_SetTraceback(exceptionInstance, traceback);
    }

    Py_XDECREF(exceptionType);
    Py_XDECREF(traceback);

    return exceptionInstance;
}

void propagateErrorIfOccured(const SourceCodeLocation& propagationSite)
{
    if (errorOccured())
    {
        Reference nativeException = retrieveException();
        clearError();
        propagateError(propagationSite, nativeException);
    }
}

void propagateError(const SourceCodeLocation& propagationSite, const Reference& nativeException)
{
    Reference typeLineage = PyObject_GetAttrString(nativeException, "__mro__"); // method resolution order

    if (!PyErr_Occurred() && PyTuple_Check(typeLineage))
    {
        size_t lineageSize = PyTuple_GET_SIZE(typeLineage);

        for (int i = 0; i < lineageSize; i++)
        {
            PyObject* type = PyTuple_GET_ITEM(typeLineage, i);

            if (propagationFunctions.contains(type))
            {
                propagationFunctions[type](propagationSite, nativeException);
            }
        }
    }

    // if we've reached this point then we did not find a propagation function, so throw it as a base exception
    throw ExceptionBase(propagationSite, nativeException);
}

}
}
}
