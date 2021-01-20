#include "SettingsWidget.h"

#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "ExternalModules.h"

namespace medPython
{

class SettingsWidgetPrivate
{
public:
    QWidget* modulePathWidgets;
    QStringList modulePaths;
};

SettingsWidget::SettingsWidget(QWidget* parent) :
    medSettingsWidget(parent), d(new SettingsWidgetPrivate())
{
    setLayout(new QVBoxLayout);

    QPushButton* addModulePathButton = new QPushButton("Add module path");
    layout()->addWidget(addModulePathButton);
    connect(addModulePathButton, &QPushButton::clicked, this, &SettingsWidget::addModulePath);

    d->modulePathWidgets = new QWidget();
    d->modulePathWidgets->setLayout(new QVBoxLayout);
    layout()->addWidget(d->modulePathWidgets);
}

SettingsWidget::~SettingsWidget()
{
    delete d;
}

bool SettingsWidget::write()
{
    ModulePaths::setPaths(d->modulePaths);
    return true;
}

void SettingsWidget::read()
{
    d->modulePaths = ModulePaths::getPaths();

    foreach (QString path, d->modulePaths)
    {
        addModulePathWidgets(path);
    }
}

bool SettingsWidget::validate()
{
    return true;
}

void SettingsWidget::addModulePath()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select the module directory", QString(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!(path.isEmpty() || d->modulePaths.contains(path)))
    {
        d->modulePaths.append(path);
        addModulePathWidgets(path);
    }
}

void SettingsWidget::removeModulePath(QString path)
{
    d->modulePaths.removeOne(path);
}

void SettingsWidget::addModulePathWidgets(QString path)
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
