#include "medImageSelectionWidget.h"

#include <QtGui>

#include <medDataManager.h>
#include <medDatabaseView.h>
#include <medDatabasePreview.h>
#include <medDatabaseProxyModel.h>
#include <medDatabaseModel.h>
#include <medDatabaseExporter.h>
#include <medAbstractDbController.h>
#include <medDropSite.h>
#include <medThumbnailContainer.h>

class medImageSelectionWidgetPrivate
{
public:
    // data
    medDatabaseModel *model;
    medDatabaseProxyModel *proxy;

    // view
    medThumbnailContainer *preview;
    medDatabaseView *view;
    medThumbnailContainer *selected;

    QPushButton* btOk;
    QPushButton* btCancel;
};

medImageSelectionWidget::medImageSelectionWidget(QList<medDataIndex> previouslySelectedIndexes, QWidget *parent) : d(new medImageSelectionWidgetPrivate)
{
    bool justBringStudies = true;
    d->model = new medDatabaseModel(this, justBringStudies);
    d->proxy = new medDatabaseProxyModel(this);
    d->proxy->setSourceModel(d->model);

    d->preview = new medThumbnailContainer(previouslySelectedIndexes);
    d->preview->setAllowDeleting(false);
    d->preview->setAllowDragging(true);
    d->preview->setAllowDropping(false);

    d->selected = new medThumbnailContainer(previouslySelectedIndexes);
    d->selected->setAllowDeleting(true);
    d->selected->setAllowDragging(false);
    d->selected->setAllowDropping(true);

    d->view = new medDatabaseView();
    d->view->setModel(d->proxy);

    d->btOk = new QPushButton(tr("OK"));
    d->btCancel = new QPushButton(tr("Cancel"));
    d->btOk->setDefault(true);

    // layouting

    QVBoxLayout* mainLayout = new QVBoxLayout();

    QSplitter* splitterHorizontal = new QSplitter(Qt::Horizontal);
    QSplitter* splitterVertical = new QSplitter(Qt::Vertical);
    splitterHorizontal->setHandleWidth(1);
    splitterVertical->setHandleWidth(1);
	splitterHorizontal->setOpaqueResize(false);
	splitterVertical->setOpaqueResize(false);

    splitterHorizontal->insertWidget(0, d->view);
    splitterHorizontal->insertWidget(1, d->preview);

	splitterHorizontal->setStretchFactor(0, 1);
	splitterHorizontal->setStretchFactor(1, 3);

    splitterVertical->insertWidget(0, splitterHorizontal);
    splitterVertical->insertWidget(1, d->selected);

    splitterVertical->setStretchFactor(0, 2);
    splitterVertical->setStretchFactor(1, 3);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->setDirection(QBoxLayout::RightToLeft);

    int btWidth = 60;
    int btHeight = 35;
    d->btOk->setFixedSize(btWidth, btWidth);
    d->btCancel->setFixedSize(btWidth, btWidth);
    buttonsLayout->addWidget(d->btOk);
    buttonsLayout->addWidget(d->btCancel);

    mainLayout->addWidget(splitterVertical);
    mainLayout->addLayout(buttonsLayout);

    this->setLayout(mainLayout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(d->view, SIGNAL(patientClicked(const medDataIndex&)), this, SLOT(onPatientSelected(const medDataIndex&)));
    connect(d->view, SIGNAL(seriesClicked(const medDataIndex&)), d->preview, SLOT(addSeriesItem(const medDataIndex&)));

    connect(d->btOk, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
    connect(d->btCancel, SIGNAL(clicked()), this, SLOT(onCancelButtonClicked()));

    // for the moment we just need patient and study
    for (int var = 2; var < d->proxy->columnCount(); ++var) {
        d->view->hideColumn(var);
    }
}

medImageSelectionWidget::~medImageSelectionWidget(void)
{
    delete d;

    d = NULL;
}

QSize medImageSelectionWidget::sizeHint(void) const
{
    const int width = QApplication::desktop()->screen(0)->width();
    const int height = QApplication::desktop()->screen(0)->width();

    int widthHint = (int) (width * 0.6);
    int heightHint = (int) (height * 0.6);

    return QSize(widthHint, heightHint);
}

void medImageSelectionWidget::clear()
{
}

void medImageSelectionWidget::paintEvent(QPaintEvent* paintEvent)
{
//    qDebug() << "medImageSelectionWidget Paint Event";
}

void medImageSelectionWidget::resizeEvent(QResizeEvent* resizeEvent)
{
//    qDebug() << "medImageSelectionWidget Resize Event";
}

void medImageSelectionWidget::onPatientSelected(const medDataIndex& patientId)
{
    d->preview->reset();

    medAbstractDbController* db =  medDataManager::instance()->controllerForDataSource(patientId.dataSourceId());
    if (db)
    {
        QList<medDataIndex> studies = db->studies(patientId);
        for (QList<medDataIndex>::const_iterator studyIt( studies.begin() ); studyIt != studies.end(); ++studyIt )
        {
            QList<medDataIndex> series = db->series(*studyIt);
            for (QList<medDataIndex>::const_iterator seriesIt( series.begin() ); seriesIt != series.end(); ++seriesIt )
            {
                d->preview->addSeriesItem(*seriesIt);
            }
        }
    }
}

void medImageSelectionWidget::onCancelButtonClicked()
{
//    qDebug() << "Cancel button clicked";
    reject();
}

void medImageSelectionWidget::onOkButtonClicked()
{
//    qDebug() << "OK button clicked";
    accept();
}

QList<medDataIndex> medImageSelectionWidget::getSelectedIndexes()
{
    return d->selected->getContainedIndexes();
}
