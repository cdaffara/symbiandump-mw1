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
* Description:  CCFActionPlugIn class implementation.
*
*/



#include <ecom/ecom.h>

#include "cfactionplugin.h"
#include "CFActionPluginImpl.h"
#include "cftrace.h"

EXPORT_C CCFActionPlugIn* CCFActionPlugIn::NewL( 
	const TUid& aImplementationUid )
    {
    FUNC_LOG;
    
    CCFActionPlugIn* self = CCFActionPlugIn::NewLC( aImplementationUid );
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CCFActionPlugIn* CCFActionPlugIn::NewLC(
	const TUid& aImplementationUid )
    {
    FUNC_LOG;
    
    CCFActionPlugIn* self =
        reinterpret_cast<CCFActionPlugIn*>(
            REComSession::CreateImplementationL(
                aImplementationUid,
                _FOFF( CCFActionPlugIn, iDtorKey ) ) );
    CleanupStack::PushL( self );
    
    return self;
    }

EXPORT_C CCFActionPlugIn::~CCFActionPlugIn()
    {
    FUNC_LOG;
    
    delete iImpl;

    #ifndef _DEBUG
    REComSession::DestroyedImplementation( iDtorKey );
    #else
    if( iDtorKey != KNullUid )
        {
        REComSession::DestroyedImplementation( iDtorKey );
        }
    #endif
    }

EXPORT_C CCFActionPlugIn::CCFActionPlugIn()
    {
    FUNC_LOG;    
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFActionPlugIn::Extension
//-----------------------------------------------------------------------------
//
EXPORT_C TAny* CCFActionPlugIn::Extension( const TUid& /*aExtensionUid*/ ) const
    {
    FUNC_LOG;
    
    return NULL;
    }

//-----------------------------------------------------------------------------
// CCFActionPlugIn::AsyncExecutionCompleted
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFActionPlugIn::AsyncExecutionCompleted()
    {
    FUNC_LOG;
    
    if ( iImpl )
    	{
    	iImpl->AsyncExecutionCompleted();
    	}
    }

//------------------------------------------------------------------------------
// CCFActionPlugIn::PrepareExecutionL
//------------------------------------------------------------------------------
//
EXPORT_C void CCFActionPlugIn::PrepareExecutionL()
    {
    FUNC_LOG;
    
    // Empty
    }

//-----------------------------------------------------------------------------
// CCFActionPlugIn::FinishedExecution
//-----------------------------------------------------------------------------
//
EXPORT_C void CCFActionPlugIn::FinishedExecution()
    {
    FUNC_LOG;
    
    // Empty
    }

// End of File
