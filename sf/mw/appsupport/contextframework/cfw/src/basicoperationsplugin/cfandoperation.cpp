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
* Description:  Class that defines logical AND operation in an XML tree.
*
*/



// INCLUDES
#include "cfandoperation.h"
#include "cfbasicoptrace.h"

#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptAndName, "and" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFAndOperation::CCFAndOperation
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFAndOperation::CCFAndOperation( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    :   CCFOperationNode( aServices, aParent )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFAndOperation::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFAndOperation::ConstructL()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFAndOperation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFAndOperation* CCFAndOperation::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFAndOperation* self = NewLC( aServices, aParent );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFAndOperation::NewLC
// Two-phased constructor that leaves object to the cleanup stack.
// -----------------------------------------------------------------------------
//
CCFAndOperation* CCFAndOperation::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFAndOperation* self = new( ELeave ) CCFAndOperation( aServices, aParent );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CCFAndOperation::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFAndOperation* CCFAndOperation::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptAndName ) != 0 )
        {
        return NULL; // Cannot create and operation from the given node.
        }

    CCFAndOperation* self = NewLC( aServices, aParent );    // CLEANUP<< self

    TBool unsupportedNode( EFalse );
    CMDXMLNode* child = aNode.FirstChild();
    while ( child )
        {
        if ( child->NodeType() == CMDXMLNode::EElementNode )
            {
            CCFOperationNode* op = self->iServices.ParseL( self, *child );
            if ( op )
                {
                self->iChildren.AppendL( op );
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
    if ( unsupportedNode || !self->iChildren.Count() )
        {
        delete self; // Discard unsupported and operation.
        self = NULL;

        if ( unsupportedNode )
            {
            TPtrC nodeName( child->NodeName() );
            INFO_1( "CCFAndOperation::ParseL - Unsupported node [%S]",
                    &nodeName );
            }
        else
            {
            INFO( "CCFAndOperation::ParseL - No children" );
            }
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFAndOperation::~CCFAndOperation()
    {
    FUNC_LOG;

    iChildren.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CCFAndOperation::ActivateL
// ---------------------------------------------------------------------------
//
void CCFAndOperation::ActivateL()
    {
    FUNC_LOG;

    for ( TInt i = 0; i < iChildren.Count(); ++i )
        {
        iChildren[ i ]->ActivateL();
        }
    }

// ---------------------------------------------------------------------------
// CCFAndOperation::Deactivate
// ---------------------------------------------------------------------------
//
void CCFAndOperation::Deactivate()
    {
    FUNC_LOG;

    for ( TInt i = 0; i < iChildren.Count(); ++i )
        {
        iChildren[ i ]->Deactivate();
        }
    }

// ---------------------------------------------------------------------------
// CCFAndOperation::CheckSecurity
// ---------------------------------------------------------------------------
//
TInt CCFAndOperation::CheckSecurity()
    {
    FUNC_LOG;

    for ( TInt i = 0; i < iChildren.Count(); ++i )
        {
        TInt err = iChildren[ i ]->CheckSecurity();
        if ( err != KErrNone )
            {
            return err;
            }
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCFAndOperation::Evaluate
// -----------------------------------------------------------------------------
//
void CCFAndOperation::Evaluate()
    {
    FUNC_LOG;

    DOM_INFO( "CCFAndOperation::Evaluate" );

    // get the value of the condition
    TCFConditionValue newValue = Value();
    TBool branchUndefined = EFalse;

    // iterate through all the children
    for ( TInt i = 0; i < iChildren.Count(); ++i )
        {
        // get the child's value
        newValue = iChildren[ i ]->Value();
        if ( !newValue )
            {
            break;
            }
        else if ( newValue == ECFConditionUndefined )
            {
            branchUndefined = ETrue;
            }
        }

    if ( branchUndefined && newValue == ECFConditionTrue )
        {
        newValue = ECFConditionUndefined;
        }

    if ( newValue != Value() )
        {
        INFO_1( "CCFAndOperation::Evaluate - Value changed to (-1=undefined, 0=false, 1=true): %d", newValue );

        iValue = newValue;
        if ( iParent )
            {
            // Recursion here; hopefully tree not deep
            iParent->Evaluate();
            }
        }
    else
        {
        INFO_1( "CCFAndOperation::Evaluate - Value still (-1=undefined, 0=false, 1=true): %d", newValue );
        }
    }

// -----------------------------------------------------------------------------
// CCFAndOperation::Cleanup
// -----------------------------------------------------------------------------
//
void CCFAndOperation::Cleanup()
    {
    FUNC_LOG;

    for ( TInt i = 0; i < iChildren.Count(); ++i )
        {
        iChildren[ i ]->Cleanup();
        }
    }
