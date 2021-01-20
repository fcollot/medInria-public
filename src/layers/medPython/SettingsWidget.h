#pragma once

#include <medSettingsWidget.h>

#include "Export.h"

namespace medPython
{

class SettingsWidgetPrivate;

class MEDPYTHON_EXPORT SettingsWidget : public medSettingsWidget
{
    Q_OBJECT
    MED_SETTINGS_INTERFACE("Python","Python Settings")

public:
    SettingsWidget(QWidget* parent = nullptr);
    ~SettingsWidget();

    bool write() override;

public slots:
    void read() override;
    bool validate() override;

private slots:
    void addModulePath();
    void removeModulePath(QString path);

private:
    SettingsWidgetPrivate* const d;

    void addModulePathWidgets(QString path);
};

}
