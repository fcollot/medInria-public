%ignore medToolBox::addToolBox;
%ignore medToolBox::removeToolBox;
%ignore medToolBox::success;
%ignore medToolBox::failure;

%{
#include "medToolBox.h"
#include "medToolBoxBody.h"
#include "medDoubleParameterL.h"
#include "medAbstractParameterL.h"
%}

%include "medToolBox.h"

/* Useful functions to provide functionality not otherwise possible in Python
 * because of the difficulty in wrapping Qt with SWIG.
 */
%extend medToolBox
{
    void showWidget(QString name)
    {
        $self->findChild<QWidget*>(name)->show();
    }

    void hideWidget(QString name)
    {
        $self->findChild<QWidget*>(name)->hide();
    }

    void hideParameter(QString name)
    {
      $self->findChild<medAbstractParameterL*>(name)->hide();
    }

    void clickButton(QString name)
    {
        $self->findChild<QAbstractButton*>(name)->click();
    }

    void setSpinBoxValue(QString name, int value)
    {
        $self->findChild<QSpinBox*>(name)->setValue(value);
    }

    void setSpinBoxValue(QString name, double value)
    {
        $self->findChild<QDoubleSpinBox*>(name)->setValue(value);
    }

    void setCheckBox(QString name, bool value)
    {
        $self->findChild<QCheckBox*>(name)->setChecked(value);
    }

    void setCurrentIndex(QString name, int index)
    {
        $self->findChild<QComboBox*>(name)->setCurrentIndex(index);
    }

    PyObject* getSpinBoxValue(QString name)
    {
        QSpinBox* spinBox = $self->findChild<QSpinBox*>(name);

        if (spinBox)
        {
            return PyLong_FromLong(spinBox->value());
        }
        else
        {
            QDoubleSpinBox* doubleSpinBox = $self->findChild<QDoubleSpinBox*>(name);

            if (doubleSpinBox)
            {
                return PyFloat_FromDouble(doubleSpinBox->value());
            }
            else
            {
                Py_RETURN_NONE;
            }
        }
    }

    PyObject* getMedDoubleParameterValue(QString name)
    {
        medDoubleParameterL* parameter = $self->findChild<medDoubleParameterL*>(name);
        if (parameter)
        {
            return PyFloat_FromDouble(parameter->value());
        }
        else
        {
            Py_RETURN_NONE;
        }
    }

    bool isMinimized()
    {
        return $self->body()->isHidden();
    }
}
