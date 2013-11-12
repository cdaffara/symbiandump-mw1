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
* Description:  description
*
*/


// SYSTEM INCLUDES
#include <cfcontextinterface.h>

// USER INCLUDES
#include "contextsource_v2.h"
#include "contextsourcedef_v2.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CContextSource_V2::CContextSource_V2( TContextSourceInitParams* aParams ):
    CCFContextSourcePlugIn( aParams )
    {

    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CContextSource_V2::ConstructL( )
    {
    iContext = CCFContextObject::NewL();
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CContextSource_V2* CContextSource_V2::NewL( TContextSourceInitParams* aParams )
    {
    CContextSource_V2* self = CContextSource_V2::NewLC( aParams );
    CleanupStack::Pop ( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CContextSource_V2* CContextSource_V2::NewLC( TContextSourceInitParams* aParams )
    {
    CContextSource_V2* self = new( ELeave ) CContextSource_V2( aParams );
    CleanupStack::PushL ( self );
    self->ConstructL ( );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CContextSource_V2::~CContextSource_V2( )
    {
    // Publish context that the context source V2 is not yet installed
    TRAP_IGNORE( UpdateContextObjectL(
        KContextSource_V2_Source,
        KContextSource_V2_Type,
        TPtrC( KContextSource_V2_Type_Values[EContextSource_V2_ValueNo] ) ) );
    RThread client;
    iCF.PublishContext( *iContext, client );
    client.Close();
    
    delete iContext;
    }

//------------------------------------------------------------------------------
// CContextSource_V2::Extension
//------------------------------------------------------------------------------
//
TAny* CContextSource_V2::Extension( const TUid& /*aExtensionUid */) const
    {
    return NULL;
    }

//------------------------------------------------------------------------------
// CContextSource_V2::HandleSettingL
//------------------------------------------------------------------------------
//
void CContextSource_V2::HandleSettingL(
    CCFContextSourceSettingArray* /*aSettingList */)
    {
    
    }

//------------------------------------------------------------------------------
// CContextSource_V2::DefineContextsL
//------------------------------------------------------------------------------
//
void CContextSource_V2::DefineContextsL()
    {
    _LIT_SECURITY_POLICY_PASS( passSec );
    
    TInt err = iCF.DefineContext( KContextSource_V2_Source,
        KContextSource_V2_Type, passSec, passSec, *this );
    }

//------------------------------------------------------------------------------
// CContextSource_V2::InitializeL
//------------------------------------------------------------------------------
//
void CContextSource_V2::InitializeL()
    {
    iContext->SetSourceL( KContextSource_V2_Source );
    iContext->SetTypeL( KContextSource_V2_Type );
    
    // Publish context that context source V2 is installed
    iContext->SetValueL( TPtrC(
        KContextSource_V2_Type_Values[EContextSource_V2_ValueYes] ) );
    RThread thread;
    TInt err = iCF.PublishContext( *iContext, thread );
    thread.Close();
    }

//------------------------------------------------------------------------------
// CContextSource_V2::Subscribers
//------------------------------------------------------------------------------
//
void CContextSource_V2::Subscribers( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/ )
    {
    
    }

//------------------------------------------------------------------------------
// CContextSource_V2::NoSubscribers
//------------------------------------------------------------------------------
//
void CContextSource_V2::NoSubscribers( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType */)
    {
    
    }

//------------------------------------------------------------------------------
// CContextSource_V2::HandleCommand
//------------------------------------------------------------------------------
//
void CContextSource_V2::HandleCommand(
    const CCFContextSourceCommand& /*aCommand*/ )
    {
    // Nothing to do.
    }

//------------------------------------------------------------------------------
// CContextSource_V2::CheckValidity
//------------------------------------------------------------------------------
//
TInt CContextSource_V2::CheckValidity( RThread& /*aClientThread*/,
    const CCFContextSourceSettingArray& /*aSettingList */)
    {
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CContextSource_V2::UpdateSettingsL
//------------------------------------------------------------------------------
//
void CContextSource_V2::UpdateSettingsL(
    CCFContextSourceSettingArray* /*aSettingList */)
    {
    
    }

//------------------------------------------------------------------------------
// CContextSource_V2::UpdateContextObjectL
//------------------------------------------------------------------------------
//
void CContextSource_V2::UpdateContextObjectL( const TDesC& aSource,
    const TDesC& aType, const TDesC& aValue )
    {
    User::LeaveIfNull( iContext );
    
    iContext->SetSourceL( aSource );
    iContext->SetTypeL( aType );
    iContext->SetValueL( aValue );
    }

// End of file
