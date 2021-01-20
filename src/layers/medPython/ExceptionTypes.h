#pragma once

#include <stdexcept>

#include <QString>

#include "PythonObject.h"

#define PYTHON_EXCEPTION_WRAPPER(name, parent, pythonExceptionType) \
    class name : public parent \
    { \
    public: \
        static const PyObject* pythonType; \
    }; \
    const PyObject* name::pythonType  = pythonExceptionType

namespace medPython
{

class PythonBaseExceptionPrivate;

class PythonBaseException : public std::exception
{
public:
    static const PyObject* pythonType;

    PythonBaseException();
    ~PythonBaseException();

    const char* what() const throw() override;

    void setMessage(QString message);
    void setPythonInstance(PythonObject pythonInstance);

private:
    PythonBaseExceptionPrivate* const d;
};

// BASE EXCEPTIONS
PYTHON_EXCEPTION_WRAPPER(PythonException, PythonBaseException, PyExc_Exception);
PYTHON_EXCEPTION_WRAPPER(GeneratorExit, PythonBaseException, PyExc_GeneratorExit);
PYTHON_EXCEPTION_WRAPPER(KeyboardInterrupt, PythonBaseException, PyExc_KeyboardInterrupt);
PYTHON_EXCEPTION_WRAPPER(SystemExit, PythonBaseException, PyExc_SystemExit);

// ARITHMETIC ERRORS
PYTHON_EXCEPTION_WRAPPER(ArithmeticError, PythonException, PyExc_ArithmeticError);
PYTHON_EXCEPTION_WRAPPER(FloatingPointError, ArithmeticError, PyExc_FloatingPointError);
PYTHON_EXCEPTION_WRAPPER(OverflowError, ArithmeticError, PyExc_OverflowError);
PYTHON_EXCEPTION_WRAPPER(ZeroDivisionError, ArithmeticError, PyExc_ZeroDivisionError);

// IMPORT ERRORS
PYTHON_EXCEPTION_WRAPPER(ImportError, PythonException, PyExc_ImportError);
PYTHON_EXCEPTION_WRAPPER(ModuleNotFoundError, ImportError, PyExc_ModuleNotFoundError);

// LOOKUP ERRORS
PYTHON_EXCEPTION_WRAPPER(LookupError, PythonException, PyExc_LookupError);
PYTHON_EXCEPTION_WRAPPER(IndexError, LookupError, PyExc_IndexError);
PYTHON_EXCEPTION_WRAPPER(KeyError, LookupError, PyExc_KeyError);

// NAME ERRORS
PYTHON_EXCEPTION_WRAPPER(NameError, PythonException, PyExc_NameError);
PYTHON_EXCEPTION_WRAPPER(UnboundLocalError, NameError, PyExc_UnboundLocalError);

// OS ERRORS
PYTHON_EXCEPTION_WRAPPER(OSError, PythonException, PyExc_OSError);
PYTHON_EXCEPTION_WRAPPER(BlockingIOError, OSError, PyExc_BlockingIOError);
PYTHON_EXCEPTION_WRAPPER(ChildProcessError, OSError, PyExc_ChildProcessError);
PYTHON_EXCEPTION_WRAPPER(ConnectionError, OSError, PyExc_ConnectionError);
PYTHON_EXCEPTION_WRAPPER(BrokenPipeError, ConnectionError, PyExc_BrokenPipeError);
PYTHON_EXCEPTION_WRAPPER(ConnectionAbortedError, ConnectionError, PyExc_ConnectionAbortedError);
PYTHON_EXCEPTION_WRAPPER(ConnectionRefusedError, ConnectionError, PyExc_ConnectionRefusedError);
PYTHON_EXCEPTION_WRAPPER(ConnectionResetError, OSError, PyExc_ConnectionResetError);
PYTHON_EXCEPTION_WRAPPER(FileExistsError, OSError, PyExc_FileExistsError);
PYTHON_EXCEPTION_WRAPPER(FileNotFoundError, OSError, PyExc_FileNotFoundError);
PYTHON_EXCEPTION_WRAPPER(InterruptedError, OSError, PyExc_InterruptedError);
PYTHON_EXCEPTION_WRAPPER(IsADirectoryError, OSError, PyExc_IsADirectoryError);
PYTHON_EXCEPTION_WRAPPER(NotADirectoryError, OSError, PyExc_NotADirectoryError);
PYTHON_EXCEPTION_WRAPPER(PermissionError, OSError, PyExc_PermissionError);
PYTHON_EXCEPTION_WRAPPER(ProcessLookupError, OSError, PyExc_ProcessLookupError);
PYTHON_EXCEPTION_WRAPPER(TimeoutError, OSError, PyExc_TimeoutError);

// RUNTIME ERRORS
PYTHON_EXCEPTION_WRAPPER(RuntimeError, PythonException, PyExc_RuntimeError);
PYTHON_EXCEPTION_WRAPPER(NotImplementedError, RuntimeError, PyExc_NotImplementedError);
PYTHON_EXCEPTION_WRAPPER(RecursionError, RuntimeError, PyExc_RecursionError);

// SYNTAX ERRORS
PYTHON_EXCEPTION_WRAPPER(SyntaxError, PythonException, PyExc_SyntaxError);
PYTHON_EXCEPTION_WRAPPER(IndentationError, SyntaxError, PyExc_IndentationError);
PYTHON_EXCEPTION_WRAPPER(TabError, IndentationError, PyExc_TabError);

// VALUE ERRORS
PYTHON_EXCEPTION_WRAPPER(ValueError, PythonException, PyExc_ValueError);
PYTHON_EXCEPTION_WRAPPER(UnicodeError, ValueError, PyExc_UnicodeError);
PYTHON_EXCEPTION_WRAPPER(UnicodeDecodeError, UnicodeError, PyExc_UnicodeDecodeError);
PYTHON_EXCEPTION_WRAPPER(UnicodeEncodeError, UnicodeError, PyExc_UnicodeEncodeError);
PYTHON_EXCEPTION_WRAPPER(UnicodeTranslateError, UnicodeError, PyExc_UnicodeTranslateError);

// OTHER ERRORS
PYTHON_EXCEPTION_WRAPPER(AssertionError, PythonException, PyExc_AssertionError);
PYTHON_EXCEPTION_WRAPPER(AttributeError, PythonException, PyExc_AttributeError);
PYTHON_EXCEPTION_WRAPPER(BufferError, PythonException, PyExc_BufferError);
PYTHON_EXCEPTION_WRAPPER(EOFError, PythonException, PyExc_EOFError);
PYTHON_EXCEPTION_WRAPPER(MemoryError, PythonException, PyExc_MemoryError);
PYTHON_EXCEPTION_WRAPPER(ReferenceError, PythonException, PyExc_ReferenceError);
PYTHON_EXCEPTION_WRAPPER(StopAsyncIteration, PythonException, PyExc_StopAsyncIteration);
PYTHON_EXCEPTION_WRAPPER(StopIteration, PythonException, PyExc_StopIteration);
PYTHON_EXCEPTION_WRAPPER(SystemError, PythonException, PyExc_SystemError);
PYTHON_EXCEPTION_WRAPPER(TypeError, PythonException, PyExc_TypeError);

}
