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
* Description:  COrientationActionPlugIn class implementation.
*
*/


#include <cfactionindication.h>

#include "orientationactionplugin.h"
#include "trace.h"
#include "orientationaction.h"
#include "orientationactiondef.h"

// MEMBER FUNCTIONS

COrientationActionPlugIn* COrientationActionPlugIn::NewL()
    {
    FUNC_LOG;
    
    COrientationActionPlugIn* self = COrientationActionPlugIn::NewLC();
    CleanupStack::Pop( self );
    
    return self;
    }
  
COrientationActionPlugIn* COrientationActionPlugIn::NewLC()
    {
    FUNC_LOG;
    
    COrientationActionPlugIn* self = new( ELeave ) COrientationActionPlugIn;
    CleanupStack::PushL( self );
    
    return self;
    }
  
// Destructor
COrientationActionPlugIn::~COrientationActionPlugIn()
    {
    FUNC_LOG;
    
    delete iOrientationAction;
    }

COrientationActionPlugIn::COrientationActionPlugIn()
    {
    FUNC_LOG;
    
    // Nothing to do
    }

// METHODS

//-----------------------------------------------------------------------------
// COrientationActionPlugIn::InitializeL
//-----------------------------------------------------------------------------
//
void COrientationActionPlugIn::InitializeL()
    {
    FUNC_LOG;
    
    iOrientationAction = COrientationAction::NewL();
    }

//-----------------------------------------------------------------------------
// COrientationActionPlugIn::ExecuteL
//-----------------------------------------------------------------------------
//
CCFActionPlugIn::TExecutionTime COrientationActionPlugIn::ExecuteL(
    CCFActionIndication* aActionIndication )
    {
    FUNC_LOG;
    
    CCFActionPlugIn::TExecutionTime time = CCFActionPlugIn::ENone;
    
    const RKeyValueArray& params = aActionIndication->Parameters();
    // Check for action ID: Orientation
    if( aActionIndication->Identifier().CompareF(
        KOrientationAction ) == KErrNone )
        {
        time = HandleOrientationActionL( *aActionIndication );
        }
    else
        {
        ERROR_1( KErrArgument, "Unknown action id: [%S]",
            &aActionIndication->Identifier() );
        }
    
    return time;
    }

//-----------------------------------------------------------------------------
// COrientationActionPlugIn::GetActionsL
//-----------------------------------------------------------------------------
//
void COrientationActionPlugIn::GetActionsL( CDesCArray& aActionList ) const
    {
    FUNC_LOG;
    
    aActionList.AppendL( KOrientationAction );
    }

//-----------------------------------------------------------------------------
// COrientationActionPlugIn::SecurityPolicy
//-----------------------------------------------------------------------------
//
const TSecurityPolicy& COrientationActionPlugIn::SecurityPolicy() const
    {
    FUNC_LOG;
    
    return KOrientationActionSec;
    }

//------------------------------------------------------------------------------
// COrientationActionPlugIn::HandleOrientationActionL
//------------------------------------------------------------------------------
//
CCFActionPlugIn::TExecutionTime COrientationActionPlugIn::HandleOrientationActionL(
    const CCFActionIndication& aAction )
    {
    FUNC_LOG;
    
    const RKeyValueArray& params = aAction.Parameters();
    TPtrC mode( KNullDesC );
    TPtrC key( KNullDesC );
    TPtrC value( KNullDesC );
    TInt count = params.Count();
    for( TInt i = 0; i < count; i++ )
        {
        key.Set( params[i]->Key() );
        value.Set( params[i]->Value() );
        
        // Mode
        if( key.CompareF( KOrientationParamMode ) == KErrNone )
            {
            mode.Set( value );
            }
        }

    // Check if we have a correct mode
    if( mode.Length() )
        {
        // Return value can be ignored
        TInt err = iOrientationAction->Execute( mode );
        if( err == KErrNone )
            {
            INFO_2( "Executed action id: [%S], mode: [%S]",
                &aAction.Identifier(), &mode );
            }
        else
            {
            ERROR_2( err, "Failed to execute action id: [%S], mode: [%S]",
                &aAction.Identifier(), &mode );
            }
        }
    else
        {
        ERROR_2( KErrArgument, "Failed to execute action id: [%S], mode: [%S]. Unknown mode",
            &aAction.Identifier(), &mode );
        }
    
    // Always synchronous
    return CCFActionPlugIn::ENone;
    }

// End of file

