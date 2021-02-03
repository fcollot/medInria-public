#pragma once

#include "medPythonExceptionBase.h"

namespace med
{
namespace python
{

template <class PARENT, PyObject** NATIVE_TYPE>
class MEDPYTHON_EXPORT ExceptionWrapper : public PARENT
{
public:
    static Object nativeType()
    {
        return *NATIVE_TYPE;
    }

    using PARENT::PARENT;
};

// BASE EXCEPTIONS
typedef ExceptionWrapper<ExceptionBase, &PyExc_Exception> Exception;
typedef ExceptionWrapper<ExceptionBase, &PyExc_GeneratorExit> GeneratorExit;
typedef ExceptionWrapper<ExceptionBase, &PyExc_KeyboardInterrupt> KeyboardInterrupt;
typedef ExceptionWrapper<ExceptionBase, &PyExc_SystemExit> SystemExit;

// ARITHMETIC ERRORS
typedef ExceptionWrapper<Exception, &PyExc_ArithmeticError> ArithmeticError;
typedef ExceptionWrapper<ArithmeticError, &PyExc_FloatingPointError> FloatingPointError;
typedef ExceptionWrapper<ArithmeticError, &PyExc_OverflowError> OverflowError;
typedef ExceptionWrapper<ArithmeticError, &PyExc_ZeroDivisionError> ZeroDivisionError;

// IMPORT ERRORS
typedef ExceptionWrapper<Exception, &PyExc_ImportError> ImportError;
typedef ExceptionWrapper<ImportError, &PyExc_ModuleNotFoundError> ModuleNotFoundError;

// LOOKUP ERRORS
typedef ExceptionWrapper<Exception, &PyExc_LookupError> LookupError;
typedef ExceptionWrapper<LookupError, &PyExc_IndexError> IndexError;
typedef ExceptionWrapper<LookupError, &PyExc_KeyError> KeyError;

// NAME ERRORS
typedef ExceptionWrapper<Exception, &PyExc_NameError> NameError;
typedef ExceptionWrapper<NameError, &PyExc_UnboundLocalError> UnboundLocalError;

// OS ERRORS
typedef ExceptionWrapper<Exception, &PyExc_OSError> OSError;
typedef ExceptionWrapper<OSError, &PyExc_BlockingIOError> BlockingIOError;
typedef ExceptionWrapper<OSError, &PyExc_ChildProcessError> ChildProcessError;
typedef ExceptionWrapper<OSError, &PyExc_ConnectionError> ConnectionError;
typedef ExceptionWrapper<ConnectionError, &PyExc_BrokenPipeError> BrokenPipeError;
typedef ExceptionWrapper<ConnectionError, &PyExc_ConnectionAbortedError> ConnectionAbortedError;
typedef ExceptionWrapper<ConnectionError, &PyExc_ConnectionRefusedError> ConnectionRefusedError;
typedef ExceptionWrapper<OSError, &PyExc_ConnectionResetError> ConnectionResetError;
typedef ExceptionWrapper<OSError, &PyExc_FileExistsError> FileExistsError;
typedef ExceptionWrapper<OSError, &PyExc_FileNotFoundError> FileNotFoundError;
typedef ExceptionWrapper<OSError, &PyExc_InterruptedError> InterruptedError;
typedef ExceptionWrapper<OSError, &PyExc_IsADirectoryError> IsADirectoryError;
typedef ExceptionWrapper<OSError, &PyExc_NotADirectoryError> NotADirectoryError;
typedef ExceptionWrapper<OSError, &PyExc_PermissionError> PermissionError;
typedef ExceptionWrapper<OSError, &PyExc_ProcessLookupError> ProcessLookupError;
typedef ExceptionWrapper<OSError, &PyExc_TimeoutError> TimeoutError;

// RUNTIME ERRORS
typedef ExceptionWrapper<Exception, &PyExc_RuntimeError> RuntimeError;
typedef ExceptionWrapper<RuntimeError, &PyExc_NotImplementedError> NotImplementedError;
typedef ExceptionWrapper<RuntimeError, &PyExc_RecursionError> RecursionError;

// SYNTAX ERRORS
typedef ExceptionWrapper<Exception, &PyExc_SyntaxError> SyntaxError;
typedef ExceptionWrapper<SyntaxError, &PyExc_IndentationError> IndentationError;
typedef ExceptionWrapper<IndentationError, &PyExc_TabError> TabError;

// VALUE ERRORS
typedef ExceptionWrapper<Exception, &PyExc_ValueError> ValueError;
typedef ExceptionWrapper<ValueError, &PyExc_UnicodeError> UnicodeError;
typedef ExceptionWrapper<UnicodeError, &PyExc_UnicodeDecodeError> UnicodeDecodeError;
typedef ExceptionWrapper<UnicodeError, &PyExc_UnicodeEncodeError> UnicodeEncodeError;
typedef ExceptionWrapper<UnicodeError, &PyExc_UnicodeTranslateError> UnicodeTranslateError;

// OTHER ERRORS
typedef ExceptionWrapper<Exception, &PyExc_AssertionError> AssertionError;
typedef ExceptionWrapper<Exception, &PyExc_AttributeError> AttributeError;
typedef ExceptionWrapper<Exception, &PyExc_BufferError> BufferError;
typedef ExceptionWrapper<Exception, &PyExc_EOFError> EOFError;
typedef ExceptionWrapper<Exception, &PyExc_MemoryError> MemoryError;
typedef ExceptionWrapper<Exception, &PyExc_ReferenceError> ReferenceError;
typedef ExceptionWrapper<Exception, &PyExc_StopAsyncIteration> StopAsyncIteration;
typedef ExceptionWrapper<Exception, &PyExc_StopIteration> StopIteration;
typedef ExceptionWrapper<Exception, &PyExc_SystemError> SystemError;
typedef ExceptionWrapper<Exception, &PyExc_TypeError> TypeError;

}
}
