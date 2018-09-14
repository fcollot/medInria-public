#pragma once

#include <QHash>
#include <medUtilitiesExport.h>
#include <dtkCore/dtkSmartPointer>
#include <itkImage.h>
#include <medAbstractProcess.h>

class medAbstractData;
class medAbstractView;
class vtkDataArray;

class MEDUTILITIES_EXPORT medUtilities
{
public:

    enum Mode3DType
    {
        VR,
        MIP_MAXIMUM,
        MIP_MINIMUM,
        MSR
    };

    enum DataArrayType
    {
        POINT_ARRAY,
        CELL_ARRAY,
        FIELD_ARRAY
    };

    static void setDerivedMetaData(medAbstractData* derived, medAbstractData* original,
                                   QString derivationDescription, bool queryForDescription = false);

    static void copyMetaDataIfEmpty(medAbstractData* derived, medAbstractData* original, QList<QString> metaDataKeys);
    static void copyMetaDataIfEmpty(medAbstractData* derived, medAbstractData* original, QString metaDataKey);

    static QStringList metaDataKeysToCopyForDerivedData(medAbstractData* derived);

    static void generateSeriesAndSOPInstanceId(medAbstractData* data);

    static void querySeriesDescription(medAbstractData* data);

    static void applyOrientationMatrix(medAbstractView* view, double* inPoint, double* outPoint, bool withTranslation = true);

    static void applyInverseOrientationMatrix(medAbstractView* view, double* inPoint, double* outPoint, bool withTranslation = true);

    static void switchTo3D(medAbstractView *view, Mode3DType mode3D = VR); // Display mesh in 3D orientation

    static medAbstractData *changeMaxNumberOfMeshTriangles(medAbstractData *mesh, int value);

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

    /**
     * @brief Retrieves an array from input data. This functions first looks in
     * point data, then cell data and finally in field data.
     *
     * @param[in] data input data, must be a mesh or map
     * @param[in] arrayName array to retrieve
     * @return specified array if it exits, nullptr otherwise
     */
    static vtkDataArray* getArray(medAbstractData* data,
                                  QString arrayName);

    /**
     * @brief Return array index and data type as optional argument
     * @param[in] data input data, must be a mesh or map
     * @param[in] arrayName array name
     * @param[out] arrayType type of array: point, cell or field array
     * @return data index of array
     */
    static int getArrayIndex(medAbstractData* data,
                             QString arrayName,
                             DataArrayType* arrayType = nullptr);

    /**
     * @brief Retrieve single tuple from a real-valued array.
     *
     * @param[in] data input data, must be a mesh or map
     * @param[in] arrayName input array name
     * @param[in] index tuple index
     * @return tuple as a list. Returns a empty list on failure
     */
    static QList<double> peekArray(medAbstractData* data,
                                   QString arrayName,
                                   int index);
    /**
     * @brief Compute real-valued array range (min and max values).
     *
     * @param[in] data input data, must be a mesh or map
     * @param[in] arrayName input array name
     * @param[out] minRange minimum range
     * @param[out] maxRange maximum range
     * @param[in] component range will be computed on this component.
     *            Pass -1 to compute the L2 norm over all components.
     * @return true if all went well, false otherwise
     */
    static bool arrayRange(medAbstractData* data,
                           QString arrayName,
                           double* minRange,
                           double* maxRange,
                           int component = 0);

    /**
     * @brief Computes mean and standard deviation of an real-valued array.
     * N.B. stats will be computed only on the first component.
     *
     * @param[in] data input data, must be a mesh or map
     * @param[in] arrayName input array name
     * @param[in] component stats will be computed on this component
     * @return list with 2 values: mean and standard deviation (in that order).
     *         Returns an empty list on failure.
     */
    static bool arrayStats(medAbstractData* data,
                           QString arrayName,
                           double* mean,
                           double* stdDev,
                           int component = 0);

