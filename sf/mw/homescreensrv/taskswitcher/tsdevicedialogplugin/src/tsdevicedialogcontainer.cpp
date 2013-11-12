/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/

#include "tsdevicedialogcontainer.h"

#include <QAbstractListModel>

#include <HbDialog>
#include <HbMainWindow>
#include <HbLabel>
#include <HbFrameDrawer>
#include <HbFrameItem>
#include <HbStyleLoader>

#include <tspropertydefs.h>

#include "tstasksgrid.h"
#include "tstasksgriditem.h"

namespace
{
    const char KDocmlPath[] = ":/resource/layout.docml";
}

TsDeviceDialogContainer::TsDeviceDialogContainer(QAbstractListModel *model,
                                                 QObject *parent)
    :
    QObject(parent),
    mVisibilityPublisher(TsProperty::KTsPath),
    mDismissRequestSubscriber(QString("%1/%2").arg(TsProperty::KTsPath).arg(TsProperty::KDismissRequestPath))
{
    bool ok(true);
    mLoader.load(KDocmlPath, &ok);
    Q_ASSERT(ok);

    HbDialog *dialog =
        qobject_cast<HbDialog *>(mLoader.findWidget("tsdevicedialog"));
    TsTasksGrid *grid =
        qobject_cast<TsTasksGrid *>(mLoader.findWidget("taskgrid"));
    Q_ASSERT(dialog);
    Q_ASSERT(grid);
    
    bool cssLoaded = HbStyleLoader::registerFilePath(":/resource/hbdialog.css");
    Q_ASSERT(cssLoaded);
    {
        HbLabel *dialogHeading = qobject_cast<HbLabel *>(dialog->headingWidget());
        Q_ASSERT(dialogHeading);
        HbFrameDrawer *headingFrame = new HbFrameDrawer(QLatin1String("qtg_fr_popup_heading"), HbFrameDrawer::ThreePiecesHorizontal);
        dialogHeading->setBackgroundItem(new HbFrameItem(headingFrame));
    }

    grid->setEnabledAnimations(HbAbstractItemView::None);
    grid->setModel(model);

    changeOrientation(dialog->mainWindow()->orientation());
    switchViewOnModelChange();

    // needed because of Qt::QueuedConnection used below
    // @todo: check if we actually need queued connections
    qRegisterMetaType<QModelIndex>("QModelIndex");

    // connect the grid and model
    connect(grid,
            SIGNAL(activated(QModelIndex)),
            model,
            SLOT(openApplication(QModelIndex)));
    connect(grid,
            SIGNAL(activated(QModelIndex)),
            dialog,
            SLOT(close()));
    connect(grid,
            SIGNAL(deleteButtonClicked(QModelIndex)),
            model, SLOT(closeApplication(QModelIndex)),
            Qt::QueuedConnection);

    connect(dialog->mainWindow(),
            SIGNAL(orientationChanged(Qt::Orientation)),
            this,
            SLOT(changeOrientation(Qt::Orientation)));
    connect(dialog,
            SIGNAL(aboutToClose()),
            this,
            SIGNAL(deviceDialogClosed()));

    // switch between grid and "no items" label when model is updated
    connect(model, SIGNAL(modelReset()), this, SLOT(switchViewOnModelChange()));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(switchViewOnModelChange()));
    connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(switchViewOnModelChange()));

    connect(this,
            SIGNAL(deviceDialogClosed()),
            this,
            SLOT(notifyDialogClosed()));
    mVisibilityPublisher.setValue(TsProperty::KVisibilityPath,
                                  static_cast<int>(true));
    mVisibilityPublisher.sync();

    connect(&mDismissRequestSubscriber,
            SIGNAL(contentsChanged()),
            this,
            SLOT(handleDismissRequest()));
}

TsDeviceDialogContainer::~TsDeviceDialogContainer()
{
    delete deviceDialogWidget();
}

bool TsDeviceDialogContainer::setDeviceDialogParameters(
    const QVariantMap &parameters)
{
    Q_UNUSED(parameters);
    return false;
}

int TsDeviceDialogContainer::deviceDialogError() const
{
    return 0;
}

void TsDeviceDialogContainer::closeDeviceDialog(bool byClient)
{
    Q_UNUSED(byClient)
    // @todo: should there be hide calls? deviceDialogWidget->hide();
    emit deviceDialogClosed();
}

HbPopup *TsDeviceDialogContainer::deviceDialogWidget() const
{
    HbDialog *widget =
        qobject_cast<HbDialog *>(mLoader.findWidget("tsdevicedialog"));
    Q_ASSERT(widget);
    return widget;
}

QObject *TsDeviceDialogContainer::signalSender() const
{
    return const_cast<TsDeviceDialogContainer *>(this);
}

void TsDeviceDialogContainer::changeOrientation(Qt::Orientation orientation)
{
    bool ok(true);
    if (orientation == Qt::Horizontal) {
        mLoader.load(KDocmlPath, "landscape", &ok);
    } else {
        mLoader.load(KDocmlPath, "portrait", &ok);
    }
    Q_ASSERT(ok);
}

void TsDeviceDialogContainer::notifyDialogClosed()
{
    mVisibilityPublisher.setValue(TsProperty::KVisibilityPath,
                                  static_cast<int>(false));
    mVisibilityPublisher.sync();
}

void TsDeviceDialogContainer::switchViewOnModelChange()
{
    TsTasksGrid *grid =
        qobject_cast<TsTasksGrid *>(mLoader.findWidget("taskgrid"));
    HbWidget *noItemsWidget =
        qobject_cast<HbWidget *>(mLoader.findWidget("noitemswidget"));
    Q_ASSERT(grid);
    Q_ASSERT(noItemsWidget);

    if (grid->model() && grid->model()->rowCount()) {
        noItemsWidget->hide();
        grid->show();
    } else {
        noItemsWidget->show();
        grid->hide();
    }
}

void TsDeviceDialogContainer::handleDismissRequest()
{
    if (mDismissRequestSubscriber.value().toBool()) {
        emit deviceDialogClosed();
    }
}
