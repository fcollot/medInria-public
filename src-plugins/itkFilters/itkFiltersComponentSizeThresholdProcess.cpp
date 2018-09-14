/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "itkFiltersComponentSizeThresholdProcess.h"

#include <dtkCore/dtkAbstractProcessFactory.h>

#include <itkBinaryThresholdImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkImage.h>
#include <itkRelabelComponentImageFilter.h>

#include <medAbstractDataFactory.h>
#include <medUtilities.h>

class itkFiltersComponentSizeThresholdProcessPrivate
{
public:
    double minimumSize;
};

const double itkFiltersComponentSizeThresholdProcess::defaultMinimumSize = 50.0;

//-------------------------------------------------------------------------------------------

itkFiltersComponentSizeThresholdProcess::itkFiltersComponentSizeThresholdProcess(itkFiltersComponentSizeThresholdProcess *parent) 
    : itkFiltersProcessBase(parent), d(new itkFiltersComponentSizeThresholdProcessPrivate)
{  
    d->minimumSize = defaultMinimumSize;
}

itkFiltersComponentSizeThresholdProcess::itkFiltersComponentSizeThresholdProcess(const itkFiltersComponentSizeThresholdProcess& other)
     : itkFiltersProcessBase(other), d(other.d)
{
}

//-------------------------------------------------------------------------------------------

itkFiltersComponentSizeThresholdProcess::~itkFiltersComponentSizeThresholdProcess( void )
{
    delete d;
}

//-------------------------------------------------------------------------------------------

bool itkFiltersComponentSizeThresholdProcess::registered( void )
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("itkComponentSizeThresholdProcess", createitkFiltersComponentSizeThresholdProcess);
}

QString itkFiltersComponentSizeThresholdProcess::description() const
{
    return tr("Size Threshold filter");
}

//-------------------------------------------------------------------------------------------

void itkFiltersComponentSizeThresholdProcess::setParameter(int data, int channel)
{
    if (channel != 0)
        return;
     
    d->minimumSize = data;
}

//-------------------------------------------------------------------------------------------

int itkFiltersComponentSizeThresholdProcess::tryUpdate()
{  
    int res = DTK_FAILURE;

    if ( getInputData() )
    {
        res = DISPATCH_ON_PIXEL_TYPE(&itkFiltersComponentSizeThresholdProcess::updateProcess, this, getInputData());
    }

    return res;
}

template <class PixelType> int itkFiltersComponentSizeThresholdProcess::updateProcess()
{
    typedef itk::Image< PixelType, 3 > ImageType;
    typedef itk::Image< unsigned short, 3 > OutputImageType;

    typedef itk::ConnectedComponentImageFilter <ImageType, OutputImageType> ConnectedComponentFilterType;
    typename ConnectedComponentFilterType::Pointer connectedComponentFilter = ConnectedComponentFilterType::New();
    connectedComponentFilter->SetInput ( dynamic_cast<ImageType *> ( ( itk::Object* ) (getInputData()->data() ) ) );
    connectedComponentFilter->Update();

    // RELABEL COMPONENTS according to their sizes (0:largest(background))
    typedef itk::RelabelComponentImageFilter<OutputImageType, OutputImageType> FilterType;
    typename FilterType::Pointer relabelFilter = FilterType::New();
    relabelFilter->SetInput(connectedComponentFilter->GetOutput());
    relabelFilter->SetMinimumObjectSize(d->minimumSize);
    relabelFilter->Update();

    // BINARY FILTER
    typedef itk::BinaryThresholdImageFilter <OutputImageType, OutputImageType> BinaryThresholdImageFilterType;
    typename BinaryThresholdImageFilterType::Pointer thresholdFilter
            = BinaryThresholdImageFilterType::New();
    thresholdFilter->SetInput(relabelFilter->GetOutput());
    thresholdFilter->SetUpperThreshold(0);
    thresholdFilter->SetInsideValue(0);
    thresholdFilter->SetOutsideValue(1);

    thresholdFilter->Update();

    itk::CStyleCommand::Pointer callback = itk::CStyleCommand::New();
    callback->SetClientData ( ( void * ) this );
    callback->SetCallback ( itkFiltersProcessBase::eventCallback );
    connectedComponentFilter->AddObserver ( itk::ProgressEvent(), callback );

    setOutputData(medAbstractDataFactory::instance()->createSmartPointer ( "itkDataImageUShort3" ));
    getOutputData()->setData ( thresholdFilter->GetOutput() );

    QString newSeriesDescription = "connectedComponent " + QString::number(d->minimumSize);
    medUtilities::setDerivedMetaData(getOutputData(), getInputData(), newSeriesDescription);

    return DTK_SUCCEED;
}

template <> int itkFiltersComponentSizeThresholdProcess::updateProcess<float>()
{
    return castToUInt3AndUpdate<float>();
}

template <> int itkFiltersComponentSizeThresholdProcess::updateProcess<double>()
{
    return castToUInt3AndUpdate<double>();
}

template <class PixelType> int itkFiltersComponentSizeThresholdProcess::castToUInt3AndUpdate()
{
    if (castToUInt3<PixelType>() == DTK_SUCCEED)
    {
        return updateProcess<unsigned int>();
    }
    else
    {
        return DTK_FAILURE;
    }
}

template <class PixelType> int itkFiltersComponentSizeThresholdProcess::castToUInt3()
{
    //we will later label the image so we don't care about precision.
    typedef itk::Image< PixelType, 3 > InputImageType;
    typedef itk::Image< unsigned int, 3 > OutputImageType;
    typedef itk::CastImageFilter< InputImageType, OutputImageType > CastFilterType;

    typename CastFilterType::Pointer  caster = CastFilterType::New();
    typename InputImageType::Pointer im = dynamic_cast< InputImageType*>((itk::Object*)(getInputData()->data()));
    caster->SetInput(im);
    caster->Update();

    setInputData(medAbstractDataFactory::instance()->createSmartPointer ( "itkDataImageUInt3" ));
    getInputData()->setData(caster->GetOutput());

    return DTK_SUCCEED;
}

// /////////////////////////////////////////////////////////////////
// Type instanciation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess * createitkFiltersComponentSizeThresholdProcess ( void )
{
    return new itkFiltersComponentSizeThresholdProcess;
}
