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
* Description:  CCFGreaterOrEqual class implementation.
*
*/



// INCLUDES
#include "cfgreaterorequal.h"
#include "cfcontextobject.h"
#include "cfcontextoperationutils.h"
#include "cfbasicoptrace.h"

#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptGreaterOrEqualName, "greaterOrEqual" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFGreaterOrEqual::CCFGreaterOrEqual
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFGreaterOrEqual::CCFGreaterOrEqual( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    HBufC* aName,
    HBufC* aSource,
    const CCFContextOperation::TCmpType aType )
    :   CCFContextOperation( aServices, aParent, aName, aSource ),
        iType( aType )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFGreaterOrEqual::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFGreaterOrEqual::ConstructL( const TDesC& aCmpVal )
    {
    FUNC_LOG;

    iCmpValue = aCmpVal.AllocL();
    }

// -----------------------------------------------------------------------------
// CCFGreaterOrEqual::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFGreaterOrEqual* CCFGreaterOrEqual::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    TDesC& aName,
    TDesC& aSource,
    const CCFContextOperation::TCmpType aType,
    const TDesC& aCmpVal )
    {
    FUNC_LOG;

    CCFGreaterOrEqual* self
        = NewLC( aServices, aParent, aName, aSource, aType, aCmpVal );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFGreaterOrEqual::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFGreaterOrEqual* CCFGreaterOrEqual::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    TDesC& aName,
    TDesC& aSource,
    const CCFContextOperation::TCmpType aType,
    const TDesC& aCmpVal )
    {
    FUNC_LOG;

    HBufC* name = aName.AllocLC();
    HBufC* source = aSource.AllocLC();
    CCFGreaterOrEqual* self = new( ELeave ) CCFGreaterOrEqual( aServices,
            aParent,
            name,
            source,
            aType );
    CleanupStack::PushL( self );
    self->ConstructL( aCmpVal );

    // Cleanup
    CleanupStack::Pop( self );
    CleanupStack::Pop( source );
    CleanupStack::Pop( name );

    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFGreaterOrEqual::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFGreaterOrEqual* CCFGreaterOrEqual::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptGreaterOrEqualName ) != 0 )
        {
        return NULL; // Cannot create greater or equal operation from the node.
        }

    TPtrC contextSource;
    TPtrC contextType;
    TCmpType cmpType( CCFContextOperation::EInvalidCmpType );
    TPtrC cmpValue;
    TInt contextLevelDelay( 0 );
    TBool argsOK = CFContextOperationUtils::ParseTwoComparisonArgs( aNode,
            contextSource,
            contextType,
            cmpType,
            cmpValue,
            contextLevelDelay );

    CCFGreaterOrEqual* self = NULL;
    if ( argsOK )
        {
        self = NewL( aServices,
                aParent,
                contextType,
                contextSource,
                cmpType,
                cmpValue );
        if ( contextLevelDelay )
            {
            self->iContextLevelDelay = contextLevelDelay;
            }
        }
    else
        {
        INFO( "CCFGreaterOrEqual::ParseL - Unsupported arguments" );
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFGreaterOrEqual::~CCFGreaterOrEqual()
    {
    FUNC_LOG;

    delete iCmpValue;
    }

// -----------------------------------------------------------------------------
// CCFGreaterOrEqual::IsTrueL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCFGreaterOrEqual::IsTrueL( const CCFContextObject& aContextObject )
    {
    FUNC_LOG;

    TBool value( EFalse );
    switch ( iType )
        {
        case CCFContextOperation::EIntCmp:
            {
            TInt a = CFContextOperationUtils::StringToIntL( *iCmpValue );
            TInt b = CFContextOperationUtils::StringToIntL(
                    aContextObject.Value() );
            value = b >= a;
            break;
            }
        case CCFContextOperation::ETimeCmp:
            {
            TTime a = CFContextOperationUtils::StringToTimeL( *iCmpValue );
            TTime b = CFContextOperationUtils::StringToTimeL(
                    aContextObject.Value() );
            value = b >= a;
            break;
            }
        case CCFContextOperation::EFloatCmp:
            {
            TReal a = CFContextOperationUtils::StringToRealL( *iCmpValue );
            TReal b = CFContextOperationUtils::StringToRealL(
                    aContextObject.Value() );
            value = b >= a;
            break;
            }
        case CCFContextOperation::EStringCmp:
        default:
            {
            value = aContextObject.Value() >= ( *iCmpValue );
            break;
            }
        }

    return value;
    }
