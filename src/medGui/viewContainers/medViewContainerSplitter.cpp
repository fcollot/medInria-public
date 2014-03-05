/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medViewContainerSplitter.h>

#include <QDebug>

#include <medDataManager.h>
#include <medViewContainer.h>
#include <medAbstractData.h>
#include <QUuid>

medViewContainerSplitter::medViewContainerSplitter(QWidget *parent)
{
    this->setOrientation(Qt::Horizontal);
    this->setHandleWidth(1);
    this->setContentsMargins(0,0,0,0);
    this->setOpaqueResize(false);
}

medViewContainerSplitter::~medViewContainerSplitter()
{
    this->setParent(NULL);
}

medViewContainer * medViewContainerSplitter::hSplit(medViewContainer *sender)
{
    return this->split(sender, Qt::AlignBottom);
}

medViewContainer * medViewContainerSplitter::vSplit(medViewContainer *sender)
{
    return this->split(sender, Qt::AlignRight);
}

medViewContainer *medViewContainerSplitter::split(medViewContainer *sender, Qt::AlignmentFlag alignement)
{
    if(!sender)
        return NULL;

    int index = this->indexOf(sender);
    int newSize = this->sizes()[index] / 2;

    medViewContainer *newContainer = new medViewContainer;

    switch(alignement)
    {
    case Qt::AlignLeft:
        if(this->orientation() == Qt::Horizontal)
        {
            this->insertViewContainer(index, newContainer);
            this->recomputeSizes(index + 1, index, newSize);
        }
        else
            this->insertNestedSplitter(index, newContainer, sender);
        break;
    case Qt::AlignBottom:
        if(this->orientation() == Qt::Vertical)
        {
            this->insertViewContainer(index + 1, newContainer);
            this->recomputeSizes(index, index + 1, newSize);
        }
        else
            this->insertNestedSplitter(index, sender, newContainer);
        break;
    case Qt::AlignRight:
        if(this->orientation() == Qt::Horizontal)
        {
            this->insertViewContainer(index + 1, newContainer);
            this->recomputeSizes(index, index + 1, newSize);
        }
        else
            this->insertNestedSplitter(index, sender, newContainer);
        break;
    case Qt::AlignTop:
        if(this->orientation() == Qt::Vertical)
        {
            this->insertViewContainer(index, newContainer);
            this->recomputeSizes(index + 1, index, newSize);
        }
        else
            this->insertNestedSplitter(index, newContainer, sender);
        break;
    }

    return newContainer;
}


medViewContainer * medViewContainerSplitter::hSplit()
{
    return this->split(Qt::AlignBottom);
}

medViewContainer * medViewContainerSplitter::vSplit()
{
    return this->split(Qt::AlignRight);
}

medViewContainer *medViewContainerSplitter::split(Qt::AlignmentFlag alignement)
{
    medViewContainer* sender = dynamic_cast<medViewContainer*>(this->sender());
    return this->split(sender, alignement);
}

void medViewContainerSplitter::split(medDataIndex index, Qt::AlignmentFlag alignement)
{
    medViewContainer *newContainer = this->split(alignement);
    newContainer->addData(medDataManager::instance()->data(index));
}

void medViewContainerSplitter::checkIfStillDeserveToLive()
{
    if(this->count() == 0)
        this->~medViewContainerSplitter();
}

void medViewContainerSplitter::insertViewContainer(int index, medViewContainer *container)
{
    connect(container, SIGNAL(hSplitRequest()), this, SLOT(hSplit()));
    connect(container, SIGNAL(vSplitRequest()), this, SLOT(vSplit()));
    connect(container, SIGNAL(splitRequest(medDataIndex, Qt::AlignmentFlag)),
            this, SLOT(split(medDataIndex, Qt::AlignmentFlag)));
    connect(container, SIGNAL(destroyed()), this, SLOT(checkIfStillDeserveToLive()));
    connect(container, SIGNAL(destroyed()), this, SIGNAL(containerRemoved()));
    container->setContainerParent(this);

    emit newContainer(container->uuid());

    this->insertWidget(index, container);
    this->setCollapsible(index, false);
}

void medViewContainerSplitter::addViewContainer(medViewContainer *container)
{   
    int newSize = 0;
    if(this->count() > 0)
        newSize = this->sizes()[this->count() - 1] / 2;

    this->insertViewContainer(this->count(), container);
    if(this->count() > 1)
        this->recomputeSizes(this->count() - 2, this->count() - 1, newSize);
}

void medViewContainerSplitter::recomputeSizes(int requestIndex, int newIndex, int newSize)
{
    if(requestIndex < 0 || newIndex < 0)
        return;

    QList <int> newSizes = this->sizes();
    if(requestIndex >= newSizes.size() || newIndex >= newSizes.size())
        return;

    newSizes.replace(requestIndex, newSize);
    newSizes.replace(newIndex, newSize);
    this->setSizes(newSizes);
}

void medViewContainerSplitter::insertNestedSplitter(int index,
                                                 medViewContainer *oldContainer,
                                                 medViewContainer *newContainer)
{
    Qt::Orientation ori = Qt::Vertical;
    if(this->orientation() == Qt::Vertical)
            ori = Qt::Horizontal;

    QList<int> savedSizes = this->sizes();
    oldContainer->disconnect(this);
    medViewContainerSplitter *splitter = new medViewContainerSplitter;
    splitter->setOrientation(ori);
    connect(splitter, SIGNAL(newContainer(QUuid)), this, SIGNAL(newContainer(QUuid)));
    connect(splitter, SIGNAL(containerRemoved()), this, SIGNAL(containerRemoved()));
    connect(splitter, SIGNAL(destroyed()), this, SLOT(checkIfStillDeserveToLive()));
    splitter->addViewContainer(oldContainer);
    splitter->addViewContainer(newContainer);
    this->insertWidget(index, splitter);
    this->setCollapsible(index, false);
    this->setSizes(savedSizes);

    // resize nested container because QVtkWidget2 is automaticlly resize to fit the its view
    // (given the fixed width/height of the splitter) when it is added to the splitter.
    int newSize = 0;
    if(splitter->orientation() == Qt::Vertical)
        newSize = splitter->width() / 2;
    else
        newSize = splitter->height() / 2;
    splitter->recomputeSizes(0, 1, newSize);

}
