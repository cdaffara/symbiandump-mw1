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
* Description:  Base class for all homescreen widgets.
*
*/


#ifndef HSWIDGET_H
#define HSWIDGET_H

#include <hswidgetmodel_global.h>
#include <hbwidget.h>


class HsWidgetPrivate;
class IHsWidgetPreferenceService;

class HSWIDGETMODEL_EXPORT HsWidget : public HbWidget
{
    Q_OBJECT

public:

    enum StartResult { StartResultRunning, StartResultFinished, StartResultFaulted };
    enum StopResult { StopResultFinished, StopResultFinishing, StopResultFaulted, StopResultFaulting };
    enum SuspendResult { SuspendResultSuspended, SuspendResultFaulted, SuspendResultFaulting };
    enum ResumeResult { ResumeResultRunning, ResumeResultFaulted, ResumeResultFaulting };


    HsWidget(QGraphicsItem *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~HsWidget();

    IHsWidgetPreferenceService *widgetPreferenceService() const;

    void start();
    void stop();
    void suspend();
    void resume();

signals:

    void succeeded();
    void stopped();
    void faulted();

protected:

    
    virtual StartResult onStart() = 0;
    virtual StopResult onStop() = 0;
    virtual SuspendResult onSuspend();
    virtual ResumeResult onResume();

    void setFinished();
    void setFaulted();
    void setFaulting();

    /**
     * @copydoc QGraphicsWidget::mouseDoubleClickEvent()
     */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) { Q_UNUSED(event) }

    /**
     * @copydoc QGraphicsWidget::mouseMoveEvent()
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) { Q_UNUSED(event) }

    /**
     * @copydoc QGraphicsWidget::mousePressEvent()
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) { Q_UNUSED(event) }

    /**
     * @copydoc QGraphicsWidget::mouseReleaseEvent()
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) { Q_UNUSED(event) }

private:

    Q_DISABLE_COPY(HsWidget)

private:

    HsWidgetPrivate *mD;
    friend class HsWidgetPrivate;

};




#endif
