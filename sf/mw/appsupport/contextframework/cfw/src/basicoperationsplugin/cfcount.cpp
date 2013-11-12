/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFCount class implementation.
*
*/



// INCLUDES
#include "cfcount.h"
#include "cfcountoneshot.h"
#include "cfcountrepeating.h"
#include "cfcontextoperationutils.h"
#include "cfbasicoptrace.h"

#include <cfcontextobject.h>
#include <gmxmlelement.h>
#include <s32strm.h>

// CONSTANTS
_LIT( KScriptCountName,               "count"               );
_LIT( KScriptRepeatIntervalAttribute, "repeatInterval"      );
_LIT( KScriptOneShotAtAttribute,      "oneShotAt"           );

static const TUint KInitialCount = 0;


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCFCount::CCFCount
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CCFCount::CCFCount( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    HBufC* aName,
    HBufC* aSource,
    const CCFContextOperation::TCmpType aType,
    const TUint aCount,
    const TCountType aCountType )
    :   CCFContextOperation( aServices, aParent, aName, aSource ),
        iType( aType ),
        iCurrentCount( KInitialCount ),
        iActive( EFalse ),
        iFinished( EFalse ),
        iCmpCount( aCount ),
        iCountType( aCountType )
    {
    FUNC_LOG;

    if ( iCmpCount == KInitialCount )
        {
        iValue = ECFConditionTrue;
        }
    else
        {
        iValue = ECFConditionFalse;
        }
    }

// ---------------------------------------------------------------------------
// CCFCount::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CCFCount::ConstructL( const TDesC& aCmpVal )
    {
    FUNC_LOG;

    iCmpValue = aCmpVal.AllocL();
    }

