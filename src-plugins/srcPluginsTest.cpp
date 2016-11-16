#include "srcPluginsTest.h"

bool srcPluginsTest::registered()
{
    return medPipelineRegistry::registerDelegate<srcPluginsTest>();
}

srcPluginsTest::srcPluginsTest()
{
}

srcPluginsTest::~srcPluginsTest()
{
}

medPipelineStepProperties srcPluginsTest::createDelegateDisplayResultsStep()
{
    return createDisplayStep("DisplayResults")
            .setDelegateClass(staticMetaObject.className());
}

void srcPluginsTest::startDisplayResultsStep(medPipelineWorkingState* state)
{
    QList<dtkSmartPointer<medAbstractData> > meshes;
    foreach (QString identifier, state->getAllMedData())
    {
        dtkSmartPointer<medAbstractData> data = state->peekValue(identifier);
        foreach (medAttachedData* attachedData, data->attachedData())
        {
            medPipelineAttachedData* pipelineAttachedData = dynamic_cast<medPipelineAttachedData*>(attachedData);
            if (pipelineAttachedData && pipelineAttachedData->hasRenderingParameters())
            {
                meshes.append(data);
                break;
            }
        }
    }
    QHash<QString, QVariant> resultValues = state->peekValue<QHash<QString, QVariant> >(resultValuesId);
    startDisplayResultsStep(meshes, resultValues);
}

medPipelineWorkspace* srcPluginsTest::getWorkspace()
{
    return medPipelineWorkspace::instance();
}

medPipelineStepProperties srcPluginsTest::createDisplayStep(QString name)
{
    return medPipelineStepProperties(name)
            .setTitle("Display results")
            .setDescription("Visualisation of the results");
}

void srcPluginsTest::startDisplayResultsStep(QList<dtkSmartPointer<medAbstractData> > meshes, QHash<QString, QVariant> values)
{
    for (int i = 0; i < meshes.length(); i++)
    {
        getWorkspace()->addData(meshes[i]);
    }

    if (meshes.length() > 0)
    {
        getWorkspace()->setVolumeRendering();
    }

    QPushButton* exportSceneButton = new QPushButton(QIcon(":icons/scene.png"), "Export the scene");
    exportSceneButton->setIconSize(QSize(40, 40));
    exportSceneButton->setMinimumSize(100, 60);
    exportSceneButton->setMaximumSize(170, 60);
    connect(exportSceneButton, SIGNAL(clicked()), this, SLOT(exportScene()));
    medToolBox* toolbox =  new medToolBox;
    QWidget* widget = new QWidget(toolbox);
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->addWidget(exportSceneButton);
    widget->setLayout(layout);
    toolbox->addWidget(widget);
    getWorkspace()->addToolBox("Export Scene", toolbox);

    medPipelineValuesToolBox& valuesToolBox = static_cast<medPipelineValuesToolBox&>(getWorkspace()->addToolBox("medPipelineValuesToolBox"));
    valuesToolBox.addValues(values);
}

void srcPluginsTest::setResultValue(medPipelineWorkingState* state, QString name, QVariant value)
{
    QHash<QString, QVariant> resultValues = state->peekValue<QHash<QString, QVariant> >(resultValuesId);
    resultValues[name] = value;
    state->setValue(resultValuesId, resultValues);
}

void srcPluginsTest::exportScene()
{
    medAbstractWorkspace* workspace = medPipelineWorkspace::instance();

    medTabbedViewContainers* tabbedViewContainers = workspace->stackedViewContainers();
    QList<medViewContainer*> containersList = tabbedViewContainers->containersInTab(tabbedViewContainers->currentIndex());

    if(containersList.size() != 1)
    {
        medMessageController::instance()->showError("Scene saving only works with one container!", 5000);
    }
    else
    {
        //Export scene
        QDir workingDir(containersList.first()->saveScene());
        //Removing field data
        QDirIterator it(workingDir.absolutePath(), QStringList() << "*.vtk", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            it.next();
            vtkSmartPointer<vtkGenericDataObjectReader> reader =
                    vtkSmartPointer<vtkGenericDataObjectReader>::New();
            reader->SetFileName(it.filePath().toStdString().c_str());
            reader->Update();

            // All of the standard data types can be checked and obtained like this:
            if(reader->IsFilePolyData())
            {
                vtkPolyData* polyData = reader->GetPolyDataOutput();
                vtkSmartPointer<vtkFieldData> fieldData = polyData->GetFieldData();
                vtkSmartPointer<vtkFieldData> newFieldData = vtkSmartPointer<vtkFieldData>::New();

                for (int i = 0; i < fieldData->GetNumberOfArrays(); i++)
                {
                    QString arrayName = fieldData->GetArrayName(i);

                    if (!arrayName.startsWith("medMetaData::"))
                    {
                        newFieldData->AddArray(fieldData->GetAbstractArray(i));
                    }
                }

                polyData->SetFieldData(newFieldData);
                vtkMetaSurfaceMesh* mesh = vtkMetaSurfaceMesh::New();
                mesh->SetDataSet(polyData);
                mesh->Write(it.filePath().toStdString().c_str());
                mesh->Delete();
            }
        }
    }

}
