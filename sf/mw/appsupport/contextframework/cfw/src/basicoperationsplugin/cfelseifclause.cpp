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
* Description:  CCFElseIfClause class implementation.
*
*/



// INCLUDE FILES
#include "cfelseifclause.h"
#include "cfscriptaction.h"
#include "cfbasicoptrace.h"

#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptElseIfName16,  "elseIf"    );
_LIT( KScriptActionsName16, "actions"   );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFElseIfClause::CCFElseIfClause
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFElseIfClause::CCFElseIfClause( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    :   CCFClauseNode( aServices, aParent )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFElseIfClause::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFElseIfClause::ConstructL()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFElseIfClause::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFElseIfClause* CCFElseIfClause::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFElseIfClause* self = NewLC( aServices, aParent );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFElseIfClause::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFElseIfClause* CCFElseIfClause::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFElseIfClause* self = new( ELeave ) CCFElseIfClause( aServices, aParent );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CCFElseIfClause::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFElseIfClause* CCFElseIfClause::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptElseIfName16 ) != 0 )
        {
        return NULL; // Cannot create else if clause from the given node.
        }

    CCFElseIfClause* self = NewLC( aServices, aParent );    // CLEANUP<< self

    // Parse children.
    TBool unsupportedNode( EFalse );
    TBool actionsParsed( EFalse );
    CMDXMLNode* child = aNode.FirstChild();
    while ( child )
        {
        if ( child->NodeType() == CMDXMLNode::EElementNode )
            {
            if ( child->NodeName().CompareF( KScriptActionsName16 ) == 0 )
                {
                if ( actionsParsed )
                    {
                    INFO( "CCFElseIfClause::ParseL - Redefinition not allowed, actions already parsed" );
                    unsupportedNode = ETrue;
                    break;
                    }
                self->ParseActionsL( *child, self->iActions );
                actionsParsed = ETrue;
                }
            else
                {
                if ( self->iChild )
                    {
                    TPtrC nodeName( child->NodeName() );
                    INFO_1( "CCFElseIfClause::ParseL - One child operation is allowed. [%S] is not supported",
                            &nodeName );
                    unsupportedNode = ETrue; // Already parsed the child.
                    break;
                    }
                CCFOperationNode* op = self->iServices.ParseL( self, *child );
                if ( op )
                    {
                    self->iChild = op;
                    }
                else
                    {
                    TPtrC nodeName( child->NodeName() );
                    INFO_1( "CCFElseIfClause::ParseL - Unsupported node [%S]",
                            &nodeName );
                    unsupportedNode = ETrue;
                    break;
                    }
                }
            }
        else if ( child->NodeType() != CMDXMLNode::ECommentNode )
            {
            TPtrC nodeName( child->NodeName() );
            INFO_1( "CCFElseIfClause::ParseL - Unsupported node [%S]",
                    &nodeName );
            unsupportedNode = ETrue;
            break;
            }
        child = child->NextSibling();
        }

    CleanupStack::Pop( self );                              // CLEANUP>> self
    if ( !self->iChild || unsupportedNode )
        {
        INFO( "CCFElseIfClause::ParseL - No child operation defined or unsupported node" );
        delete self; // Discard unsupported else if clause.
        self = NULL;
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFElseIfClause::~CCFElseIfClause()
    {
    FUNC_LOG;

    delete iChild;
    iActions.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// CCFElseIfClause::FireActionsL
// -----------------------------------------------------------------------------
//
void CCFElseIfClause::FireActionsL()
    {
    FUNC_LOG;

    ACTION_DOM_INFO( "CCFElseIfClause::FireActionsL" );
    ACTION_INFO_2( "Firing [%d] ELSE-IF-actions for script ID=[%d]",
            iActions.Count(),
            iServices.ScriptId() );

    for ( TInt i = 0; i < iActions.Count(); ++i )
        {
        iActions[ i ]->ActL();
        }
    }

// ---------------------------------------------------------------------------
// CCFElseIfClause::ActivateL
// ---------------------------------------------------------------------------
//
void CCFElseIfClause::ActivateL()
    {
    FUNC_LOG;

    if ( iChild )
        {
        iChild->ActivateL();
        }
    }

// ---------------------------------------------------------------------------
// CCFElseIfClause::Deactivate
// ---------------------------------------------------------------------------
//
void CCFElseIfClause::Deactivate()
    {
    FUNC_LOG;

    if ( iChild )
        {
        iChild->Deactivate();
        }
    }

// ---------------------------------------------------------------------------
// CCFElseIfClause::CheckSecurity
// ---------------------------------------------------------------------------
//
TInt CCFElseIfClause::CheckSecurity()
    {
    FUNC_LOG;

    TInt err( KErrNone );
    // Check security for context subscriptions.
    if ( iChild )
        {
        err = iChild->CheckSecurity();
        if ( err != KErrNone )
            {
            ERROR( err, "CCFElseIfClause::CheckSecurity - Context read security check failed!" );
            return err;
            }
        }

    // Check security for actions.
    for ( TInt i = 0; i < iActions.Count(); ++i )
        {
        TSecurityPolicy securityPolicy;
        TInt err = iActions[ i ]->GetSecurityPolicy( securityPolicy );
        if ( err != KErrNone )
            {
            ERROR( err, "CCFElseIfClause::CheckSecurity - Getting action security policy failed!" );
            return err;
            }

        err = iServices.CheckScriptOwnerAccess( securityPolicy );
        if ( err != KErrNone )
            {
            ERROR( err, "CCFElseIfClause::CheckSecurity - Checking action security policy failed!" );
            return err;
            }
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCFElseIfClause::Evaluate
// -----------------------------------------------------------------------------
//
void CCFElseIfClause::Evaluate()
    {
    FUNC_LOG;

    DOM_INFO( "CCFElseIfClause::Evaluate" );

    if ( iChild )
        {
        // ElseIf clause has a root condition.
        TCFConditionValue newValue = iChild->Value();

        if ( newValue != Value() )
            {
            INFO_1( "CCFElseIfClause::Evaluate - Value changed to (-1=undefined, 0=false, 1=true): %d", newValue );

            iValue = newValue;
            if ( iParent )
                {
                iParent->Evaluate();
                }
            }
        else
            {
            INFO_1( "CCFElseIfClause::Evaluate - Value still (-1=undefined, 0=false, 1=true): %d", newValue );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCFElseIfClause::Cleanup
// -----------------------------------------------------------------------------
//
void CCFElseIfClause::Cleanup()
    {
    FUNC_LOG;

    if ( iChild )
        {
        iChild->Cleanup();
        }
    }
