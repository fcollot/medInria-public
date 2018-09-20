#pragma once

#include <dtkCore/dtkSmartPointer>
#include <itkImage.h>
#include <medAbstractProcess.h>
#include <medUtilities.h>
#include <medUtilitiesExport.h>

class medAbstractData;
class medAbstractView;
class vtkDataArray;

class MEDUTILITIES_EXPORT medUtilitiesITK
{
public:

    /**
     * @brief minimumValue computes the minimum pixel intensity in a volume
     * @param the input volume
     * @return the minimum intensity value (double)
     */
    static double minimumValue(dtkSmartPointer<medAbstractData> data);

    /**
     * @brief maximumValue computes the maximum pixel intensity in a volume
     * @param the input volume
     * @return the maximum intensity value (double)
     */
    static double maximumValue(dtkSmartPointer<medAbstractData> data);

    static double volume(dtkSmartPointer<medAbstractData> data);

    typedef itk::Image<char, 3> itkImageChar3;
    typedef itk::Image<unsigned char, 3> itkImageUChar3;
    typedef itk::Image<short, 3> itkImageShort3;
    typedef itk::Image<unsigned short, 3> itkImageUShort3;
    typedef itk::Image<int, 3> itkImageInt3;
    typedef itk::Image<unsigned int, 3> itkImageUInt3;
    typedef itk::Image<long, 3> itkImageLong3;
    typedef itk::Image<unsigned long, 3> itkImageULong3;
    typedef itk::Image<float, 3> itkImageFloat3;
    typedef itk::Image<double, 3> itkImageDouble3;
    typedef itk::Image<char, 4> itkImageChar4;
    typedef itk::Image<unsigned char, 4> itkImageUChar4;
    typedef itk::Image<short, 4> itkImageShort4;
    typedef itk::Image<unsigned short, 4> itkImageUShort4;
    typedef itk::Image<int, 4> itkImageInt4;
    typedef itk::Image<unsigned int, 4> itkImageUInt4;
    typedef itk::Image<long, 4> itkImageLong4;
    typedef itk::Image<unsigned long, 4> itkImageULong4;
    typedef itk::Image<float, 4> itkImageFloat4;
    typedef itk::Image<double, 4> itkImageDouble4;

    /** Identifier of the class used to store a specific itk image type, to be used with dtkAbstractDataFactory
     */
    template <typename ImageType> static QString itkDataImageId();

