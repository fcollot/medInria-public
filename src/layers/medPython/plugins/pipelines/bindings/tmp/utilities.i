#undef MEDUTILITIES_EXPORT
#define MEDUTILITIES_EXPORT

%{
#include "medUtilities.h"
#include "medUtilitiesITK.h"
#include "medUtilitiesVTK.h"
%}

/* These rules allow SWIG to replace C++ output arguments by multiple return values in Python.
 * (Python does not support output arguments)
 * To avoid side effects the rules are cleared after use.
 */
%apply double* OUTPUT {double* minRange, double* maxRange, double* mean, double* stdDev, double* variance};
%apply int* OUTPUT {int* arrayId};
%apply int* OUTPUT {medUtilitiesVTK::DataArrayType* arrayType};

%include "medUtilities.h"
%include "medUtilitiesITK.h"
%include "medUtilitiesVTK.h"

%clear double* minRange;
%clear double* maxRange;
%clear double* mean;
%clear double* stdDev;
%clear double* variance;
%clear int* arrayId;
%clear medUtilitiesVTK::DataArrayType* arrayType;

%forwardFunction(setDerivedMetaData, medUtilities)
%forwardFunction(copyMetaDataIfEmpty, medUtilities)
%forwardFunction(metaDataKeysToCopyForDerivedData, medUtilities)
%forwardFunction(generateSeriesAndSOPInstanceId, medUtilities)
%forwardFunction(querySeriesDescription, medUtilities)
%forwardFunction(applyOrientationMatrix, medUtilities)
%forwardFunction(applyInverseOrientationMatrix, medUtilities)
%forwardFunction(switchTo3D, medUtilities)


%forwardFunction(itkDataImageId, medUtilitiesITK)
%forwardFunction(minimumValue, medUtilitiesITK)
%forwardFunction(maximumValue, medUtilitiesITK)
%forwardFunction(volume, medUtilitiesITK)
%forwardFunction(meanStdDeviation, medUtilitiesITK)

%forwardFunction(changeMaxNumberOfMeshTriangles, medUtilitiesVTK)
%forwardFunction(getArray, medUtilitiesVTK)
%forwardFunction(getArrayIndex, medUtilitiesVTK)
%forwardFunction(peekArray, medUtilitiesVTK)
%forwardFunction(arrayRange, medUtilitiesVTK)
%forwardFunction(arrayStats, medUtilitiesVTK)
