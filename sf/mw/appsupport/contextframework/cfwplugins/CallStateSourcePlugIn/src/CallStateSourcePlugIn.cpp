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
* Description:  CCallStateSourcePlugIn class implementation.
*
*/


#include <cfcontextobject.h>
#include <cfcontextinterface.h>

#include <ctsydomainpskeys.h>

#include "CallStateSourcePlugIn.h"
#include "CallStateSourceContextDef.h"
#include "PropertyListener.h"


// CONSTANTS
#ifdef _DEBUG
// Panic category
_LIT( KPanicCat, "CallStatSrc" );

// Panic codes
enum TPanicReason
    {
    EPlugInNotInitialized
    };

// Panic function
LOCAL_C void Panic( TInt aCode )
    {
    User::Panic( KPanicCat, aCode );
    }
#endif

CCallStateSourcePlugIn* CCallStateSourcePlugIn::NewL(
    TContextSourceInitParams* aParams )
    {
    CCallStateSourcePlugIn* self = CCallStateSourcePlugIn::NewLC( aParams );
    CleanupStack::Pop( self );

    return self;
    }

CCallStateSourcePlugIn* CCallStateSourcePlugIn::NewLC(
TContextSourceInitParams* aParams )
    {
    CCallStateSourcePlugIn* self =
        new( ELeave ) CCallStateSourcePlugIn( aParams );
    CleanupStack::PushL( self );

    return self;
    }

// Destructor
CCallStateSourcePlugIn::~CCallStateSourcePlugIn()
    {
    delete iCallStateProperty;
    delete iContext;
    }

CCallStateSourcePlugIn::CCallStateSourcePlugIn(
    TContextSourceInitParams* aParams ):
    CCFContextSourcePlugIn( aParams )
    {
    }

// METHODS

//-----------------------------------------------------------------------------
// CCallStateSourcePlugIn::HandleSettingL
//-----------------------------------------------------------------------------
//
void CCallStateSourcePlugIn::HandleSettingL(
    CCFContextSourceSettingArray* /*aSettingList*/ )
	{
	// Not called since we don't have settings
	}

//-----------------------------------------------------------------------------
// CCallStateSourcePlugIn::DefineContextsL
//-----------------------------------------------------------------------------
//
void CCallStateSourcePlugIn::DefineContextsL()
    {
    iCF.DefineContext( KCallStateSource, KCallStateType, KCallStateTypeSec );
    }

//-----------------------------------------------------------------------------
// CCallStateSourcePlugIn::DefineContextsL
//-----------------------------------------------------------------------------
//
void CCallStateSourcePlugIn::InitializeL()
    {
    // Initialize context
    iContext = CCFContextObject::NewL();

    // Initialize call state property listener
    iCallStateProperty = CPropertyListener::NewL(
        KPSUidCtsyCallInformation,
        KCTsyCallState,
        *this );

    iContext->SetSourceL( KCallStateSource );
    iContext->SetTypeL( KCallStateType );

    // Publish current context value
    PublishContextL();
    }

//-----------------------------------------------------------------------------
// CCallStateSourcePlugIn::HandlePropertyChangedL
//-----------------------------------------------------------------------------
//
void CCallStateSourcePlugIn::HandlePropertyChangedL()
    {
    // Publish current context value
    PublishContextL();
    }

//-----------------------------------------------------------------------------
// CCallStateSourcePlugIn::PublishContextL
//-----------------------------------------------------------------------------
//
void CCallStateSourcePlugIn::PublishContextL()
    {
    __ASSERT_DEBUG( iCallStateProperty, Panic( EPlugInNotInitialized ) );

    // Get current call state value
    TInt value = 0;
    TInt err = iCallStateProperty->Property().Get( value );
    if( err == KErrNone )
        {
        if( value >= 0 && value < KCallStateTypeValueCount  )
            {
            // Publish context, ignore error values
            RThread thread;
            iContext->SetValueL( TPtrC( KCallStateTypeValues[value] ) );
            iCF.PublishContext( *iContext, thread );
            thread.Close();
            }
        }
    }
