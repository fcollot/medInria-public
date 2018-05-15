/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <itkFiltersProcessBase.h>

class medAbstractData;
class itkFiltersSubtractProcessPrivate;

class ITKFILTERSPLUGIN_EXPORT itkFiltersSubtractProcess : public itkFiltersProcessBase
{
    Q_OBJECT

public:
    static const double defaultSubtractValue;

    itkFiltersSubtractProcess(itkFiltersSubtractProcess * parent = 0);
    itkFiltersSubtractProcess(const itkFiltersSubtractProcess& other);
    virtual ~itkFiltersSubtractProcess(void);
    static bool registered(void);

    virtual QString description(void) const;

protected:
    void setDoubleParameter(double data, int channel = 0, int frame = 0) override;

    template <class PixelType> int updateProcess();

    int internalUpdate() override;

private:
    itkFiltersSubtractProcessPrivate *d;
};

dtkAbstractProcess * createitkFiltersSubtractProcess(void);


