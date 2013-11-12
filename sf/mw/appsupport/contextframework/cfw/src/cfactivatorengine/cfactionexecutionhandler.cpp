/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionExecutionHandler class implementation.
*
*/



#include "cfactionexecutionhandler.h"
#include "cfactionpluginthread.h"
#include "cftrace.h"

CCFActionExecutionHandler::CCFActionExecutionHandler(
	CCFActionPlugInThread* aActionPluginThread ) :
	CActive( EPriorityStandard ),
	iActionPlugInThread( aActionPluginThread )
	{
    FUNC_LOG;

	}

CCFActionExecutionHandler* CCFActionExecutionHandler::NewLC(
	CCFActionPlugInThread* aActionPluginThread )
	{
    FUNC_LOG;

	CCFActionExecutionHandler* self = new ( ELeave ) CCFActionExecutionHandler(
		aActionPluginThread );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

CCFActionExecutionHandler* CCFActionExecutionHandler::NewL(
	CCFActionPlugInThread* aActionPluginThread )
	{
    FUNC_LOG;

	CCFActionExecutionHandler* self = CCFActionExecutionHandler::NewLC(
		aActionPluginThread );
	CleanupStack::Pop(); // self;
	return self;
	}

void CCFActionExecutionHandler::ConstructL()
	{
    FUNC_LOG;

	CActiveScheduler::Add( this );				// Add to scheduler
	}

CCFActionExecutionHandler::~CCFActionExecutionHandler()
	{
    FUNC_LOG;

	Cancel(); // Cancel any request, if outstanding
	}

void CCFActionExecutionHandler::DoCancel()
	{
    FUNC_LOG;

    iActionPlugInThread->CancelActionsWait();
	}

void CCFActionExecutionHandler::StartAsyncActionsWait()
	{
    FUNC_LOG;

	Cancel();  // Cancel any request, just to be sure

	iActionPlugInThread->WaitActions( iStatus );
	SetActive();
	}

void CCFActionExecutionHandler::RunL()
	{
    FUNC_LOG;

    TInt status = iStatus.Int();
    switch( status )
        {
        case KErrNone:
            {
            // Execute actions from queue
            iActionPlugInThread->NewActionsAddedToQueueL();
            break;
            }
        case KErrReleasePlugin:
            {
            // Delete plug-in implementation
            iActionPlugInThread->DeletePlugin();
            break;
            }
        case KErrReloadPlugin:
            {
            // Reload plug-in implementation
            iActionPlugInThread->ConstructPluginL();
            break;
            }
        default:
            {
            // Nothing to do here.
            break;
            }
        }
	}

TInt CCFActionExecutionHandler::RunError( TInt aError )
	{
    FUNC_LOG;

	return KErrNone;
	}
