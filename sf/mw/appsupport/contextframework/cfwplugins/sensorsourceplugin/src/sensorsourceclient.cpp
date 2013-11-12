/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSensorSourceClient class implementation.
 *
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "sensorsourceclient.h"
#include "sensortrace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CSensorSourceClient::CSensorSourceClient( const TSecureId& aSid, TInt aScriptId ):
    iSid( aSid ),
    iScriptId( aScriptId )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CSensorSourceClient::ConstructL( const TDesC& aFeatureId )
    {
    FUNC_LOG;
    
    iFeatureIdDesc = aFeatureId.AllocL();
    iFeatureId.Set( *iFeatureIdDesc );
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CSensorSourceClient* CSensorSourceClient::NewL( const TSecureId& aSid,
    TInt aScriptId,
    const TDesC& aFeatureId )
    {
    FUNC_LOG;

    CSensorSourceClient* self = CSensorSourceClient::NewLC(
        aSid, aScriptId, aFeatureId );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CSensorSourceClient* CSensorSourceClient::NewLC( const TSecureId& aSid,
    TInt aScriptId,
    const TDesC& aFeatureId )
    {
    FUNC_LOG;

    CSensorSourceClient* self = new( ELeave ) CSensorSourceClient( aSid, aScriptId );
    CleanupStack::PushL( self );
    self->ConstructL( aFeatureId );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CSensorSourceClient::~CSensorSourceClient( )
    {
    FUNC_LOG;
    
    delete iFeatureIdDesc;
    }

// End of file
