#pragma once

class MEDPIPELINEPLUGIN_EXPORT srcPluginsTest : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE srcPluginsTest();
    virtual ~srcPluginsTest();

public slots:
    static medPipelineStepProperties createDelegateDisplayResultsStep();
    void startDisplayResultsStep(medPipelineWorkingState* state);
    void exportScene();

protected:
    medPipelineWorkspace* getWorkspace();

    static medPipelineStepProperties createDisplayStep(QString name);
    void startDisplayResultsStep(QList<dtkSmartPointer<medAbstractData> > meshes, QHash<QString, QVariant> values);

    void setResultValue(medPipelineWorkingState* state, QString name, QVariant value);

private:
};
