#pragma once

#include <medSettingsWidget.h>

#include "medPythonExport.h"

class medPythonSettingsWidgetPrivate;

class MEDPYTHON_EXPORT medPythonSettingsWidget : public medSettingsWidget
{
    Q_OBJECT
    MED_SETTINGS_INTERFACE("Python","Python Settings")

public:
    medPythonSettingsWidget(QWidget* parent = nullptr);
    ~medPythonSettingsWidget();

    bool write() override;

public slots:
    void read() override;
    bool validate() override;

private slots:
    void addModulePath();
    void removeModulePath(QString path);

private:
    medPythonSettingsWidgetPrivate* const d;

    void addModulePathWidgets(QString path);
};
