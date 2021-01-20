%{
#include <QWidget>
%}

%feature("director") QWidget;

class QWidget : public QObject, public QPaintDevice
{
public:
    virtual void setVisible(bool visible);
    void setHidden(bool hidden);
    void show();
    void hide();

    void showMinimized();
    void showMaximized();
    void showFullScreen();
    void showNormal();

    bool close();
    void raise();
    void lower();
};
