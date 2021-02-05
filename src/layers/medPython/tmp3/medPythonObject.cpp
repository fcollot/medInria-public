#include "medPythonObject.h"

#include "medPythonConditionFunctions.h"
#include "medPythonPrimitiveConversions.h"

namespace med
{
namespace python
{

//Object Object::take(PyObject* nativeObject)
//{
//    return nativeObject;
//}

//Object Object::borrow(PyObject* nativeObject)
//{
//    Py_XINCREF(nativeObject);
//    return nativeObject;
//}


Object::~Object()
{
    // If this object is a function call reference, we must handle the case of an isolated function call (one whose
    // return value is not used, which means the evaluation of the call was not triggered).
    FunctionCallReference* functionCall = dynamic_cast<FunctionCallReference*>(reference.data());

    if (functionCall)
    {
        try
        {
            functionCall->ensureEvaluated();
        }
        catch ()
        {
            reference.reset();

            // Exceptions should never be thrown from destructors unless we are certain there is no stack-unwinding
            // process taking place for another exception (and this is the case for function call references)
            throw;
        }
    }
}

Object::Object(long value)
{
    *reference = internal::Convert<long>::toPython(value);
}

Object::Object(double value)
{
    *reference = internal::Convert<double>::toPython(value);
}

Object::Object(QString value)
{
    *reference = internal::Convert<QString>::toPython(value);
}

Object Object::operator[](const Object& key)
{
    return new MappingItemReference(newReference(), key.newReference());
}

Object Object::operator[](size_t i)
{
    return new SequenceItemReference(newReference(), i);
}

Object::Object(const internal::AbstractReference& reference) :
    reference(reference)
{

}

}
}
