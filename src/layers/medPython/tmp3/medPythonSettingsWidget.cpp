#include "medPythonSettingsWidget.h"

#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "medExternalPythonModules.h"

namespace med
{

class PythonSettingsWidgetPrivate
{
public:
    QWidget* modulePathWidgets;
    QStringList modulePaths;
};

PythonSettingsWidget::PythonSettingsWidget(QWidget* parent) :
    medSettingsWidget(parent), d(new PythonSettingsWidgetPrivate())
{
    setLayout(new QVBoxLayout);

    QPushButton* addModulePathButton = new QPushButton("Add module path");
    layout()->addWidget(addModulePathButton);
    connect(addModulePathButton, &QPushButton::clicked, this, &PythonSettingsWidget::addModulePath);

    d->modulePathWidgets = new QWidget();
    d->modulePathWidgets->setLayout(new QVBoxLayout);
    layout()->addWidget(d->modulePathWidgets);
}

PythonSettingsWidget::~PythonSettingsWidget()
{
    delete d;
}

bool PythonSettingsWidget::write()
{
    ExternalPythonModules::setPaths(d->modulePaths);
    return true;
}

void PythonSettingsWidget::read()
{
    d->modulePaths = ExternalPythonModules::getPaths();

    foreach (QString path, d->modulePaths)
    {
        addModulePathWidgets(path);
    }
}

bool PythonSettingsWidget::validate()
{
    return true;
}

void PythonSettingsWidget::addModulePath()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select the module directory", QString(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!(path.isEmpty() || d->modulePaths.contains(path)))
    {
        d->modulePaths.append(path);
        addModulePathWidgets(path);
    }
}

void PythonSettingsWidget::removeModulePath(QString path)
{
    d->modulePaths.removeOne(path);
}

void PythonSettingsWidget::addModulePathWidgets(QString path)
{
    QWidget* widget = new QWidget();
    widget->setLayout(new QHBoxLayout);
    d->modulePathWidgets->layout()->addWidget(widget);
    widget->layout()->addWidget(new QLabel(path));
    QPushButton* removeRow = new QPushButton("remove");
    widget->layout()->addWidget(removeRow);
    connect(removeRow, &QPushButton::clicked, widget, &QWidget::deleteLater);
    connect(removeRow, &QPushButton::clicked, this, [=](){ removeModulePath(path); });
}

}
