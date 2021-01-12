#include "medPythonSettingsWidget.h"

#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "medPythonModulePaths.h"

class medPythonSettingsWidgetPrivate
{
public:
    QWidget* modulePathWidgets;
    QStringList modulePaths;
};

medPythonSettingsWidget::medPythonSettingsWidget(QWidget* parent) :
    medSettingsWidget(parent), d(new medPythonSettingsWidgetPrivate())
{
    setLayout(new QVBoxLayout);

    QPushButton* addModulePathButton = new QPushButton("Add module path");
    layout()->addWidget(addModulePathButton);
    connect(addModulePathButton, &QPushButton::clicked, this, &medPythonSettingsWidget::addModulePath);

    d->modulePathWidgets = new QWidget();
    d->modulePathWidgets->setLayout(new QVBoxLayout);
    layout()->addWidget(d->modulePathWidgets);
}

medPythonSettingsWidget::~medPythonSettingsWidget()
{
    delete d;
}

bool medPythonSettingsWidget::write()
{
    medPythonModulePaths::setPaths(d->modulePaths);
    return true;
}

void medPythonSettingsWidget::read()
{
    d->modulePaths = medPythonModulePaths::getPaths();

    foreach (QString path, d->modulePaths)
    {
        addModulePathWidgets(path);
    }
}

bool medPythonSettingsWidget::validate()
{
    return true;
}

void medPythonSettingsWidget::addModulePath()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select the module directory", QString(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!(path.isEmpty() || d->modulePaths.contains(path)))
    {
        d->modulePaths.append(path);
        addModulePathWidgets(path);
    }
}

void medPythonSettingsWidget::removeModulePath(QString path)
{
    d->modulePaths.removeOne(path);
}

void medPythonSettingsWidget::addModulePathWidgets(QString path)
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
