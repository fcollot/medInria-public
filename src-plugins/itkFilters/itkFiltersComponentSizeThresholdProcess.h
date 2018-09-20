/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include "itkFiltersProcessBase.h"

class itkFiltersComponentSizeThresholdProcessPrivate;
class dtkAbstractData;

namespace itk
{
template <typename PixelType, unsigned int Dimension> class Image;
}

class ITKFILTERSPLUGIN_EXPORT itkFiltersComponentSizeThresholdProcess : public itkFiltersProcessBase
{
    Q_OBJECT
    
public:
    typedef itk::Image<unsigned short, 3> OutputImageType;

    static const double defaultMinimumSize;

    itkFiltersComponentSizeThresholdProcess(itkFiltersComponentSizeThresholdProcess * parent = 0);
    itkFiltersComponentSizeThresholdProcess(const itkFiltersComponentSizeThresholdProcess& other);
    virtual ~itkFiltersComponentSizeThresholdProcess(void);
    static bool registered ( void );

    virtual QString description(void) const;
    
public slots:

    void setParameter ( int  data, int channel );
    int tryUpdate();

protected:
    template <class InputImageType> dtkSmartPointer<medAbstractData> castToOutputType(medAbstractData* inputData);
    template <class InputImageType> int updateProcess(medAbstractData* inputData);

private:
    itkFiltersComponentSizeThresholdProcessPrivate *d;
};

dtkAbstractProcess * createitkFiltersComponentSizeThresholdProcess(void);
