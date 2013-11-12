/*
* Copyright (c) 2009-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   CDisplayServiceLight class implementation.
*
*/


// SYSTEM INCLUDES
#include <cfcontextobject.h>
#include <cfcontextinterface.h>

// USER INCLUDES
#include "displayservicelight.h"
#include "displaysourcecontextdef.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CDisplayServiceLight::CDisplayServiceLight( MCFContextInterface& aCF ):
    CDisplayServiceBase( CDisplayServiceBase::EDisplayLightStatus, aCF )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CDisplayServiceLight::ConstructL()
    {
    FUNC_LOG;

    // Call base class
    CDisplayServiceBase::ConstructL();
    
    // Initialize context object
    iContext->SetSourceL( KDisplaySource );
    iContext->SetTypeL( KDisplayStatusType );
    
    // Define context:
    // Device: Display.Status.Light
    TInt err = iCF.DefineContext(
        KDisplaySource, KDisplayStatusType, KDisplayStatusTypeSec );
    User::LeaveIfError( err );
    
    // Initialize light client
    iLight = CHWRMLight::NewL( this );
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CDisplayServiceLight* CDisplayServiceLight::NewL( MCFContextInterface& aCF )
    {
    FUNC_LOG;

    CDisplayServiceLight* self = CDisplayServiceLight::NewLC( aCF );
    CleanupStack::Pop( self );
    
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CDisplayServiceLight* CDisplayServiceLight::NewLC( MCFContextInterface& aCF )
    {
    FUNC_LOG;

    CDisplayServiceLight* self = new ( ELeave ) CDisplayServiceLight( aCF );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CDisplayServiceLight::~CDisplayServiceLight()
    {
    FUNC_LOG;
    
    delete iLight;
    }

//------------------------------------------------------------------------------
// CDisplayServiceLight::StartL
//------------------------------------------------------------------------------
//
void CDisplayServiceLight::StartL()
    {
    FUNC_LOG;

    // Get current display light status value and publish it
    PublishContext( iLight->LightStatus( CHWRMLight::EPrimaryDisplay ) );
    }

//------------------------------------------------------------------------------
// CDisplayServiceLight::Stop
//------------------------------------------------------------------------------
//
void CDisplayServiceLight::Stop()
    {
    FUNC_LOG;
    
    // Nothing to do here.
    }

//----------------------------------------------------------------------------
// CDisplayServiceLight::LightStatusChanged
//----------------------------------------------------------------------------
//
void CDisplayServiceLight::LightStatusChanged(
    TInt aTarget,
    CHWRMLight::TLightStatus aStatus )
    {
    FUNC_LOG;

    if ( aTarget & CHWRMLight::EPrimaryDisplay )
        {
        PublishContext( aStatus );
        }
    }

//------------------------------------------------------------------------------
// CDisplayServiceLight::PublishContext
//------------------------------------------------------------------------------
//
void CDisplayServiceLight::PublishContext( CHWRMLight::TLightStatus aStatus )
    {
    FUNC_LOG;
    
    INFO_1( "Primary display light status = %d", aStatus );

    if ( aStatus >= 0 &&
         aStatus < KDisplayStatusTypeValueCount &&
         aStatus != iCurrentStatus )
        {
        // Store new status
        iCurrentStatus = aStatus;
        
        // Publish context, ignore error values
        TRAP_IGNORE( iContext->SetValueL(
            TPtrC( KDisplayStatusTypeValues[aStatus] ) ) );
        iCF.PublishContext( *iContext, iThread );
        }
    }

// End of file
