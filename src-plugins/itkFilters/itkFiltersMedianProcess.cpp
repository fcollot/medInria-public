/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <itkFiltersMedianProcess.h>

#include <dtkCore/dtkAbstractProcessFactory.h>

#include <itkImage.h>
#include <itkMedianImageFilter.h>

#include <medUtilities.h>

//-------------------------------------------------------------------------------------------

itkFiltersMedianProcess::itkFiltersMedianProcess(itkFiltersMedianProcess *parent) 
    : itkFiltersProcessBase(parent)
{
}

//-------------------------------------------------------------------------------------------

bool itkFiltersMedianProcess::registered( void )
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("itkMedianProcess", createitkFiltersMedianProcess);
}

//-------------------------------------------------------------------------------------------

QString itkFiltersMedianProcess::description() const
{
    return tr("ITK median filter");
}

//-------------------------------------------------------------------------------------------

int itkFiltersMedianProcess::tryUpdate()
{
    int res = DTK_FAILURE;

    if ( getInputData() )
    {
        res = DISPATCH_ON_PIXEL_TYPE(&itkFiltersMedianProcess::updateProcess, this, getInputData());
    }

    return res;
}

template <class PixelType> int itkFiltersMedianProcess::updateProcess()
{
    typedef itk::Image< PixelType, 3 > ImageType;
    typedef itk::MedianImageFilter< ImageType, ImageType >  MedianFilterType;
    typename MedianFilterType::Pointer medianFilter = MedianFilterType::New();

    medianFilter->SetInput ( dynamic_cast<ImageType *> ( ( itk::Object* ) ( getInputData()->data() ) ) );

    itk::CStyleCommand::Pointer callback = itk::CStyleCommand::New();
    callback->SetClientData ( ( void * ) this );
    callback->SetCallback ( itkFiltersProcessBase::eventCallback );
    medianFilter->AddObserver ( itk::ProgressEvent(), callback );

    medianFilter->Update();

    getOutputData()->setData ( medianFilter->GetOutput() );

    //Set output description metadata
    medUtilities::setDerivedMetaData(getOutputData(), getInputData(), "median filter");

    return DTK_SUCCEED;
}

// /////////////////////////////////////////////////////////////////
// Type instanciation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess * createitkFiltersMedianProcess ( void )
{
    return new itkFiltersMedianProcess;
}