    template <typename TargetClass, typename... OtherArguments>
    static int dispatchOn3DPixelType(int (TargetClass::*char3Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*uchar3Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*short3Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*ushort3Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*int3Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*uint3Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*long3Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*ulong3Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*float3Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*double3Function)(medAbstractData*, OtherArguments...),
                                     TargetClass* targetInstance,
                                     medAbstractData* inputData,
                                     OtherArguments... otherArgs)
    {
        int result;

        if (callPixelSpecificFunction<TargetClass, itkImageChar3, OtherArguments...>(char3Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageUChar3, OtherArguments...>(uchar3Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageShort3, OtherArguments...>(short3Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageUShort3, OtherArguments...>(ushort3Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageInt3, OtherArguments...>(int3Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageUInt3, OtherArguments...>(uint3Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageLong3, OtherArguments...>(long3Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageULong3, OtherArguments...>(ulong3Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageFloat3, OtherArguments...>(float3Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageDouble3, OtherArguments...>(double3Function, targetInstance, &result, inputData, otherArgs...))
        {
            return result;
        }
        else
        {
            return medAbstractProcess::PIXEL_TYPE;
        }
    }

    template <typename TargetClass, typename... OtherArguments>
    static int dispatchOn4DPixelType(int (TargetClass::*char4Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*uchar4Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*short4Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*ushort4Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*int4Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*uint4Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*long4Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*ulong4Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*float4Function)(medAbstractData*, OtherArguments...),
                                     int (TargetClass::*double4Function)(medAbstractData*, OtherArguments...),
                                     TargetClass* targetInstance,
                                     medAbstractData* inputData,
                                     OtherArguments... otherArgs)
    {
        int result;

        if (callPixelSpecificFunction<TargetClass, itkImageChar4, OtherArguments...>(char4Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageUChar4, OtherArguments...>(uchar4Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageShort4, OtherArguments...>(short4Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageUShort4, OtherArguments...>(ushort4Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageInt4, OtherArguments...>(int4Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageUInt4, OtherArguments...>(uint4Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageLong4, OtherArguments...>(long4Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageULong4, OtherArguments...>(ulong4Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageFloat4, OtherArguments...>(float4Function, targetInstance, &result, inputData, otherArgs...)
            || callPixelSpecificFunction<TargetClass, itkImageDouble4, OtherArguments...>(double4Function, targetInstance, &result, inputData, otherArgs...))
        {
            return result;
        }
        else
        {
            return medAbstractProcess::PIXEL_TYPE;
        }
    }

private:

    template <typename TargetClass, typename ImageType, typename... OtherArguments>
    static bool callPixelSpecificFunction(int (TargetClass::*function)(medAbstractData*, OtherArguments...),
                                          TargetClass* targetInstance, int* result, medAbstractData* inputData, OtherArguments... otherArgs)
    {
        if ((function != nullptr) && (inputData->identifier().startsWith("itkDataImage")))
        {
            ImageType* image = dynamic_cast<ImageType*>(static_cast<itk::ImageBase<ImageType::ImageDimension>*>(inputData->data()));

            if (image)
            {
                *result = (targetInstance->*function)(inputData, otherArgs...);
                return true;
            }
        }

        return false;
    }
};


/** Call a function templated over the pixel type of an ITK image
 *  This macro will only check for 3D pixel types. The function signature must be of the form
 *  @code
 *  template<ImageType> int function(medAbstractData*, ...)
 *  @endcode
 *  where ImageType is the ITK type, the first argument is the data containing the image and ... may be zero or more arguments of any type.
 *  @param function a pointer to a member function (see signature above)
 *  @param instance the instance on which to invoke the function
 *  @param ... the arguments to the function
 *  @return the return value of the function or medAbstractProcess::PIXEL_TYPE if the pixel type was not handled
 */
#define DISPATCH_ON_3D_PIXEL_TYPE(function, instance, ...) \
    medUtilitiesITK::dispatchOn3DPixelType( \
    function<medUtilitiesITK::itkImageChar3>, \
    function<medUtilitiesITK::itkImageUChar3>, \
    function<medUtilitiesITK::itkImageShort3>, \
    function<medUtilitiesITK::itkImageUShort3>, \
    function<medUtilitiesITK::itkImageInt3>, \
    function<medUtilitiesITK::itkImageUInt3>, \
    function<medUtilitiesITK::itkImageLong3>, \
    function<medUtilitiesITK::itkImageULong3>, \
    function<medUtilitiesITK::itkImageFloat3>, \
    function<medUtilitiesITK::itkImageDouble3>, \
    (instance), \
    __VA_ARGS__)


/** Call a function templated over the pixel type of an ITK image
 *  This macro will only check for 4D pixel types. The function signature must be of the form
 *  @code
 *  template<ImageType> int function(medAbstractData*, ...)
 *  @endcode
 *  where ImageType is the ITK type, the first argument is the data containing the image and ... may be zero or more arguments of any type.
 *  @param function a pointer to a member function (see signature above)
 *  @param instance the instance on which to invoke the function
 *  @param ... the arguments to the function
 *  @return the return value of the function or medAbstractProcess::PIXEL_TYPE if the pixel type was not handled
 */
#define DISPATCH_ON_4D_PIXEL_TYPE(function, instance, ...) \
    medUtilitiesITK::dispatchOn4DPixelType( \
    function<medUtilitiesITK::itkImageChar4>, \
    function<medUtilitiesITK::itkImageUChar4>, \
    function<medUtilitiesITK::itkImageShort4>, \
    function<medUtilitiesITK::itkImageUShort4>, \
    function<medUtilitiesITK::itkImageInt4>, \
    function<medUtilitiesITK::itkImageUInt4>, \
    function<medUtilitiesITK::itkImageLong4>, \
    function<medUtilitiesITK::itkImageULong4>, \
    function<medUtilitiesITK::itkImageFloat4>, \
    function<medUtilitiesITK::itkImageDouble4>, \
    (instance), \
    __VA_ARGS__)
