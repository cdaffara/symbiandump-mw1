/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionPlugInImpl class implementation.
*
*/



#include "CFActionPluginImpl.h"
#include "cfactionplugin.h"
#include "cftrace.h"
#include "cfactionpluginthread.h"

// MEMBER FUNCTIONS

CCFActionPlugInImpl* CCFActionPlugInImpl::NewL(
    CCFActionPlugIn* aPlugIn,
    CCFActionPlugInThread* aPlugInThread )
    {
    FUNC_LOG;
    
    CCFActionPlugInImpl* self = CCFActionPlugInImpl::NewLC( aPlugIn,
    		aPlugInThread );
    CleanupStack::Pop( self );
    
    return self;
    }

CCFActionPlugInImpl* CCFActionPlugInImpl::NewLC(
    CCFActionPlugIn* aPlugIn,
    CCFActionPlugInThread* aPlugInThread )
    {
    FUNC_LOG;
    
    CCFActionPlugInImpl* self = new( ELeave ) CCFActionPlugInImpl( aPlugIn,
    		aPlugInThread );
    CleanupStack::PushL( self );
    self->ConstructL();
     
    return self;
    }

CCFActionPlugInImpl::~CCFActionPlugInImpl()
    {
    FUNC_LOG;

    }

CCFActionPlugInImpl::CCFActionPlugInImpl( CCFActionPlugIn* aPlugIn,
	CCFActionPlugInThread* aPlugInThread):
    iPlugInThread( aPlugInThread ),
    iPlugIn( aPlugIn )
    {
    FUNC_LOG;
    }

void CCFActionPlugInImpl::ConstructL()
    {
    FUNC_LOG;
    }
    
// METHODS

void CCFActionPlugInImpl::AsyncExecutionCompleted()
	{
	FUNC_LOG;
	
	iPlugInThread->AsyncExecutionCompleted();
	}
