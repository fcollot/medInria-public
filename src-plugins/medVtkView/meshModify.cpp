/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/


#include "meshModify.h"

#include <medAbstractData.h>
#include <medAbstractDataFactory.h>
#include <medMetaDataKeys.h>
#include <medToolBoxFactory.h>
#include <medDataManager.h>
#include <medVtkViewBackend.h>
#include <medViewContainer.h>
#include <medTabbedViewContainers.h>

#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkTransform.h>
#include <vtkPolyData.h>

#include <vtkImageView3D.h>
#include <vtkMetaDataSet.h>
#include <vtkMetaDataSetSequence.h>
#include <vtkTransformFilter.h>
#include <vtkMetaSurfaceMesh.h>

class vtkMyCallback : public vtkCommand
{
public:
    static vtkMyCallback *New()
    { return new vtkMyCallback; }
    virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
        vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
        vtkBoxWidget * widget = reinterpret_cast<vtkBoxWidget*>(caller);
        widget->GetTransform(t);
        for(unsigned int i = 0; i < _dataset->GetNumberOfActors(); i++) {
            _dataset->GetActor(i)->SetUserTransform(t);
        }
    }
    void setDataSet(vtkMetaDataSet * dataset) {_dataset = dataset;}
    void setView(medAbstractLayeredView* view) {_view = view;}

private:
    vtkMetaDataSet * _dataset;
    medAbstractLayeredView* _view;
};


