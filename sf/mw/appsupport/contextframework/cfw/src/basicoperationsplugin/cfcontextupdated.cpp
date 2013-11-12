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
* Description:  CCFContextUpdated class implementation.
*
*/



// INCLUDES
#include "cfcontextupdated.h"
#include "cfcontextoperationutils.h"
#include "cfbasicoptrace.h"

#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptUpdatedName, "contextUpdated" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFContextUpdated::CCFContextUpdated
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFContextUpdated::CCFContextUpdated( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    HBufC* aName,
    HBufC* aSource )
    : CCFContextOperation( aServices, aParent, aName, aSource )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFContextUpdated::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFContextUpdated::ConstructL( const TDesC& aValue )
    {
    FUNC_LOG;

    if ( aValue.Length() )
        {
        iCmpValue = aValue.AllocL();
        }
    }

// -----------------------------------------------------------------------------
// CCFContextUpdated::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFContextUpdated* CCFContextUpdated::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    const TDesC& aName,
    const TDesC& aSource,
    const TDesC& aValue )
    {
    FUNC_LOG;

    CCFContextUpdated* self
        = NewLC( aServices, aParent, aName, aSource, aValue );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFContextUpdated::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFContextUpdated* CCFContextUpdated::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    const TDesC& aName,
    const TDesC& aSource,
    const TDesC& aValue )
    {
    FUNC_LOG;

    HBufC* name = aName.AllocLC();
    HBufC* source = aSource.AllocLC();
    CCFContextUpdated* self
        = new( ELeave ) CCFContextUpdated( aServices, aParent, name, source );
    CleanupStack::PushL( self );
    self->ConstructL( aValue );

    // Cleanup
    CleanupStack::Pop( self );
    CleanupStack::Pop( source );
    CleanupStack::Pop( name );

    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFContextUpdated::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFContextUpdated* CCFContextUpdated::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptUpdatedName ) != 0 )
        {
        return NULL; // Cannot create context updated operation from the node.
        }

    TPtrC contextSource;
    TPtrC contextType;
    TPtrC contextValue;
    TInt contextLevelDelay( 0 );

    TBool argsOK( ETrue );
    TBool contextRefOK( EFalse );
    CMDXMLNode* child = aNode.FirstChild();
    while ( child )
        {
        if ( child->NodeType() == CMDXMLNode::EElementNode )
            {
            if ( !contextRefOK )
                {
                contextRefOK = CFContextOperationUtils::ParseContextRef(
                        *child,
                        contextSource,
                        contextType,
                        contextValue,
                        contextLevelDelay );
                if ( !contextRefOK )
                    {
                    argsOK = EFalse;
                    break;
                    }
                }
            else
                {
                argsOK = EFalse;
                break;
                }
            }
        else if ( child->NodeType() != CMDXMLNode::ECommentNode )
            {
            argsOK = EFalse; // Unsupported node encountered.
            break;
            }
        child = child->NextSibling();
        }

    CCFContextUpdated* self = NULL;
    if ( argsOK && contextRefOK )
        {
        self = NewL( aServices,
                aParent,
                contextType,
                contextSource,
                contextValue );
        if ( contextLevelDelay )
            {
            self->iContextLevelDelay = contextLevelDelay;
            }
        }
    else
        {
        INFO( "CCFContextUpdated::ParseL - Unsupported arguments" );
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFContextUpdated::~CCFContextUpdated()
    {
    FUNC_LOG;

    delete iCmpValue;
    }


// ---------------------------------------------------------------------------
// CCFContextUpdated::IsAllRequired
// ---------------------------------------------------------------------------
//
TBool CCFContextUpdated::IsAllRequired() const
    {
    FUNC_LOG;

    return ETrue;
    }

// ---------------------------------------------------------------------------
// CCFContextUpdated::Evaluate
// ---------------------------------------------------------------------------
//
TBool CCFContextUpdated::Evaluate( const CCFContextObject& aContext,
    TInt& aContextLevelDelay )
    {
    FUNC_LOG;

    DOM_INFO( "CCFContextUpdated::Evaluate" );

    // Ask concrete implementation if it is true.
    TInt val( EFalse );
    TRAPD( err, val = IsTrueL( aContext ) );

    if ( err != KErrNone )
        {
        ERROR( err,
            "CCFContextUpdated: IsTrueL leave occurred, evaluation cancelled" );
        return EFalse;
        }

    // Evaluate parents if we have changed.
    if ( val != iValue )
        {
        INFO_1( "CCFContextUpdated::Evaluate - Value changed to (-1=undefined, 0=false, 1=true): %d", val );

        iValue = static_cast< TCFConditionValue >( val );
        if ( iParent )
            {
            iParent->Evaluate();
            }
        }
    else
        {
        INFO_1( "CCFContextUpdated::Evaluate - Value still (-1=undefined, 0=false, 1=true): %d", val );
        }

    if ( iPreviousTruthCheckWithReferencedContext )
        {
        aContextLevelDelay = iContextLevelDelay;
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCFContextUpdated::IsTrueL
// -----------------------------------------------------------------------------
//
TBool CCFContextUpdated::IsTrueL( const CCFContextObject& aContext )
    {
    FUNC_LOG;

    TBool value( EFalse );
    if ( aContext.Source() == Source() && aContext.Type() == Type() )
        {
        // Context was updated, no need to check for change.
        if ( iCmpValue )
            {
            if ( aContext.Value() == *iCmpValue )
                {
                value = ETrue;
                }
            }
        else
            {
            value = ETrue;
            }
        iPreviousTruthCheckWithReferencedContext = ETrue;
        }
    else
        {
        iPreviousTruthCheckWithReferencedContext = EFalse;
        }

    return value;
    }
