#include "v3dViewFiberInteractor.h"

#include <dtkCore/dtkAbstractData.h>
#include <dtkCore/dtkAbstractView.h>
#include <dtkCore/dtkAbstractViewFactory.h>

#include <vtkPolyData.h>
#include <vtkFibersManager.h>
#include <vtkImageView.h>

class v3dViewFiberInteractorPrivate
{
public:
    dtkAbstractData *data;
    dtkAbstractView *view;
	
	vtkFibersManager *manager;
	QHash<QString, dtkAbstractData*> bundleList;
};

v3dViewFiberInteractor::v3dViewFiberInteractor(): dtkAbstractViewInteractor(), d(new v3dViewFiberInteractorPrivate)
{
	d->data = 0;
	d->view = 0;
	d->manager = vtkFibersManager::New();
	
	// addProperty here
	this->addProperty("Visibility", QStringList() << "true" << "false");
	this->addProperty("BoxVisibility", QStringList() << "true" << "false");
	this->addProperty("RenderingMode", QStringList() << "lines" << "ribbons" << "tubes");
	this->addProperty("GPUMode", QStringList() << "true" << "false");
	this->addProperty("ColorMode", QStringList() << "local" << "global");
	this->addProperty("BoxBooleanOperation", QStringList() << "plus" << "minus");
}

v3dViewFiberInteractor::~v3dViewFiberInteractor()
{
	d->manager->Delete();
}

QString v3dViewFiberInteractor::description(void) const
{
	return "v3dViewFiberInteractor";
}

QStringList v3dViewFiberInteractor::handled(void) const
{
	return QStringList () << "v3dView";
}

bool v3dViewFiberInteractor::registered(void)
{
    return dtkAbstractViewFactory::instance()->registerViewInteractorType("v3dViewFiberInteractor", QStringList() << "v3dView", createV3dViewFiberInteractor);
}

void v3dViewFiberInteractor::setData(dtkAbstractData *data)
{
    if (vtkPolyData *fibers = dynamic_cast<vtkPolyData *>((vtkDataObject *)(data->data()))) {
		d->manager->SetInput(fibers);
		d->data = data;
	}
}

void v3dViewFiberInteractor::setView(dtkAbstractView *view)
{
	if (vtkImageView *vtkView = dynamic_cast<vtkImageView *>((vtkObject *)(view->view()))) {
		d->manager->SetRenderWindowInteractor(vtkView->GetInteractor());
		d->view = view;
    }
}

void v3dViewFiberInteractor::onPropertySet(QString key, QString value)
{
	if (key=="Visibility")
		this->onVisibilityPropertySet (value);
	
	if (key=="BoxVisibility")
		this->onBoxVisibilityPropertySet (value);
	
	if (key=="RenderingMode")
		this->onRenderingModePropertySet (value);
	
	if (key=="GPUMode")
		this->onGPUModePropertySet (value);
	
	if (key=="ColorMode")
		this->onColorModePropertySet (value);
	
	if (key=="BoxBooleanOperation")
		this->onBoxBooleanOperationPropertySet (value);
	
	if (d->view)
		d->view->update();
}

void v3dViewFiberInteractor::onVisibilityPropertySet (QString value)
{
	if (value=="true")
		d->manager->SetVisibility(1);
	else
		d->manager->SetVisibility(0);
}

void v3dViewFiberInteractor::onBoxVisibilityPropertySet (QString value)
{
	if (value=="true")
		d->manager->SetBoxWidget(1);
	else
		d->manager->SetBoxWidget(0);
}

void v3dViewFiberInteractor::onRenderingModePropertySet (QString value)
{
	if (value=="lines")
		d->manager->SetRenderingModeToPolyLines();
	
	if (value=="ribbons")
		d->manager->SetRenderingModeToRibbons();
	
	if (value=="tubes")
		d->manager->SetRenderingModeToTubes();
}

void v3dViewFiberInteractor::onGPUModePropertySet (QString value)
{
	if (value=="true") {
		vtkFibersManager::UseHardwareShadersOn();
		d->manager->ChangeMapperToUseHardwareShaders();
	}
	else {
		vtkFibersManager::UseHardwareShadersOff();
		d->manager->ChangeMapperToDefault();
	}
}

void v3dViewFiberInteractor::onColorModePropertySet (QString value)
{
	if (value=="local")
		d->manager->SetColorModeToLocalFiberOrientation();
	
	if (value=="global")
		d->manager->SetColorModelToGlobalFiberOrientation();
}

void v3dViewFiberInteractor::onBoxBooleanOperationPropertySet (QString value)
{
	if (value=="plus")
		d->manager->GetVOILimiter()->SetBooleanOperationToAND();
	
	if (value=="minus")
		d->manager->GetVOILimiter()->SetBooleanOperationToNOT();
	
	d->manager->GetVOILimiter()->Modified();
}

void v3dViewFiberInteractor::onSelectionTagged(void)
{
	d->manager->SwapInputOutput();
}

void v3dViewFiberInteractor::onSelectionReset(void)
{
	d->manager->Reset();
}

// /////////////////////////////////////////////////////////////////
// Type instanciation
// /////////////////////////////////////////////////////////////////

dtkAbstractViewInteractor *createV3dViewFiberInteractor(void)
{
    return new v3dViewFiberInteractor;
}
