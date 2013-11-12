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
* Description:  CFContextSourcePlugIn implementation.
*
*/



#include <ecom/ecom.h>
#include <cfcontextsourceplugin.h>
#include <cfcontextinterface.h>
#include <cfcontextsubscriptionlistener.h>

// CONSTANTS

#ifdef _DEBUG
_LIT( KPanicCat, "CntxSrcPlugIn" );

enum TPanicReason
    {
    ENoPanic,  // ENone - ENone caused a trouble when including w32std.h
    EInvalidInitParam,
    EInvalidContextManager,
    EInvalidFs
    };
    
LOCAL_C void Panic( TPanicReason aReason )
    {
    User::Panic( KPanicCat, aReason );
    }
#endif

// MEMBER FUNCTIONS

EXPORT_C CCFContextSourcePlugIn* CCFContextSourcePlugIn::NewL(
    const TUid& aImplementationUid,
    TContextSourceInitParams* aParams )
    {
    CCFContextSourcePlugIn* self =
        CCFContextSourcePlugIn::NewLC( aImplementationUid, aParams );
    CleanupStack::Pop( self );
    
    return self;
    }

EXPORT_C CCFContextSourcePlugIn* CCFContextSourcePlugIn::NewLC(
    const TUid& aImplementationUid,
    TContextSourceInitParams* aParams )
    {
    __ASSERT_DEBUG( aParams, Panic( EInvalidInitParam ) );

    CCFContextSourcePlugIn* self =
        reinterpret_cast<CCFContextSourcePlugIn*>(
            REComSession::CreateImplementationL(
                aImplementationUid,
                _FOFF( CCFContextSourcePlugIn, iDtorKey ),
                aParams ) );

    CleanupStack::PushL( self );
    
    return self;
    }

EXPORT_C CCFContextSourcePlugIn::~CCFContextSourcePlugIn()
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

EXPORT_C CCFContextSourcePlugIn::CCFContextSourcePlugIn(
    TContextSourceInitParams* aParams ):
    iCF( aParams->iCF )
    {
    }
    
//-----------------------------------------------------------------------------
// CCFContextSourcePlugIn::Extension
//-----------------------------------------------------------------------------
//
EXPORT_C TAny* CCFContextSourcePlugIn::Extension(
    const TUid& /*aExtensionUid*/ ) const
    {
    return NULL;
    }

// End of File
