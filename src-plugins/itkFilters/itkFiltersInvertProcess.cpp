/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <itkFiltersInvertProcess.h>

#include <dtkCore/dtkAbstractProcessFactory.h>

#include <itkImage.h>
#include <itkInvertIntensityImageFilter.h>

#include <medUtilities.h>

itkFiltersInvertProcess::itkFiltersInvertProcess(itkFiltersInvertProcess *parent)
    : itkFiltersProcessBase(parent)
{
}

//-------------------------------------------------------------------------------------------

bool itkFiltersInvertProcess::registered( void )
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("itkInvertProcess", createitkFiltersInvertProcess);
}

//-------------------------------------------------------------------------------------------

QString itkFiltersInvertProcess::description() const
{
    return tr("ITK invert intensity filter");
}

//-------------------------------------------------------------------------------------------

int itkFiltersInvertProcess::tryUpdate()
{
    int res = DTK_FAILURE;

    if ( getInputData() )
    {
        res = DISPATCH_ON_PIXEL_TYPE(&itkFiltersInvertProcess::updateProcess, this, getInputData());
    }

    return res;
}

template <class PixelType> int itkFiltersInvertProcess::updateProcess()
{
    typedef itk::Image< PixelType, 3 > ImageType;
    typedef itk::InvertIntensityImageFilter< ImageType, ImageType >  InvertFilterType;
    typename InvertFilterType::Pointer invertFilter = InvertFilterType::New();

    // compute maximum intensity of image
    typedef itk::MinimumMaximumImageCalculator< ImageType > MinMaxFilterType;
    typename MinMaxFilterType::Pointer maxFilter = MinMaxFilterType::New();

    maxFilter->SetImage( dynamic_cast<ImageType *> ( ( itk::Object* ) ( getInputData()->data() ) ) );
    maxFilter->Compute();
    PixelType maximum = maxFilter->GetMaximum();
    PixelType minimum = maxFilter->GetMinimum();

    invertFilter->SetInput ( dynamic_cast<ImageType *> ( ( itk::Object* ) ( getInputData()->data() ) ) );
    invertFilter->SetMaximum(maximum + minimum);

    itk::CStyleCommand::Pointer callback = itk::CStyleCommand::New();
    callback->SetClientData ( ( void * ) this );
    callback->SetCallback ( itkFiltersProcessBase::eventCallback );
    invertFilter->AddObserver ( itk::ProgressEvent(), callback );

    invertFilter->Update();

    getOutputData()->setData ( invertFilter->GetOutput() );

    //Set output description metadata
    medUtilities::setDerivedMetaData(getOutputData(), getInputData(), "invert filter");

    return DTK_SUCCEED;
}

template <> int itkFiltersInvertProcess::updateProcess<float>()
{
    qDebug() << "Error: Invert image filter does not suport floating pixel values";
    return medAbstractProcess::PIXEL_TYPE;
}

template <> int itkFiltersInvertProcess::updateProcess<double>()
{
    return updateProcess<float>();
}

// /////////////////////////////////////////////////////////////////
// Type instanciation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess * createitkFiltersInvertProcess ( void )
{
    return new itkFiltersInvertProcess;
}
