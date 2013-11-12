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
* Description:  CCFNotEqual class implementation.
*
*/



// INCLUDE FILES
#include "cfnotequal.h"
#include "cfcontextobject.h"
#include "cfcontextoperationutils.h"
#include "cfbasicoptrace.h"

#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptNotEqualName, "notEqual" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFNotEqual::CCFNotEqual
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFNotEqual::CCFNotEqual( MCFOperationServices& aServices,
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
// CCFNotEqual::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFNotEqual::ConstructL( const TDesC& aCmpVal )
    {
    FUNC_LOG;

    iCmpValue = aCmpVal.AllocL();
    }

// -----------------------------------------------------------------------------
// CCFNotEqual::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFNotEqual* CCFNotEqual::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    TDesC& aName,
    TDesC& aSource,
    const CCFContextOperation::TCmpType aType,
    const TDesC& aCmpVal )
    {
    FUNC_LOG;

    CCFNotEqual* self
        = NewLC( aServices, aParent, aName, aSource, aType, aCmpVal );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFNotEqual::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFNotEqual* CCFNotEqual::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    TDesC& aName,
    TDesC& aSource,
    const CCFContextOperation::TCmpType aType,
    const TDesC& aCmpVal )
    {
    FUNC_LOG;

    HBufC* name = aName.AllocLC();
    HBufC* source = aSource.AllocLC();
    CCFNotEqual* self
        = new( ELeave ) CCFNotEqual( aServices, aParent, name, source, aType );
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
// CCFNotEqual::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFNotEqual* CCFNotEqual::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptNotEqualName ) != 0 )
        {
        return NULL; // Cannot create not equal operation from the given node.
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

    CCFNotEqual* self = NULL;
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
        INFO( "CCFNotEqual::ParseL - Unsupported arguments" );
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFNotEqual::~CCFNotEqual()
    {
    FUNC_LOG;

    delete iCmpValue;
    }

// -----------------------------------------------------------------------------
// CCFNotEqual::IsTrueL
//
// -----------------------------------------------------------------------------
//
TBool CCFNotEqual::IsTrueL( const CCFContextObject& aContextObject )
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
            value = ( a != b );
            break;
            }
        case CCFContextOperation::ETimeCmp:
            {
            TTime a = CFContextOperationUtils::StringToTimeL( *iCmpValue );
            TTime b = CFContextOperationUtils::StringToTimeL(
                    aContextObject.Value() );
            value = ( a != b );
            break;
            }
        case CCFContextOperation::EFloatCmp:
            {
            TReal a = CFContextOperationUtils::StringToRealL( *iCmpValue );
            TReal b = CFContextOperationUtils::StringToRealL(
                    aContextObject.Value() );
            value = ( a != b );
            break;
            }
        case CCFContextOperation::EStringCmp:
        default:
            {
            value = ( *iCmpValue != aContextObject.Value() );
            break;
            }
        }

    return value;
    }
