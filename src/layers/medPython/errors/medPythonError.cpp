#include "medPythonError.h"

#include <QHash>

#include "medPythonCore.h"
#include "medPythonExceptions.h"
#include "medPythonObject.h"

namespace med
{
namespace python
{

namespace
{

PyObject* tracebackModule;

typedef void (*PropagationFunction)(const SourceCodeLocation&, PyObject*);

QHash<const PyObject*, PropagationFunction> propagationFunctions;

template <class EXCEPTION_TYPE>
void initializeErrorPropagation()
{
    PropagationFunction propagationFunction = [](const SourceCodeLocation& propagationSite, PyObject* nativeException)
    {
        throw EXCEPTION_TYPE(propagationSite, nativeException);
    };

    propagationFunctions[EXCEPTION_TYPE::nativeType] = propagationFunction;
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

}

void internal::initializeErrorHandling()
{
    PyObject* tracebackString = utils::fromString("traceback");
    tracebackModule = PyImport_Import(tracebackString);
    initialiazeExceptionClasses();
    initializeErrorPropagation();
}

void internal::finalizeErrorHandling()
{
    utils::decref(tracebackModule);
    propagationFunctions.clear();
}

bool errorOccured()
{
    return PyErr_Occurred();
}

void clearError()
{
    PyErr_Clear();
}

PyObject* retrieveException()
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

    Py_CLEAR(exceptionType);
    Py_CLEAR(traceback);

    return exceptionInstance;
}

void propagateErrorIfOccured(const SourceCodeLocation& propagationSite)
{
    if (errorOccured())
    {
        propagateCurrentError(propagationSite);
    }
}

void propagateCurrentError(const SourceCodeLocation& propagationSite)
{
    PyObject* nativeException = retrieveException();
    clearError();
    propagateError(propagationSite, nativeException);
}

void propagateError(const SourceCodeLocation& propagationSite, PyObject* nativeException)
{
    Object exception = nativeException;
    Object typeLineage = PyObject_GetAttrString(*exception, "__mro__"); // method resolution order

    if (!PyErr_Occurred() && PyTuple_Check(*typeLineage))
    {
        ssize_t lineageSize = PyTuple_GET_SIZE(*typeLineage);

        for (int i = 0; i < lineageSize; i++)
        {
            PyObject* type = PyTuple_GET_ITEM(*typeLineage, i);

            if (propagationFunctions.contains(type))
            {
                propagationFunctions[type](propagationSite, exception.newReference());
            }
        }
    }

    // if we've reached this point then we failed to find a propagation function, so we throw it as a base exception
    throw ExceptionBase(propagationSite, *exception);
}

PyObject* getTracebackModule()
{
    return tracebackModule;
}

}
}
