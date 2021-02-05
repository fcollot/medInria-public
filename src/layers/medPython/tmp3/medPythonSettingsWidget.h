#pragma once

#include <medSettingsWidget.h>

#include "medPythonExport.h"

namespace med
{

class PythonSettingsWidgetPrivate;

class MEDPYTHON_EXPORT PythonSettingsWidget : public medSettingsWidget
{
    Q_OBJECT
    MED_SETTINGS_INTERFACE("PythonSettings","Python Settings")

public:
    PythonSettingsWidget(QWidget* parent = nullptr);
    ~PythonSettingsWidget();

    bool write() override;

public slots:
    void read() override;
    bool validate() override;

private slots:
    void addModulePath();
    void removeModulePath(QString path);

private:
    PythonSettingsWidgetPrivate* const d;

    void addModulePathWidgets(QString path);
};

}
