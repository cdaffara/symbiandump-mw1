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
#include "contextsource_v1.h"
#include "contextsourcedef_v1.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CContextSource_V1::CContextSource_V1( TContextSourceInitParams* aParams ):
    CCFContextSourcePlugIn( aParams )
    {

    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CContextSource_V1::ConstructL( )
    {
    iContext = CCFContextObject::NewL();
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CContextSource_V1* CContextSource_V1::NewL( TContextSourceInitParams* aParams )
    {
    CContextSource_V1* self = CContextSource_V1::NewLC( aParams );
    CleanupStack::Pop ( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CContextSource_V1* CContextSource_V1::NewLC( TContextSourceInitParams* aParams )
    {
    CContextSource_V1* self = new( ELeave ) CContextSource_V1( aParams );
    CleanupStack::PushL ( self );
    self->ConstructL ( );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CContextSource_V1::~CContextSource_V1( )
    {
    // Publish context that the context source v1 is not yet installed
    TRAP_IGNORE( UpdateContextObjectL(
        KContextSource_V1_Source,
        KContextSource_V1_Type,
        TPtrC( KContextSource_V1_Type_Values[EContextSource_V1_ValueNo] ) ) );
    RThread client;
    iCF.PublishContext( *iContext, client );
    client.Close();
    
    delete iContext;
    }

//------------------------------------------------------------------------------
// CContextSource_V1::Extension
//------------------------------------------------------------------------------
//
TAny* CContextSource_V1::Extension( const TUid& aExtensionUid ) const
    {
    TAny* interface = NULL;
    if( aExtensionUid == KCFContextSourceInterfaceUid )
        {
        interface = ( MCFContextSource* )this;
        }
    return interface;
    }

//------------------------------------------------------------------------------
// CContextSource_V1::HandleSettingL
//------------------------------------------------------------------------------
//
void CContextSource_V1::HandleSettingL(
    CCFContextSourceSettingArray* /*aSettingList */)
    {
    
    }

//------------------------------------------------------------------------------
// CContextSource_V1::DefineContextsL
//------------------------------------------------------------------------------
//
void CContextSource_V1::DefineContextsL()
    {
    _LIT_SECURITY_POLICY_PASS( passSec );
    
    TInt err = iCF.DefineContext( KContextSource_V1_Source,
        KContextSource_V1_Type, passSec, passSec, *this );
    }

//------------------------------------------------------------------------------
// CContextSource_V1::InitializeL
//------------------------------------------------------------------------------
//
void CContextSource_V1::InitializeL()
    {
    iContext->SetSourceL( KContextSource_V1_Source );
    iContext->SetTypeL( KContextSource_V1_Type );
    
    // Publish context that context source v1 is installed
    iContext->SetValueL( TPtrC(
        KContextSource_V1_Type_Values[EContextSource_V1_ValueYes] ) );
    RThread thread;
    TInt err = iCF.PublishContext( *iContext, thread );
    thread.Close();
    }

//------------------------------------------------------------------------------
// CContextSource_V1::Subscribers
//------------------------------------------------------------------------------
//
void CContextSource_V1::Subscribers( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType*/ )
    {
    
    }

//------------------------------------------------------------------------------
// CContextSource_V1::NoSubscribers
//------------------------------------------------------------------------------
//
void CContextSource_V1::NoSubscribers( const TDesC& /*aContextSource*/,
    const TDesC& /*aContextType */)
    {
    
    }

//------------------------------------------------------------------------------
// CContextSource_V1::HandleCommand
//------------------------------------------------------------------------------
//
void CContextSource_V1::HandleCommand(
    const CCFContextSourceCommand& /*aCommand*/ )
    {
    // Nothing to do.
    }

//------------------------------------------------------------------------------
// CContextSource_V1::CheckValidity
//------------------------------------------------------------------------------
//
TInt CContextSource_V1::CheckValidity( RThread& /*aClientThread*/,
    const CCFContextSourceSettingArray& /*aSettingList */)
    {
    return KErrNone;
    }

//------------------------------------------------------------------------------
// CContextSource_V1::UpdateSettingsL
//------------------------------------------------------------------------------
//
void CContextSource_V1::UpdateSettingsL(
    CCFContextSourceSettingArray* /*aSettingList */)
    {
    
    }

//------------------------------------------------------------------------------
// CContextSource_V1::UpdateContextObjectL
//------------------------------------------------------------------------------
//
void CContextSource_V1::UpdateContextObjectL( const TDesC& aSource,
    const TDesC& aType, const TDesC& aValue )
    {
    User::LeaveIfNull( iContext );
    
    iContext->SetSourceL( aSource );
    iContext->SetTypeL( aType );
    iContext->SetValueL( aValue );
    }

// End of file