meshModifyToolBox::meshModifyToolBox(QWidget * parent)
    : medToolBox(parent)
    , _boxWidget(0)
    , _dataset(0)
    , _modifying(true)
{
    this->setTitle(tr("Mesh manipulation"));

    QWidget * w = new QWidget(this);
    this->addWidget(w);
    w->setLayout(new QVBoxLayout);
    
    _modifyButton = new QPushButton("Modify");
    _modifyButton->setEnabled(false);
    w->layout()->addWidget(_modifyButton);

    _cancelButton = new QPushButton("Cancel");
    _cancelButton->setEnabled(false);
    w->layout()->addWidget(_cancelButton);

    _exportButton = new QPushButton("Export");
    _exportButton->setEnabled(false);
    w->layout()->addWidget(_exportButton);

    _importButton = new QPushButton("Import");
    _importButton->setEnabled(false);
    w->layout()->addWidget(_importButton);

    connect(_modifyButton, SIGNAL(clicked()), this, SLOT(toggleWidget()));
    connect(_cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(_exportButton, SIGNAL(clicked()), this, SLOT(exportTransform()));
    connect(_importButton, SIGNAL(clicked()), this, SLOT(importTransform()));

    this->switchMinimize();
}


meshModifyToolBox::~meshModifyToolBox()
{
    
}


bool meshModifyToolBox::registered()
{
    return medToolBoxFactory::instance()->registerToolBox<meshModifyToolBox>();
}


void meshModifyToolBox::updateView()
{
    medTabbedViewContainers * containers = workspace->stackedViewContainers();
    QList<medViewContainer*> containersInTabSelected = containers->containersInTab(containers->currentIndex());
    medAbstractView *view=NULL;
    for(int i=0;i<containersInTabSelected.length();i++)
    {
        if (containersInTabSelected[i]->isSelected())
        {
            view = containersInTabSelected[i]->view();
            break;
        }
    }

    if (_view != view)
        cancel();

    medAbstractLayeredView * view3d = qobject_cast<medAbstractLayeredView*>(view);
    if (! view3d) {
        _modifyButton->setEnabled(false);
        return;
    }

    _view = view3d;
    _modifyButton->setEnabled(true);
}

void meshModifyToolBox::setWorkspace(medAbstractWorkspace* workspace)
{
    qDebug()<<"DANS SET WORKSPACE";
    this->workspace = workspace;
    medTabbedViewContainers * containers = workspace->stackedViewContainers();

    QObject::connect(containers,SIGNAL(containersSelectedChanged()),this,SLOT(updateView()));
    updateView();
}

void meshModifyToolBox::toggleWidget()
{
    medAbstractData * data = _view->layerData(_view->currentLayer());

    if ( ! _dataset && ! data->identifier().contains("vtkDataMesh"))
        return;

    if ( ! _dataset) {
        _dataset = reinterpret_cast<vtkMetaDataSet*>(data->data());
        if ( ! _dataset ) return;
    }

    vtkPointSet * pointset = dynamic_cast<vtkPointSet*>(_dataset->GetDataSet());
    if ( ! pointset ) {_dataset = 0;return;}

    if (_modifying) {
        _boxWidget = vtkSmartPointer<vtkBoxWidget>::New();
        _boxWidget->SetInteractor(static_cast<medVtkViewBackend*>(_view->backend())->view3D->GetInteractor());
        _boxWidget->SetPlaceFactor(1.25);

        double bounds[6] = {}; // init to zero
        pointset->GetBounds(bounds);
        _boxWidget->PlaceWidget(bounds);
        _callback = vtkSmartPointer<vtkMyCallback>::New();
        _callback->setDataSet(_dataset);
        _callback->setView(_view);
        _boxWidget->AddObserver(vtkCommand::InteractionEvent, _callback);

        _boxWidget->On();
        _cancelButton->setEnabled(true);
        _exportButton->setEnabled(true);
        _importButton->setEnabled(true);
    } else {
        vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
        _boxWidget->GetTransform(t);

        if (data->identifier() == "vtkDataMesh") {
            vtkSmartPointer<vtkTransformFilter> transformFilter =
                    vtkSmartPointer<vtkTransformFilter>::New();
            transformFilter->SetInput(pointset);
            transformFilter->SetTransform(t);
            transformFilter->Update();

            vtkPointSet * newPointset = pointset->NewInstance();
            newPointset->DeepCopy(transformFilter->GetOutput());

            dtkSmartPointer<medAbstractData> newData = medAbstractDataFactory::instance()->createSmartPointer("vtkDataMesh");

            newData->setMetaData(medMetaDataKeys::PatientName.key(), "John Doe");
            newData->setMetaData(medMetaDataKeys::StudyDescription.key(), "generated");
            newData->setMetaData(medMetaDataKeys::SeriesDescription.key(), "result of manual modification");

            vtkMetaDataSet * dataset = _dataset->NewInstance();
            dataset->SetDataSet(newPointset);
            newData->setData(dataset);
            medDataManager::instance()->importNonPersistent( newData.data() );

        } else if (data->identifier() == "vtkDataMesh4D") {
            vtkMetaDataSetSequence * seq = vtkMetaDataSetSequence::SafeDownCast(_dataset);
            const std::vector<vtkMetaDataSet*> & datasetList = seq->GetMetaDataSetList();

            dtkSmartPointer<medAbstractData> newData = medAbstractDataFactory::instance()->createSmartPointer("vtkDataMesh4D");
            newData->setMetaData(medMetaDataKeys::PatientName.key(), "John Doe");
            newData->setMetaData(medMetaDataKeys::StudyDescription.key(), "generated");
            newData->setMetaData(medMetaDataKeys::SeriesDescription.key(), "result of manual modification");

            vtkMetaDataSetSequence * newSeq = vtkMetaDataSetSequence::New();

            vtkSmartPointer<vtkTransformFilter> transformFilter =
                    vtkSmartPointer<vtkTransformFilter>::New();

            foreach(vtkMetaDataSet * dataset, datasetList) {
                vtkPointSet * pointset = vtkPointSet::SafeDownCast(dataset->GetDataSet());
                transformFilter->SetInput(pointset);
                transformFilter->SetTransform(t);
                transformFilter->Update();

                vtkPointSet * newPointset = pointset->NewInstance();
                newPointset->DeepCopy(transformFilter->GetOutput());

                vtkMetaDataSet * newDataset = dataset->NewInstance();
                newDataset->SetDataSet(newPointset);
                newDataset->SetTime(dataset->GetTime());

                newSeq->AddMetaDataSet(newDataset);
            }

            newData->setData(newSeq);
            medDataManager::instance()->importNonPersistent( newData.data() );
        }
        cancel();
    }

    _modifying = ! _modifying;
    _modifyButton->setText(_modifying ? "Modify" : "Save");
}


void meshModifyToolBox::cancel()
{
    if (_dataset) {
        // reset transforms on the original
        vtkSmartPointer<vtkTransform> t_id = vtkSmartPointer<vtkTransform>::New();
        for(unsigned int i = 0; i < _dataset->GetNumberOfActors(); i++) {
            _dataset->GetActor(i)->SetUserTransform(t_id);
        }
    }

    if (_boxWidget)
        _boxWidget->Off();

    _modifying = true;
    _modifyButton->setText("Modify");
    _cancelButton->setEnabled(false);
    _exportButton->setEnabled(false);
    _importButton->setEnabled(false);
    _dataset = 0;
}


void meshModifyToolBox::dataAdded(medAbstractData * data, int index)
{
    if (data->identifier().contains("vtkDataMesh"))
        _modifyButton->setEnabled(true);
}

void meshModifyToolBox::exportTransform()
{
    if ( ! _boxWidget || ! _boxWidget->GetEnabled())
        return;

    vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
    _boxWidget->GetTransform(t);

    vtkSmartPointer<vtkMatrix4x4>  m = vtkSmartPointer<vtkMatrix4x4>::New();
    m->DeepCopy(t->GetMatrix());

    QByteArray matrixStr;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            matrixStr += QByteArray::number(m->GetElement(i, j)) + "\t";
        }
        matrixStr += "\n";
    }

    QString filePath = QFileDialog::getSaveFileName(0, "Export the matrix file");
    if (filePath.isEmpty())
        return;

    QFile f(filePath);
    if ( ! f.open(QIODevice::WriteOnly)) {
        qDebug() << "Can't open file" << filePath;
        return;
    }

    f.write(matrixStr);
    f.close();

    qDebug() << "Done exporting tranform!";
}

void meshModifyToolBox::importTransform()
{
    QString filePath = QFileDialog::getOpenFileName(0, "Import the matrix file");
    if (filePath.isEmpty())
        return;

    QFile f(filePath);
    if ( ! f.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open file" << filePath;
        return;
    }

    QByteArray matrixStr = f.readAll();
    f.close();

    vtkSmartPointer<vtkMatrix4x4> m = vtkSmartPointer<vtkMatrix4x4>::New();
    int i = 0, j = 0;
    foreach(QByteArray line, matrixStr.split('\n')) {
        foreach(QByteArray num, line.split('\t')) {
            m->SetElement(i,j,num.toDouble());
            j++;
        }
        i++;j = 0;
    }

    vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
    t->SetMatrix(m);

    _boxWidget->SetTransform(t);
    _boxWidget->InvokeEvent(vtkCommand::InteractionEvent);
}