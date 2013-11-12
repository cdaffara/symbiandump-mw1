/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextOperation class implementation.
*
*/


// INCLUDES
#include <f32file.h> // for KExtDelimiter '.' and KDriveDelimiter ':'

#include "cfcontextoperation.h"
#include "cfbasicoptrace.h"

// CONSTANTS
const TInt KCFContextOperationNoContextLevelDelay = 0;
const TInt KDefaultPreviousValueLength = 256;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCFContextOperation::CCFContextOperation
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CCFContextOperation::CCFContextOperation( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    HBufC* aType,
    HBufC* aSource )
    :   CCFOperationNode( aServices, aParent ),
        iContextLevelDelay( 0 ),
        iSource( aSource ),
        iType( aType )
    {
    FUNC_LOG;

    iPreviousValue = HBufC::New( KDefaultPreviousValueLength );
    }


// Destructor
CCFContextOperation::~CCFContextOperation()
    {
    FUNC_LOG;

    delete iSource;
    delete iType;
    delete iPreviousValue;
    }


// ---------------------------------------------------------------------------
// CCFContextOperation::Evaluate
// ---------------------------------------------------------------------------
//
TBool CCFContextOperation::Evaluate( const CCFContextObject& aContext,
    TInt& aContextLevelDelay )
    {
    FUNC_LOG;

    DOM_INFO( "CCFContextOperation::Evaluate" );

    // Check that context value has changed (context is state information)
    if ( iPreviousValue && ( *iPreviousValue ).Compare( aContext.Value() ) == 0 )
        {
        INFO_2( "CCFContextOperation::Evaluate - NOT evaluating (prev value [%S] == arg value [%S])!",
            iPreviousValue, &aContext.Value() );
        aContextLevelDelay = KCFContextOperationNoContextLevelDelay;
        return EFalse;
        }
    TInt err = UpdatePreviousValue( aContext.Value() );
    ERROR( err, "Previous value update failed!" );

    // Ask concrete implementation if it is true
    TInt val( EFalse );
    TRAP( err, val = IsTrueL( aContext ) );

    if ( err != KErrNone )
        {
        ERROR( err, "CCFContextOperation: IsTrueL leave occurred, evaluation cancelled" );
        return EFalse;
        }

    // Evaluate parents if we have changed
    if ( val != iValue )
        {
        INFO_1( "CCFContextOperation::Evaluate - Value changed to (-1=undefined, 0=false, 1=true): %d", val );

        iValue = static_cast< TCFConditionValue >( val );
        if ( iParent )
            {
            iParent->Evaluate();
            }
        }
    else
        {
        INFO_1( "CCFContextOperation::Evaluate - Value still (-1=undefined, 0=false, 1=true): %d", val );
        }

    aContextLevelDelay = iContextLevelDelay;
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CCFContextOperation::Source
// ---------------------------------------------------------------------------
//
TPtrC CCFContextOperation::Source() const
    {
    FUNC_LOG;

    return TPtrC( *iSource );
    }

// ---------------------------------------------------------------------------
// CCFContextOperation::Type
// ---------------------------------------------------------------------------
//
TPtrC CCFContextOperation::Type() const
    {
    FUNC_LOG;

    return TPtrC( *iType );
    }

// ---------------------------------------------------------------------------
// CCFContextOperation::IsAllRequired
// ---------------------------------------------------------------------------
//
TBool CCFContextOperation::IsAllRequired() const
    {
    FUNC_LOG;

    return EFalse;
    }


// ---------------------------------------------------------------------------
// CCFContextOperation::UpdatePreviousValue
// ---------------------------------------------------------------------------
//
TInt CCFContextOperation::UpdatePreviousValue( const TDesC& aNewValue )
    {
    FUNC_LOG;

    TInt err = KErrNone;
    if( !iPreviousValue )
        {
        iPreviousValue = HBufC::New( KDefaultPreviousValueLength );
        }

    if( iPreviousValue )
        {
        TPtr previousValuePtr = iPreviousValue->Des();
        if( previousValuePtr.MaxLength() >= aNewValue.Length() )
            {
            previousValuePtr.Copy( aNewValue );
            }
        else
            {
            delete iPreviousValue;
            iPreviousValue = NULL;
            iPreviousValue = aNewValue.Alloc();
            if( !iPreviousValue )
                {
                err = KErrNoMemory;
                }
            }
        }
    else
        {
        err = KErrNoMemory;
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CCFContextOperation::PreviousValue
// ---------------------------------------------------------------------------
//
const TDesC& CCFContextOperation::PreviousValue()
    {
    if ( iPreviousValue )
        {
        return *iPreviousValue;
        }
    return KNullDesC;
    }


// ---------------------------------------------------------------------------
// CCFContextOperation::ActivateL
// ---------------------------------------------------------------------------
//
void CCFContextOperation::ActivateL()
    {
    FUNC_LOG;

    iServices.SubscribeContextL( this );
    }

// ---------------------------------------------------------------------------
// CCFContextOperation::Deactivate
// ---------------------------------------------------------------------------
//
void CCFContextOperation::Deactivate()
    {
    FUNC_LOG;

    iServices.RemoveSubscription( this );
    }

// ---------------------------------------------------------------------------
// CCFContextOperation::CheckSecurity
// ---------------------------------------------------------------------------
//
TInt CCFContextOperation::CheckSecurity()
    {
    FUNC_LOG;

    if ( !iSource || !iType )
        {
        return KErrBadDescriptor;
        }

    return iServices.CheckContextReadSecurity( *iSource, *iType );
    }

// ---------------------------------------------------------------------------
// CCFContextOperation::Evaluate
// ---------------------------------------------------------------------------
//
void CCFContextOperation::Evaluate()
    {
    FUNC_LOG;
    }
