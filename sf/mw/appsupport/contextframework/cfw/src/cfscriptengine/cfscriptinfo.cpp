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
* Description:  CCFScriptInfo class implementation.
 *
*/


// SYSTEM INCLUDES

// USER INCLUDES
#include "cfscriptinfo.h"
#include "cftrace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCFScriptInfo::CCFScriptInfo( TInt aScriptId,
    const TUid& aOwnerUid,
    TInt aLength,
    MCFScriptOwner* aScriptOwner ):
    iScriptId( aScriptId ),
    iOwnerUid( aOwnerUid ),
    iLength( aLength ),
    iScriptOwner( aScriptOwner )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CCFScriptInfo::ConstructL( const TDesC& aName )
    {
    FUNC_LOG;
    
    iName = aName.AllocL();
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CCFScriptInfo* CCFScriptInfo::NewL( const TDesC& aName,
    TInt aScriptId,
    const TUid& aOwner,
    TInt aLength,
    MCFScriptOwner* aScriptOwner )
    {
    FUNC_LOG;

    CCFScriptInfo* self = CCFScriptInfo::NewLC(
        aName, aScriptId, aOwner, aLength, aScriptOwner );
    CleanupStack::Pop ( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CCFScriptInfo* CCFScriptInfo::NewLC( const TDesC& aName,
    TInt aScriptId,
    const TUid& aOwner,
    TInt aLength,
    MCFScriptOwner* aScriptOwner )
    {
    FUNC_LOG;

    CCFScriptInfo* self = new( ELeave ) CCFScriptInfo(
        aScriptId, aOwner, aLength, aScriptOwner );
    CleanupStack::PushL( self );
    self->ConstructL( aName );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFScriptInfo::~CCFScriptInfo( )
    {
    FUNC_LOG;
    
    delete iName;
    }

//------------------------------------------------------------------------------
// CCFScriptInfo::Name
//------------------------------------------------------------------------------
//
TPtrC CCFScriptInfo::Name() const
    {
    FUNC_LOG;
    
    TPtrC name( KNullDesC );
    if( iName )
        {
        name.Set( *iName );
        }
    return name;
    }

//------------------------------------------------------------------------------
// CCFScriptInfo::Id
//------------------------------------------------------------------------------
//
TInt CCFScriptInfo::Id() const
    {
    FUNC_LOG;
    
    return iScriptId;
    }

//------------------------------------------------------------------------------
// CCFScriptInfo::OwnerUid
//------------------------------------------------------------------------------
//
const TUid& CCFScriptInfo::OwnerUid() const
    {
    FUNC_LOG;
    
    return iOwnerUid;
    }

//------------------------------------------------------------------------------
// CCFScriptInfo::Length
//------------------------------------------------------------------------------
//
TInt CCFScriptInfo::Length() const
    {
    FUNC_LOG;
    
    return iLength;
    }

//------------------------------------------------------------------------------
// CCFScriptInfo::OwnerSession
//------------------------------------------------------------------------------
//
MCFScriptOwner* CCFScriptInfo::OwnerSession() const
    {
    FUNC_LOG;
    
    return iScriptOwner;
    }

//------------------------------------------------------------------------------
// CCFScriptInfo::SetOwnerSession
//------------------------------------------------------------------------------
//
void CCFScriptInfo::SetOwnerSession( MCFScriptOwner* aScriptOwner )
    {
    FUNC_LOG;
    
    iScriptOwner = aScriptOwner;
    }

// End of file