// -----------------------------------------------------------------------------
// CCFCount::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFCount* CCFCount::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptCountName ) != 0
        || aNode.NodeType() != CMDXMLNode::EElementNode )
        {
        return NULL; // Cannot create count operation from the given node.
        }

    CCFCount* self = NULL;
    // Check & get count attribute.
    CMDXMLElement& element = static_cast< CMDXMLElement& >( aNode );
    TPtrC countAttributeValue;
    if ( KErrNone == element.GetAttribute( KScriptRepeatIntervalAttribute,
            countAttributeValue ) )
        {
        self = CCFCountRepeating::ParseL( aServices, aParent, aNode );
        }
    else if ( KErrNone == element.GetAttribute( KScriptOneShotAtAttribute,
            countAttributeValue ) )
        {
        self = CCFCountOneShot::ParseL( aServices, aParent, aNode );
        }
    else
        {
        INFO( "CCFCount::ParseL - count missing required attribute" );
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFCount::~CCFCount()
    {
    FUNC_LOG;

    delete iCmpValue;
    delete iPersistencyFile;
    }


//-----------------------------------------------------------------------------
// CCFCount::InternalizeL
//-----------------------------------------------------------------------------
//
void CCFCount::InternalizeL( RReadStream& aStream )
    {
    FUNC_LOG;

    iValue = static_cast< TCFConditionValue >( aStream.ReadInt32L() );
    iCurrentCount = aStream.ReadUint32L();
    iFinished = aStream.ReadInt8L();
    iReEvaluateFromNextContext = aStream.ReadInt8L();
    }

//-----------------------------------------------------------------------------
// CCFCount::ExternalizeL
//-----------------------------------------------------------------------------
//
void CCFCount::ExternalizeL( RWriteStream& aStream )
    {
    FUNC_LOG;

    aStream.WriteInt32L( iValue );
    aStream.WriteUint32L( iCurrentCount );
    aStream.WriteInt8L( iFinished );
    aStream.WriteInt8L( iReEvaluateFromNextContext );
    }

//-----------------------------------------------------------------------------
// CCFCount::Cleanup
//-----------------------------------------------------------------------------
//
void CCFCount::Cleanup()
    {
    FUNC_LOG;

    if ( iPersistencyFile )
        {
        iServices.Delete( *iPersistencyFile );
        }
    }


// ---------------------------------------------------------------------------
// CCFCount::IsAllRequired
// ---------------------------------------------------------------------------
//
TBool CCFCount::IsAllRequired() const
    {
    FUNC_LOG;

    if ( iCountType == ECountTypeTrigger )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// ---------------------------------------------------------------------------
// CCFCount::Evaluate
// ---------------------------------------------------------------------------
//
TBool CCFCount::Evaluate( const CCFContextObject& aContext,
    TInt& aContextLevelDelay )
    {
    FUNC_LOG;

    DOM_INFO( "CCFCount::Evaluate" );

    TBool evaluated( EFalse );
    TInt err( KErrNone );
    TRAP( err, evaluated = DoEvaluateL( aContext ) );
    ERROR( err, "CCFCount::Evaluate - call to DoEvaluateL leaved" );

    aContextLevelDelay = iContextLevelDelay;
    return evaluated;
    }

// ---------------------------------------------------------------------------
// CCFCount::ActivateL
// ---------------------------------------------------------------------------
//
void CCFCount::ActivateL()
    {
    FUNC_LOG;

    if ( !iFinished && iPersistencyFile )
        {
        iServices.RestoreL( *iPersistencyFile, *this );
        }

    if ( !iFinished && !iActive )
        {
        iServices.SubscribeContextL( this );
        // Activate now to prevent pre-evaluation with possible cached context.
        iActive = ETrue;
        }

    // Assure that parent will be initialized correctly.
    if ( iParent )
        {
        iParent->Evaluate();
        }
    }

// ---------------------------------------------------------------------------
// CCFCount::Deactivate
// ---------------------------------------------------------------------------
//
void CCFCount::Deactivate()
    {
    FUNC_LOG;

    if ( iActive )
        {
        iActive = EFalse;
        iServices.RemoveSubscription( this );
        }

    if ( iPersistencyFile )
        {
        TRAP_IGNORE( iServices.StoreL( *iPersistencyFile, *this ) );
        }
    }

// ---------------------------------------------------------------------------
// CCFCount::DoEvaluateL
// ---------------------------------------------------------------------------
//
TBool CCFCount::DoEvaluateL( const CCFContextObject& aContext )
    {
    FUNC_LOG;

    TBool evaluated( ETrue );
    if ( !iActive )
        {
        INFO( "CCFCount::Evaluate - SKIPPED, count not active!" );
        evaluated = EFalse;

        return evaluated;
        }

    if ( !CountContextL( aContext ) )
        {
        if ( iReEvaluateFromNextContext
                && iCountType == CCFCount::ECountTypeTrigger )
            {
            INFO( "CCFCount::Evaluate - Trigger type, re-evaluating parent" );

            iReEvaluateFromNextContext = EFalse;
            if ( iParent )
                {
                iParent->Evaluate();
                }

            if ( iPersistencyFile )
                {
                iServices.StoreL( *iPersistencyFile, *this );
                }
            }
        else
            {
            INFO( "CCFCount::Evaluate - SKIPPED, count not counting this context!" );
            evaluated = EFalse;
            }

        return evaluated;
        }

    // Ask concrete implementation if it is true.
    TBool val = IsTrueL( aContext );

    // Evaluate parents if value changed.
    if ( val != iValue )
        {
        INFO_1( "CCFCount::Evaluate - Value changed to (-1=undefined, 0=false, 1=true): %d", val );

        iValue = static_cast< TCFConditionValue >( val );
        if ( iParent )
            {
            iParent->Evaluate();
            }
        iReEvaluateFromNextContext = EFalse;

        if ( iValue == CCFOperationNode::ECFConditionTrue
                && iCountType == CCFCount::ECountTypeTrigger )
            {
            INFO( "CCFCount::Evaluate - Trigger type, value changed to false" );

            iValue = CCFOperationNode::ECFConditionFalse;
            iReEvaluateFromNextContext = ETrue;
            }
        }
    else
        {
        INFO_1( "CCFCount::Evaluate - Value still (-1=undefined, 0=false, 1=true): %d", val );

        if ( iValue == CCFOperationNode::ECFConditionFalse
                && iCountType == CCFCount::ECountTypeTrigger
                && iReEvaluateFromNextContext )
            {
            INFO( "CCFCount::Evaluate - Trigger type, parent re-evaluated now!" );

            iReEvaluateFromNextContext = EFalse;
            if ( iParent )
                {
                iParent->Evaluate();
                }
            }
        }

    if ( iPersistencyFile )
        {
        iServices.StoreL( *iPersistencyFile, *this );
        }

    return evaluated;
    }

// ---------------------------------------------------------------------------
// CCFCount::CountContextL
// ---------------------------------------------------------------------------
//
TBool CCFCount::CountContextL( const CCFContextObject& aContext )
    {
    FUNC_LOG;

    TBool counted( EFalse );
    if ( aContext.Source() == Source() && aContext.Type() == Type() )
        {
        if ( iCmpValue->Length() == 0 )
            {
            ++iCurrentCount;
            counted = ETrue;
            }
        else
            {
            TBool cmpValue( EFalse );
            switch ( iType )
                {
                case CCFContextOperation::EIntCmp:
                    {
                    TInt a = CFContextOperationUtils::StringToIntL(
                            *iCmpValue );
                    TInt b = CFContextOperationUtils::StringToIntL(
                            aContext.Value() );
                    cmpValue = ( a == b );
                    break;
                    }
                case CCFContextOperation::ETimeCmp:
                    {
                    TTime a = CFContextOperationUtils::StringToTimeL(
                            *iCmpValue );
                    TTime b = CFContextOperationUtils::StringToTimeL(
                            aContext.Value() );
                    cmpValue = ( a == b );
                    break;
                    }
                case CCFContextOperation::EFloatCmp:
                    {
                    TReal a = CFContextOperationUtils::StringToRealL(
                            *iCmpValue );
                    TReal b = CFContextOperationUtils::StringToRealL(
                            aContext.Value() );
                    cmpValue = ( a == b );
                    break;
                    }
                case CCFContextOperation::EStringCmp:
                default:
                    {
                    cmpValue = ( *iCmpValue == aContext.Value() );
                    break;
                    }
                }
    
            if ( cmpValue )
                {
                ++iCurrentCount;
                counted = ETrue;
                }
            }
        }
    else
        {
        // Count type must be trigger.
        if ( iCountType != CCFCount::ECountTypeTrigger )
            {
            ERROR_GEN( "CCFCount::CountContext - count is NOT trigger type!" );
            }
        }

    return counted;
    }
