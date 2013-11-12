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
* Description:  CCFNotOperation class implementation.
*
*/



// INCLUDES
#include "cfnotoperation.h"
#include "cfbasicoptrace.h"

#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptNotName, "not" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCFNotOperation::CCFNotOperation
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CCFNotOperation::CCFNotOperation( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    : CCFOperationNode( aServices, aParent )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCFNotOperation::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CCFNotOperation::ConstructL()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCFNotOperation::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFNotOperation* CCFNotOperation::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFNotOperation* self = NewLC( aServices, aParent );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CCFNotOperation::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFNotOperation* CCFNotOperation::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFNotOperation* self = new( ELeave ) CCFNotOperation( aServices, aParent );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CCFNotOperation::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFNotOperation* CCFNotOperation::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptNotName ) != 0 )
        {
        return NULL; // Cannot create and operation from the given node.
        }

    CCFNotOperation* self = NewLC( aServices, aParent );    // CLEANUP<< self

    TBool unsupportedNode( EFalse );
    CMDXMLNode* child = aNode.FirstChild();
    while ( child )
        {
        if ( child->NodeType() == CMDXMLNode::EElementNode )
            {
            if ( self->iChild )
                {
                TPtrC nodeName( child->NodeName() );
                INFO_1( "CCFNotOperation::ParseL - One child operation is allowed. [%S] is not supported",
                        &nodeName );
                unsupportedNode = ETrue;
                break;
                }
            CCFOperationNode* op = self->iServices.ParseL( self, *child );
            if ( op )
                {
                self->iChild = op;
                }
            else
                {
                unsupportedNode = ETrue;
                break;
                }
            }
        else if ( child->NodeType() != CMDXMLNode::ECommentNode )
            {
            unsupportedNode = ETrue;
            break;
            }
        child = child->NextSibling();
        }

    CleanupStack::Pop( self );                              // CLEANUP<< self
    if ( unsupportedNode || !self->iChild )
        {
        delete self; // Discard unsupported not operation.
        self = NULL;

        if ( unsupportedNode )
            {
            TPtrC nodeName( child->NodeName() );
            INFO_1( "CCFNotOperation::ParseL - Unsupported node [%S]",
                    &nodeName );
            }
        else
            {
            INFO( "CCFNotOperation::ParseL - No child defined" );
            }
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFNotOperation::~CCFNotOperation()
    {
    FUNC_LOG;

    delete iChild;
    }


// ---------------------------------------------------------------------------
// CCFNotOperation::ActivateL
// ---------------------------------------------------------------------------
//
void CCFNotOperation::ActivateL()
    {
    FUNC_LOG;

    if ( iChild )
        {
        iChild->ActivateL();
        }
    }

// ---------------------------------------------------------------------------
// CCFNotOperation::Deactivate
// ---------------------------------------------------------------------------
//
void CCFNotOperation::Deactivate()
    {
    FUNC_LOG;

    if ( iChild )
        {
        iChild->Deactivate();
        }
    }

// ---------------------------------------------------------------------------
// CCFNotOperation::CheckSecurity
// ---------------------------------------------------------------------------
//
TInt CCFNotOperation::CheckSecurity()
    {
    FUNC_LOG;

    TInt err( KErrNone );
    // Check security for context subscriptions.
    if ( iChild )
        {
        err = iChild->CheckSecurity();
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CCFNotOperation::Evaluate
// ---------------------------------------------------------------------------
//
void CCFNotOperation::Evaluate()
    {
    FUNC_LOG;

    DOM_INFO( "CCFNotOperation::Evaluate" );

    TCFConditionValue newValue( Value() );
    if ( iChild )
        {
        newValue = iChild->Value();
        }

    if ( newValue == ECFConditionUndefined )
        {
        // Do nothing, not undefined = undefined.
        }
    else
        {
        newValue = ( TCFConditionValue )( !newValue );
        }

    if ( newValue != Value() )
        {
        INFO_1( "CCFNotOperation::Evaluate - Value changed to (-1=undefined, 0=false, 1=true): %d", newValue );

        iValue = newValue;
        if ( iParent )
            {
            iParent->Evaluate();
            }
        }
    else
        {
        INFO_1( "CCFNotOperation::Evaluate - Value still (-1=undefined, 0=false, 1=true): %d", newValue );
        }
    }

// -----------------------------------------------------------------------------
// CCFNotOperation::Cleanup
// -----------------------------------------------------------------------------
//
void CCFNotOperation::Cleanup()
    {
    FUNC_LOG;

    if ( iChild )
        {
        iChild->Cleanup();
        }
    }
