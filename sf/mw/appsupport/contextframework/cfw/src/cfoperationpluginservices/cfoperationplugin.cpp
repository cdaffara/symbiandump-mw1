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
* Description:  CCFOperationPlugIn implementation.
*
*/



#include <ecom/ecom.h>

#include "cfoperationplugin.h"
#include "cfoperationservices.h"

// CONSTANTS

//#ifdef _DEBUG
//_LIT( KPanicCat, "CfOperationPlugIn" );
//
//enum TPanicReason
//    {
//    ENoPanic,  // ENone - ENone caused a trouble when including w32std.h
//    EInvalidInitParam,
//    EInvalidContextManager,
//    EInvalidFs
//    };
//    
//LOCAL_C void Panic( TPanicReason aReason )
//    {
//    User::Panic( KPanicCat, aReason );
//    }
//#endif

// MEMBER FUNCTIONS

EXPORT_C CCFOperationPlugIn* CCFOperationPlugIn::NewL(
    const TUid& aImplementationUid,
    TOperationPluginInitParams* aParams )
    {
    CCFOperationPlugIn* self =
        CCFOperationPlugIn::NewLC( aImplementationUid, aParams );
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CCFOperationPlugIn* CCFOperationPlugIn::NewLC(
    const TUid& aImplementationUid,
    TOperationPluginInitParams* aParams )
    {
    //__ASSERT_DEBUG( aParams, Panic( EInvalidInitParam ) );

    CCFOperationPlugIn* self =
        reinterpret_cast<CCFOperationPlugIn*>(
            REComSession::CreateImplementationL(
                aImplementationUid,
                _FOFF( CCFOperationPlugIn, iDtorKey ),
                aParams ) );

    CleanupStack::PushL( self );
    
    return self;
    }

EXPORT_C CCFOperationPlugIn::~CCFOperationPlugIn()
    {
    #ifndef _DEBUG
    REComSession::DestroyedImplementation( iDtorKey );
    #else
    if( iDtorKey != KNullUid )
        {
        REComSession::DestroyedImplementation( iDtorKey );
        }
    #endif
    }

EXPORT_C CCFOperationPlugIn::CCFOperationPlugIn(
    TOperationPluginInitParams* /*aParams*/ )
    {
    }
    
//-----------------------------------------------------------------------------
// CCFOperationPlugIn::Extension
//-----------------------------------------------------------------------------
//
EXPORT_C TAny* CCFOperationPlugIn::Extension(
    const TUid& /*aExtensionUid*/ ) const
    {
    return NULL;
    }

// End of File
