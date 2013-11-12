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
* Description:  CVibraActionPlugIn class implementation.
*
*/


#include "vibraactionplugin.h"

#include <cfactionindication.h>

#include "vibratrace.h"
#include "vibraaction.h"


// CONSTANTS

// Security policy for vibra action
_LIT_SECURITY_POLICY_PASS( KSecurityPolicy );

// Vibra actions
_LIT( KVibraAction, "Vibra" );

// Vibra parameters
_LIT( KVibraMode, "Mode" );
_LIT( KVibraDelay, "Delay" );
_LIT( KVibraRepeats, "Repeats" );
_LIT( KVibraRepeatInterval, "RepeatInterval" );
_LIT( KVibraDuration, "Duration" );
_LIT( KVibraIntensity, "Intensity" );

// Vibra mode values
_LIT( KVibraStart, "Start" );
_LIT( KVibraStop, "Stop" );

#ifdef _DEBUG
// Panic category
_LIT( KPanicCat, "VibraAction" );

// Panic codes
enum TPanicCode
    {
    EVibraActionNotInitialized
    };

// Local panic function
LOCAL_C void Panic( TPanicCode aCode )
    {
    User::Panic( KPanicCat, aCode );
    }

#endif

// MEMBER FUNCTIONS

CVibraActionPlugIn* CVibraActionPlugIn::NewL()
    {
    FUNC_LOG;

    CVibraActionPlugIn* self = CVibraActionPlugIn::NewLC();
    CleanupStack::Pop( self );

    return self;
    }

CVibraActionPlugIn* CVibraActionPlugIn::NewLC()
    {
    FUNC_LOG;

    CVibraActionPlugIn* self = new( ELeave ) CVibraActionPlugIn;
    CleanupStack::PushL( self );

    return self;
    }

// Destructor
CVibraActionPlugIn::~CVibraActionPlugIn()
    {
    FUNC_LOG;

    delete iVibraAction;
    }

CVibraActionPlugIn::CVibraActionPlugIn()
    {
    FUNC_LOG;

    // Nothing to do
    }

// METHODS

//-----------------------------------------------------------------------------
// CVibraActionPlugIn::InitializeL
//-----------------------------------------------------------------------------
//
void CVibraActionPlugIn::InitializeL()
    {
    FUNC_LOG;

    iVibraAction = CVibraAction::NewL( *this );
    }

//-----------------------------------------------------------------------------
// CVibraActionPlugIn::ExecuteL
//-----------------------------------------------------------------------------
//
CCFActionPlugIn::TExecutionTime CVibraActionPlugIn::ExecuteL(
    CCFActionIndication* aActionIndication )
    {
    FUNC_LOG;

    CCFActionPlugIn::TExecutionTime time = CCFActionPlugIn::ENone;

    __ASSERT_DEBUG( iVibraAction, Panic( EVibraActionNotInitialized ) );

    // Parse settings
    const RKeyValueArray& parameters = aActionIndication->Parameters();
    TInt count = parameters.Count();
    TPtrC key( KNullDesC );
    TPtrC value( KNullDesC );

    TVibraActionInfo info;
    TPtrC mode( KNullDesC );
    for( TInt i = 0; i < count; i++ )
        {
        key.Set( parameters[i]->Key() );
        value.Set( parameters[i]->Value() );

        // Mode
        if( key.CompareF( KVibraMode ) == KErrNone )
            {
            mode.Set( value );
            INFO_1( "Vibra mode: %S", &mode );
            }
        // Delay
        else if( key.CompareF( KVibraDelay ) == KErrNone )
            {
            info.iDelay = ConvertToInt( value );
            INFO_1( "Vibra delay: %d", info.iDelay );
            }
        // Repeats
        else if( key.CompareF( KVibraRepeats ) == KErrNone )
            {
            info.iRepeats = ConvertToInt( value );
            INFO_1( "Vibra repeats: %d", info.iRepeats );
            }
        // Repeat interval
        else if( key.CompareF( KVibraRepeatInterval ) == KErrNone )
            {
            info.iRepeatInterval = ConvertToInt( value );
            INFO_1( "Vibra repeat interval: %d", info.iRepeatInterval );
            }
        // Duration
        else if( key.CompareF( KVibraDuration ) == KErrNone )
            {
            info.iDuration = ConvertToInt( value );
            INFO_1( "Vibra duration: %d", info.iDuration );
            }
        // Intensity
        else if( key.CompareF( KVibraIntensity ) == KErrNone )
            {
            info.iIntensity = ConvertToInt( value );
            INFO_1( "Vibra intensity: %d", info.iIntensity );
            }
        }

    // Start vibra
    if( mode.CompareF( KVibraStart ) == KErrNone )
        {
        // Check if we have to repeat vibra pulses
        // In this case we must not return with ENone since
        // action execution thread will start waiting in semaphore
        // and none of the active objects will then be run
        time = iVibraAction->StartVibraL( info );
        }
    // Stop vibra
    else if( mode.CompareF( KVibraStop ) == KErrNone )
        {
        iVibraAction->StopVibraL();
        }

    return time;
    }

//-----------------------------------------------------------------------------
// CVibraActionPlugIn::GetActionsL
//-----------------------------------------------------------------------------
//
void CVibraActionPlugIn::GetActionsL( CDesCArray& aActionList ) const
    {
    FUNC_LOG;

    aActionList.AppendL( KVibraAction );
    }

//-----------------------------------------------------------------------------
// CVibraActionPlugIn::SecurityPolicy
//-----------------------------------------------------------------------------
//
const TSecurityPolicy& CVibraActionPlugIn::SecurityPolicy() const
    {
    FUNC_LOG;

    return KSecurityPolicy;
    }

//-----------------------------------------------------------------------------
// CVibraActionPlugIn::VibraActionCompletedL
//-----------------------------------------------------------------------------
//
void CVibraActionPlugIn::VibraActionCompletedL()
    {
    FUNC_LOG;

    AsyncExecutionCompleted();
    }

//-----------------------------------------------------------------------------
// CVibraActionPlugIn::ConvertToInt
//-----------------------------------------------------------------------------
//
TInt CVibraActionPlugIn::ConvertToInt( const TDesC& aDesc ) const
    {
    FUNC_LOG;

    TInt value = 0;

    TLex lex( aDesc );
    if( lex.Val( value ) != KErrNone )
        {
        value = KErrNotFound;
        }

    return value;
    }
