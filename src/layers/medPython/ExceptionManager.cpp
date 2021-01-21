#include "ExceptionManager.h"

namespace medPython
{

namespace
{

typedef void (*CastAndThrowFunction)(PythonExceptionBase);

static const char* exceptionIdAttributeName = "cpp_exception_id";
static QList<CastAndThrowFunction> exceptionCastAndThrowFunctions;

}

void ExceptionManager::initialize()
{
    initializeExceptionPropagation();
}

bool ExceptionManager::exceptionOccured()
{
    return PyErr_Occurred();
}

void ExceptionManager::clear()
{
    PyErr_Clear();
}

void ExceptionManager::propagateCurrentException(SourceCodeLocation codeLocation)
{
    PythonExceptionBase uncastedException(codeLocation, retrieveCurrentException());
    clear();
    castToProperExceptionAndThrow(uncastedException);
}

PyObject* ExceptionManager::retrieveCurrentException()
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

void ExceptionManager::castToProperExceptionAndThrow(PythonExceptionBase& exception)
{
    // During initalization we set an id attribute on each standard python exception class.
    // The actual python exception may be a subclass instance, so in order to find the id
    // we need to go up the class hierarchy.

    PythonTuple typeLineage = exception.getAttribute("__mro__"); // method resolution order

    for (int i = 0; i < typeLineage.getSize(); i++)
    {
        PythonType type = typeLineage.getItem(i);

        if (type.hasAttribute(exceptionIdAttributeName))
        {
            int exceptionId = exception.getAttribute(exceptionIdAttributeName).toInteger();
            exceptionCastAndThrowFunctions[exceptionId](exception);
        }
    }

    // if we've reached this point then the cast failed, but we throw the exception anyways
    throw exception;
}

void ExceptionManager::initializeExceptionPropagation()
{
    initializeExceptionPropagation<PythonExceptionBase>();
    initializeExceptionPropagation<PythonException>();
    initializeExceptionPropagation<GeneratorExit>();
    initializeExceptionPropagation<KeyboardInterrupt>();
    initializeExceptionPropagation<SystemExit>();
    initializeExceptionPropagation<ArithmeticError>();
    initializeExceptionPropagation<FloatingPointError>();
    initializeExceptionPropagation<OverflowError>();
    initializeExceptionPropagation<ZeroDivisionError>();
    initializeExceptionPropagation<ImportError>();
    initializeExceptionPropagation<ModuleNotFoundError>();
    initializeExceptionPropagation<LookupError>();
    initializeExceptionPropagation<IndexError>();
    initializeExceptionPropagation<KeyError>();
    initializeExceptionPropagation<NameError>();
    initializeExceptionPropagation<UnboundLocalError>();
    initializeExceptionPropagation<OSError>();
    initializeExceptionPropagation<BlockingIOError>();
    initializeExceptionPropagation<ChildProcessError>();
    initializeExceptionPropagation<ConnectionError>();
    initializeExceptionPropagation<BrokenPipeError>();
    initializeExceptionPropagation<ConnectionAbortedError>();
    initializeExceptionPropagation<ConnectionRefusedError>();
    initializeExceptionPropagation<ConnectionResetError>();
    initializeExceptionPropagation<FileExistsError>();
    initializeExceptionPropagation<FileNotFoundError>();
    initializeExceptionPropagation<InterruptedError>();
    initializeExceptionPropagation<IsADirectoryError>();
    initializeExceptionPropagation<NotADirectoryError>();
    initializeExceptionPropagation<PermissionError>();
    initializeExceptionPropagation<TimeoutError>();
    initializeExceptionPropagation<RuntimeError>();
    initializeExceptionPropagation<NotImplementedError>();
    initializeExceptionPropagation<RecursionError>();
    initializeExceptionPropagation<SyntaxError>();
    initializeExceptionPropagation<IndentationError>();
    initializeExceptionPropagation<TabError>();
    initializeExceptionPropagation<ValueError>();
    initializeExceptionPropagation<UnicodeError>();
    initializeExceptionPropagation<UnicodeDecodeError>();
    initializeExceptionPropagation<UnicodeEncodeError>();
    initializeExceptionPropagation<UnicodeTranslateError>();
    initializeExceptionPropagation<AssertionError>();
    initializeExceptionPropagation<AttributeError>();
    initializeExceptionPropagation<BufferError>();
    initializeExceptionPropagation<EOFError>();
    initializeExceptionPropagation<MemoryError>();
    initializeExceptionPropagation<ReferenceError>();
    initializeExceptionPropagation<StopAsyncIteration>();
    initializeExceptionPropagation<StopIteration>();
    initializeExceptionPropagation<SystemError>();
    initializeExceptionPropagation<TypeError>();
}

template <class EXCEPTION_TYPE>
void ExceptionManager::initializeExceptionPropagation()
{
    PythonObject exceptionId = exceptionCastAndThrowFunctions.length();

    exceptionCastAndThrowFunctions.append([](PythonExceptionBase uncastedException)
    {
        throw EXCEPTION_TYPE(uncastedException);
    });

    PythonObject(EXCEPTION_TYPE::nativeType).setAttribute(exceptionIdAttributeName, exceptionId);
}

}