    template <typename InstanceClass, typename ImageType, typename... ArgumentTypes>
    static bool callPixelSpecificFunction(int (InstanceClass::*function)(ArgumentTypes...),
                                          InstanceClass* instance, int* result, medAbstractData* data, ArgumentTypes... args)
    {
        if ((function != nullptr) && (data->identifier().startsWith("itkDataImage")))
        {
            ImageType* image = dynamic_cast<ImageType*>(static_cast<itk::ImageBase<ImageType::ImageDimension>*>(data->data()));

            if (image)
            {
                *result = (instance->*function)(args...);
                return true;
            }
        }

        return false;
    }

    template <typename InstanceClass, typename ImageType, typename... ArgumentTypes>
    static bool callPixelSpecificFunction2(int (InstanceClass::*function)(ImageType*, ArgumentTypes...),
                                           InstanceClass* instance, int* result, medAbstractData* data, ArgumentTypes... args)
    {
        ImageType* image = dynamic_cast<ImageType*>(static_cast<itk::ImageBase<ImageType::ImageDimension>*>(data->data()));

        return callPixelSpecificFunction<InstanceClass, ImageType, ImageType*, ArgumentTypes...>(function, instance, result, data, image, args...);
    }

    template <typename InstanceClass, typename... ArgumentTypes>
    static int dispatchOnPixelType(int (InstanceClass::*char3Function)(ArgumentTypes...),
                                   int (InstanceClass::*uchar3Function)(ArgumentTypes...),
                                   int (InstanceClass::*short3Function)(ArgumentTypes...),
                                   int (InstanceClass::*ushort3Function)(ArgumentTypes...),
                                   int (InstanceClass::*int3Function)(ArgumentTypes...),
                                   int (InstanceClass::*uint3Function)(ArgumentTypes...),
                                   int (InstanceClass::*long3Function)(ArgumentTypes...),
                                   int (InstanceClass::*ulong3Function)(ArgumentTypes...),
                                   int (InstanceClass::*float3Function)(ArgumentTypes...),
                                   int (InstanceClass::*double3Function)(ArgumentTypes...),
                                   int (InstanceClass::*char4Function)(ArgumentTypes...),
                                   int (InstanceClass::*uchar4Function)(ArgumentTypes...),
                                   int (InstanceClass::*short4Function)(ArgumentTypes...),
                                   int (InstanceClass::*ushort4Function)(ArgumentTypes...),
                                   int (InstanceClass::*int4Function)(ArgumentTypes...),
                                   int (InstanceClass::*uint4Function)(ArgumentTypes...),
                                   int (InstanceClass::*long4Function)(ArgumentTypes...),
                                   int (InstanceClass::*ulong4Function)(ArgumentTypes...),
                                   int (InstanceClass::*float4Function)(ArgumentTypes...),
                                   int (InstanceClass::*double4Function)(ArgumentTypes...),
                                   InstanceClass* instance,
                                   medAbstractData* data,
                                   ArgumentTypes... args)
    {
        int result;

        if (callPixelSpecificFunction<InstanceClass, itk::Image<char, 3>, ArgumentTypes...>(char3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned char, 3, ArgumentTypes...>(uchar3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, short, 3, ArgumentTypes...>(short3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned short, 3, ArgumentTypes...>(ushort3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, int, 3, ArgumentTypes...>(int3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned int, 3, ArgumentTypes...>(uint3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, long, 3, ArgumentTypes...>(long3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned long, 3, ArgumentTypes...>(ulong3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, float, 3, ArgumentTypes...>(float3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, double, 3, ArgumentTypes...>(double3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, char, 4, ArgumentTypes...>(char4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned char, 4, ArgumentTypes...>(uchar4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, short, 4, ArgumentTypes...>(short4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned short, 4, ArgumentTypes...>(ushort4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, int, 4, ArgumentTypes...>(int4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned int, 4, ArgumentTypes...>(uint4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, long, 4, ArgumentTypes...>(long4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned long, 4, ArgumentTypes...>(ulong4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, float, 4, ArgumentTypes...>(float4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, double, 4, ArgumentTypes...>(double4Function, instance, &result, data, args...))
        {
            return result;
        }
        else
        {
            return medAbstractProcess::PIXEL_TYPE;
        }
    }

    template <typename InstanceClass, typename... ArgumentTypes>
    static int dispatchOnPixelType(int (InstanceClass::*char3Function)(itk::Image<char, 3>*, ArgumentTypes...),
                                   int (InstanceClass::*uchar3Function)(itk::Image<char, 3>*, ArgumentTypes...),
                                   int (InstanceClass::*short3Function)(itk::Image<char, 3>*, ArgumentTypes...),
                                   int (InstanceClass::*ushort3Function)(itk::Image<char, 3>*, ArgumentTypes...),
                                   int (InstanceClass::*int3Function)(itk::Image<char, 3>*, ArgumentTypes...),
                                   int (InstanceClass::*uint3Function)(itk::Image<char, 3>*, ArgumentTypes...),
                                   int (InstanceClass::*long3Function)(itk::Image<char, 3>*, ArgumentTypes...),
                                   int (InstanceClass::*ulong3Function)(itk::Image<char, 3>*, ArgumentTypes...),
                                   int (InstanceClass::*float3Function)(itk::Image<char, 3>*, ArgumentTypes...),
                                   int (InstanceClass::*double3Function)(itk::Image<char, 3>*, ArgumentTypes...),
                                   int (InstanceClass::*char4Function)(itk::Image<char, 4>*, ArgumentTypes...),
                                   int (InstanceClass::*uchar4Function)(itk::Image<char, 4>*, ArgumentTypes...),
                                   int (InstanceClass::*short4Function)(itk::Image<char, 4>*, ArgumentTypes...),
                                   int (InstanceClass::*ushort4Function)(itk::Image<char, 4>*, ArgumentTypes...),
                                   int (InstanceClass::*int4Function)(itk::Image<char, 4>*, ArgumentTypes...),
                                   int (InstanceClass::*uint4Function)(itk::Image<char, 4>*, ArgumentTypes...),
                                   int (InstanceClass::*long4Function)(itk::Image<char, 4>*, ArgumentTypes...),
                                   int (InstanceClass::*ulong4Function)(itk::Image<char, 4>*, ArgumentTypes...),
                                   int (InstanceClass::*float4Function)(itk::Image<char, 4>*, ArgumentTypes...),
                                   int (InstanceClass::*double4Function)(itk::Image<char, 4>*, ArgumentTypes...),
                                   InstanceClass* instance,
                                   medAbstractData* data,
                                   ArgumentTypes... args)
    {
        int result;

        if (callPixelSpecificFunction<InstanceClass, itk::Image<char, 3>, ArgumentTypes...>(char3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned char, 3, ArgumentTypes...>(uchar3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, short, 3, ArgumentTypes...>(short3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned short, 3, ArgumentTypes...>(ushort3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, int, 3, ArgumentTypes...>(int3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned int, 3, ArgumentTypes...>(uint3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, long, 3, ArgumentTypes...>(long3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned long, 3, ArgumentTypes...>(ulong3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, float, 3, ArgumentTypes...>(float3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, double, 3, ArgumentTypes...>(double3Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, char, 4, ArgumentTypes...>(char4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned char, 4, ArgumentTypes...>(uchar4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, short, 4, ArgumentTypes...>(short4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned short, 4, ArgumentTypes...>(ushort4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, int, 4, ArgumentTypes...>(int4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned int, 4, ArgumentTypes...>(uint4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, long, 4, ArgumentTypes...>(long4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, unsigned long, 4, ArgumentTypes...>(ulong4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, float, 4, ArgumentTypes...>(float4Function, instance, &result, data, args...)
            || callPixelSpecificFunction<InstanceClass, double, 4, ArgumentTypes...>(double4Function, instance, &result, data, args...))
        {
            return result;
        }
        else
        {
            return medAbstractProcess::PIXEL_TYPE;
        }
    }

    template <typename ObjectType, typename PixelType, unsigned int dimensions = 3>
    static bool createITKObject(medAbstractData* targetData, itk::LightObject::Pointer* result)
    {
        typedef itk::Image<PixelType, dimensions> ImageType;

        if (dynamic_cast<ImageType*>(static_cast<itk::ImageBase<dimensions>*>(targetData->data())) != nullptr)
        {
            *result = ObjectType::New();
            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename ObjectType, typename PixelType1, typename PixelType2, unsigned int dimensions = 3>
    static bool createITKObject(medAbstractData* targetData1, medAbstractData* targetData2, itk::LightObject::Pointer* result)
    {
        typedef itk::Image<PixelType1, dimensions> ImageType1;
        typedef itk::Image<PixelType2, dimensions> ImageType2;

        if ((dynamic_cast<ImageType1*>(static_cast<itk::ImageBase<dimensions>*>(targetData1->data())) != nullptr)
            && (dynamic_cast<ImageType2*>(static_cast<itk::ImageBase<dimensions>*>(targetData2->data())) != nullptr))
        {
            *result = ObjectType::New();
            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename CharType, typename UCharType, typename ShortType, typename UShortType,
              typename IntType, typename UIntType, typename LongType, typename ULongType,
              typename FloatType, typename DoubleType, unsigned int dimensions = 3>
    static itk::LightObject::Pointer createITKObject(medAbstractData* targetData)
    {
        itk::LightObject::Pointer result;

        if (createITKObject<CharType, char, dimensions>(targetData, &result)
            || createITKObject<UCharType, unsigned char, dimensions>(targetData, &result)
            || createITKObject<ShortType, short, dimensions>(targetData, &result)
            || createITKObject<UShortType, unsigned short, dimensions>(targetData, &result)
            || createITKObject<IntType, int, dimensions>(targetData, &result)
            || createITKObject<UIntType, unsigned int, dimensions>(targetData, &result)
            || createITKObject<LongType, long, dimensions>(targetData, &result)
            || createITKObject<ULongType, unsigned long, dimensions>(targetData, &result)
            || createITKObject<FloatType, float, dimensions>(targetData, &result)
            || createITKObject<DoubleType, double, dimensions>(targetData, &result))
        {
            return result;
        }
        else
        {
            return nullptr;
        }
    }
};

#define DISPATCH_ON_3D_PIXEL_TYPE(function, instance, ...) \
    medUtilities::dispatchOnPixelType( \
    function<itk::Image<char, 3> >, \
    function<unsigned char>, \
    function<short>, \
    function<unsigned short>, \
    function<int>, \
    function<unsigned int>, \
    function<long>, \
    function<unsigned long>, \
    function<float>, \
    function<double>, \
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, \
    (instance), \
    __VA_ARGS__)

#define DISPATCH_ON_4D_PIXEL_TYPE(function, instance, ...) \
    medUtilities::dispatchOnPixelType( \
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, \
    function<itk::Image<char, 4> >, \
    function<unsigned char>, \
    function<short>, \
    function<unsigned short>, \
    function<int>, \
    function<unsigned int>, \
    function<long>, \
    function<unsigned long>, \
    function<float>, \
    function<double>, \
    (instance), \
    __VA_ARGS__)

#define DISPATCH_ON_3D_OR_4D_PIXEL_TYPE(function, instance, ...) \
    medUtilities::dispatchOnPixelType( \
    function<itk::Image<char, 3>, 3>, \
    function<unsigned char, 3>, \
    function<short, 3>, \
    function<unsigned short, 3>, \
    function<int, 3>, \
    function<unsigned int, 3>, \
    function<long, 3>, \
    function<unsigned long, 3>, \
    function<float, 3>, \
    function<double, 3>, \
    function<itk::Image<char, 4>, 4>, \
    function<unsigned char, 4>, \
    function<short, 4>, \
    function<unsigned short, 4>, \
    function<int, 4>, \
    function<unsigned int, 4>, \
    function<long, 4>, \
    function<unsigned long, 4>, \
    function<float, 4>, \
    function<double, 4>, \
    (instance), \
    __VA_ARGS__)

#define CREATE_ITK_OBJECT(className, dimensions, targetData) \
    medUtilities::createITKObject \
    < \
    className<char>, \
    className<unsigned char>, \
    className<short>, \
    className<unsigned short>, \
    className<int>, \
    className<unsigned int>, \
    className<long>, \
    className<unsigned long>, \
    className<float>, \
    className<double>, \
    dimensions \
    > \
    ((targetData))
