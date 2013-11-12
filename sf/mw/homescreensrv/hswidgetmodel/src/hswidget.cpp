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
#include <QGraphicsDropShadowEffect>

#include "hswidget.h"
#include "hswidget_p.h"
#include "ihswidgetpreferenceservice.h"

/*DEPRECATED
    \ingroup group_hswidgetmodel group_widget_api_candidates
    \class HsWidget
    \brief Base class for all home screen widgets.

    Widget base class declares automata for derived widget. It also overwrites
    mouse event functions to enable all mouse events.
*/

/*!
    \enum HsWidget::StartResult
    
    Return values for onStart() method.
*/

/*!
    \enum HsWidget::StopResult
    
    Return values for onSuspend() method.
*/
/*!
    \enum HsWidget::SuspendResult
    
    Return values for onSuspend() method.
*/

/*!
    \enum HsWidget::ResumeResult
    
    Return values for onResume() method.
*/


const char WIDGET_PREFERENCE_SERVICE_KEY[]="http://homescreen.nokia.com/runtimeservices/widgetpreferenceservice";


HsWidgetPrivate::HsWidgetPrivate(HsWidget* handle): 
    mPublic(handle),
    mState(WidgetStateConstructed)
{

}

HsWidgetPrivate::~HsWidgetPrivate()
{

}

void HsWidgetPrivate::start()
{
    switch(mState)
    {
    case WidgetStateConstructed:
    case WidgetStateFinished:
        break;

    default:
        return;
    }

    switch(mPublic->onStart())
    {
    case HsWidget::StartResultRunning:
        mState = WidgetStateRunning;
        break;

    case HsWidget::StartResultFinished:
        mState = WidgetStateFinished;
        emit mPublic->succeeded();
        break;

    case HsWidget::StartResultFaulted:
        mState = WidgetStateFinished;
        emit mPublic->faulted();
        break;
    }
}


void HsWidgetPrivate::stop()
{
    switch(mState)
    {
    case WidgetStateRunning:
    case WidgetStateSuspended:
        break;

    default:
        return;
    }

    switch(mPublic->onStop())
    {
    case HsWidget::StopResultFinished:
        mState = WidgetStateFinished;
        emit mPublic->stopped();
        break;

    case HsWidget::StopResultFinishing:
        mState = WidgetStateFinishing;
        break;

    case HsWidget::StopResultFaulted:
        mState = WidgetStateFinished;
        emit mPublic->faulted();
        break;

    case HsWidget::StopResultFaulting:
        mState = WidgetStateFaulting;
        break;
    }
}


void HsWidgetPrivate::suspend()
{
    switch(mState)
    {
    case WidgetStateRunning:
        break;

    default:
        return;
    }

    switch(mPublic->onSuspend())
    {
    case HsWidget::SuspendResultSuspended:
        mState = WidgetStateSuspended;
        break;

    case HsWidget::SuspendResultFaulted:
        mState = WidgetStateFinished;
        emit mPublic->faulted();
        break;

    case HsWidget::SuspendResultFaulting:
        mState = WidgetStateFaulting;
        break;
    }
}

void HsWidgetPrivate::resume()
{
    switch(mState)
    {
    case WidgetStateSuspended:
        break;

    default:
        return;
    }

    switch(mPublic->onResume())
    {
    case HsWidget::ResumeResultRunning:
        mState = WidgetStateRunning;
        break;

    case HsWidget::ResumeResultFaulted:
        mState = WidgetStateFinished;
        emit mPublic->faulted();
        break;

    case HsWidget::ResumeResultFaulting:
        mState = WidgetStateFaulting;
        break;
    }
}


void HsWidgetPrivate::setFinished()
{
    switch(mState)
    {
    case WidgetStateConstructed:
    case WidgetStateFinished:
        return;

    case WidgetStateRunning:
        mState = WidgetStateFinished;
        emit mPublic->succeeded();
        break;

    case WidgetStateSuspended:
        mState = WidgetStateFinished;
        emit mPublic->succeeded();
        break;

    case WidgetStateFinishing:
        mState = WidgetStateFinished;
        emit mPublic->stopped();
        break;

    case WidgetStateFaulting:
        mState = WidgetStateFinished;
        emit mPublic->faulted();
        break;
    }
}


void HsWidgetPrivate::setFaulted()
{
    switch(mState)
    {
    case WidgetStateConstructed:
    case WidgetStateFinished:
        return;

    default:
        break;
    }

    mState = WidgetStateFinished;
    emit mPublic->faulted();
}


void HsWidgetPrivate::setFaulting()
{
    switch(mState)
    {
    case WidgetStateConstructed:
    case WidgetStateFinished:
    case WidgetStateFaulting:
        return;

    default:
        break;
    }

    mState = WidgetStateFaulting;
}
/*!
    Constructs a widget which is a child of \a parent, with widget flags set to \a flags.
    
*/
HsWidget::HsWidget(QGraphicsItem *parent, Qt::WindowFlags flags)
    : HbWidget(parent, flags),mD(new HsWidgetPrivate(this))
{   
}

/*!
    Destructor
    
*/
HsWidget::~HsWidget()
{
    delete mD;
}
/*!
    Convenience method that returns the widget preference service from dynamically
    added meta object property.
*/

IHsWidgetPreferenceService* HsWidget::widgetPreferenceService() const
{
    return property(WIDGET_PREFERENCE_SERVICE_KEY).
        value<IHsWidgetPreferenceService*>();
}

/*!
    Widget runtime starts widget
*/
void HsWidget::start()
{
    mD->start();
}

/*!
    Widget runtime stops widget
*/
void HsWidget::stop()
{
    mD->stop();
}

/*!
    Widget runtime suspends widget
*/
void HsWidget::suspend()
{
    mD->suspend();
}

/*!
    Widget runtime resumes widget
*/
void HsWidget::resume()
{
    mD->resume();
}

/*!
    \fn void HsWidget::succeeded()
  
    Emitted after this widget has succesfully done its job
*/

/*!
    \fn void HsWidget::stopped()
    
    Emitted after this widget has stopped
*/

/*!
    \fn void HsWidget::faulted()
  
    Emitted after this widget has faulted
*/

/*!
    \fn virtual StartResult HsWidget::onStart() = 0
    
    WIdget can now e.g restore preferences
*/

/*!
    \fn virtual StopResult HsWidget::onStop() = 0
    
    Release resources. Widget will be destroyed.
*/

/*!
    Called by the widget runtime. Suspends this widget.
*/
HsWidget::SuspendResult HsWidget::onSuspend()
{
    return SuspendResultSuspended;
}

/*!
    Called by the widget runtime. Resumes this widget.
*/
HsWidget::ResumeResult HsWidget::onResume()
{
    return ResumeResultRunning;
}

/*!
    Indicates to the widget runtime that this widget should
    transition to the Finished state.
*/
void HsWidget::setFinished()
{
    mD->setFinished();
}

/*!
    Indicates to the widget runtime that this widget should
    transition to the Faulted state.
*/
void HsWidget::setFaulted()
{
    mD->setFaulted();
}

/*!
    Indicates to the widget runtime that this widget should
    transition to the Faulting state.
*/
void HsWidget::setFaulting()
{
    mD->setFaulting();
}




