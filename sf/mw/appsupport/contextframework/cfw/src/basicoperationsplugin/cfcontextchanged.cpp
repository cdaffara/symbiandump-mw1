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
* Description:  CCFContextChanged class implementation.
*
*/



// INCLUDES
#include "cfcontextchanged.h"
#include "cfcontextoperationutils.h"
#include "cfbasicoptrace.h"

#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptChangedName, "contextChanged" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFContextChanged::CCFContextChanged
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFContextChanged::CCFContextChanged( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    HBufC* aName,
    HBufC* aSource )
    : CCFContextOperation( aServices, aParent, aName, aSource )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFContextChanged::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFContextChanged::ConstructL( const TDesC& aValue )
    {
    FUNC_LOG;

    if ( aValue.Length() )
        {
        iCmpValue = aValue.AllocL();
        }
    }

// -----------------------------------------------------------------------------
// CCFContextChanged::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFContextChanged* CCFContextChanged::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    const TDesC& aName,
    const TDesC& aSource,
    const TDesC& aValue )
    {
    FUNC_LOG;

    CCFContextChanged* self
        = NewLC( aServices, aParent, aName, aSource, aValue );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFContextChanged::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFContextChanged* CCFContextChanged::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    const TDesC& aName,
    const TDesC& aSource,
    const TDesC& aValue )
    {
    FUNC_LOG;

    HBufC* name = aName.AllocLC();
    HBufC* source = aSource.AllocLC();
    CCFContextChanged* self
        = new( ELeave ) CCFContextChanged( aServices, aParent, name, source );
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
// CCFContextChanged::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFContextChanged* CCFContextChanged::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptChangedName ) != 0 )
        {
        return NULL; // Cannot create context changed operation from the node.
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

    CCFContextChanged* self = NULL;
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
        INFO( "CCFContextChanged::ParseL - Unsupported arguments" );
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFContextChanged::~CCFContextChanged()
    {
    FUNC_LOG;

    delete iCmpValue;
    }

// ---------------------------------------------------------------------------
// CCFContextChanged::IsAllRequired
// ---------------------------------------------------------------------------
//
TBool CCFContextChanged::IsAllRequired() const
    {
    FUNC_LOG;

    return ETrue;
    }

// ---------------------------------------------------------------------------
// CCFContextChanged::Evaluate
// ---------------------------------------------------------------------------
//
TBool CCFContextChanged::Evaluate( const CCFContextObject& aContext,
    TInt& aContextLevelDelay )
    {
    FUNC_LOG;

    DOM_INFO( "CCFContextChanged::Evaluate" );

    // Ask concrete implementation if it is true.
    TInt val( EFalse );
    TRAPD( err, val = IsTrueL( aContext ) );

    if ( err != KErrNone )
        {
        ERROR( err,
            "CCFContextChanged: IsTrueL leave occurred, evaluation cancelled" );
        return EFalse;
        }

    // Evaluate parents if we have changed.
    if ( val != iValue )
        {
        INFO_1( "CCFContextChanged::Evaluate - Value changed to (-1=undefined, 0=false, 1=true): %d", val );

        iValue = static_cast< TCFConditionValue >( val );
        if ( iParent )
            {
            iParent->Evaluate();
            }
        }
    else
        {
        INFO_1( "CCFContextChanged::Evaluate - Value still (-1=undefined, 0=false, 1=true): %d", val );
        }

    if ( iPreviousTruthCheckWithReferencedContext )
        {
        aContextLevelDelay = iContextLevelDelay;
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCFContextChanged::IsTrueL
// -----------------------------------------------------------------------------
//
TBool CCFContextChanged::IsTrueL( const CCFContextObject& aContext )
    {
    FUNC_LOG;

    TBool value( EFalse );
    if ( aContext.Source() == Source() && aContext.Type() == Type() )
        {
        INFO_2( "CCFContextChanged::IsTrueL - Current value [%S] <> [%S] new value",
            &PreviousValue(), &aContext.Value() );

        // Check if context has changed since last time.
        if ( PreviousValue() != aContext.Value() )
            {
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

            TInt err = UpdatePreviousValue( aContext.Value() );
            ERROR( err, "Previous value update failed! [CCFContextChanged]" );
            }
        iPreviousTruthCheckWithReferencedContext = ETrue;
        }
    else
        {
        iPreviousTruthCheckWithReferencedContext = EFalse;
        }

    return value;
    }
