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
* Description:  CCFIfClause class implementation.
*
*/



// INCLUDE FILES
#include "cfifclause.h"
#include "cfscriptaction.h"
#include "cfbasicoptrace.h"

#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptIfName16,        "if"        );
_LIT( KScriptActionsName16,   "actions"   );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFIfClause::CCFIfClause
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFIfClause::CCFIfClause( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    :   CCFClauseNode( aServices, aParent )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFIfClause::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFIfClause::ConstructL()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFIfClause::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFIfClause* CCFIfClause::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFIfClause* self = NewLC( aServices, aParent );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFIfClause::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFIfClause* CCFIfClause::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFIfClause* self = new( ELeave ) CCFIfClause( aServices, aParent );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CCFIfClause::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFIfClause* CCFIfClause::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptIfName16 ) != 0 )
        {
        return NULL; // Cannot create if clause from the given node.
        }

    CCFIfClause* self = NewLC( aServices, aParent );    // CLEANUP<< self

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
                    INFO( "CCFIfClause::ParseL - Redefinition not allowed, actions already parsed" );
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
                    INFO_1( "CCFIfClause::ParseL - One child operation is allowed. [%S] is not supported",
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
                    INFO_1( "CCFIfClause::ParseL - Unsupported node [%S]",
                            &nodeName );
                    unsupportedNode = ETrue;
                    break;
                    }
                }
            }
        else if ( child->NodeType() != CMDXMLNode::ECommentNode )
            {
            TPtrC nodeName( child->NodeName() );
            INFO_1( "CCFIfClause::ParseL - Unsupported node [%S]",
                    &nodeName );
            unsupportedNode = ETrue;
            break;
            }
        child = child->NextSibling();
        }

    CleanupStack::Pop( self );                          // CLEANUP>> self
    if ( !self->iChild || unsupportedNode )
        {
        INFO( "CCFIfClause::ParseL - No child operation defined or unsupported node" );
        delete self; // Discard unsupported if clause.
        self = NULL;
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFIfClause::~CCFIfClause()
    {
    FUNC_LOG;

    delete iChild;
    iActions.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// CCFIfClause::FireActionsL
// -----------------------------------------------------------------------------
//
void CCFIfClause::FireActionsL()
    {
    FUNC_LOG;

    ACTION_INFO_2( "Firing [%d] IF-actions for script ID=[%d]",
            iActions.Count(),
            iServices.ScriptId() );

    for ( TInt i = 0; i < iActions.Count(); ++i )
        {
        iActions[ i ]->ActL();
        }
    }

// ---------------------------------------------------------------------------
// CCFIfClause::ActivateL
// ---------------------------------------------------------------------------
//
void CCFIfClause::ActivateL()
    {
    FUNC_LOG;

    if ( iChild )
        {
        iChild->ActivateL();
        }
    }

// ---------------------------------------------------------------------------
// CCFIfClause::Deactivate
// ---------------------------------------------------------------------------
//
void CCFIfClause::Deactivate()
    {
    FUNC_LOG;

    if ( iChild )
        {
        iChild->Deactivate();
        }
    }

// ---------------------------------------------------------------------------
// CCFIfClause::CheckSecurity
// ---------------------------------------------------------------------------
//
TInt CCFIfClause::CheckSecurity()
    {
    FUNC_LOG;

    TInt err( KErrNone );
    // Check security for context subscriptions.
    if ( iChild )
        {
        err = iChild->CheckSecurity();
        if ( err != KErrNone )
            {
            ERROR( err, "CCFIfClause::CheckSecurity - Context read security check failed!" );
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
            ERROR( err, "CCFIfClause::CheckSecurity - Getting action security policy failed!" );
            return err;
            }

        err = iServices.CheckScriptOwnerAccess( securityPolicy );
        if ( err != KErrNone )
            {
            ERROR( err, "CCFIfClause::CheckSecurity - Checking action security policy failed!" );
            return err;
            }
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCFIfClause::Evaluate
// -----------------------------------------------------------------------------
//
void CCFIfClause::Evaluate()
    {
    FUNC_LOG;

    DOM_INFO( "CCFIfClause::Evaluate" );

    if ( iChild )
        {
        // If clause has a root condition.
        TCFConditionValue newValue = iChild->Value();

        if ( newValue != Value() )
            {
            INFO_1( "CCFIfClause::Evaluate - Value changed to (-1=undefined, 0=false, 1=true): %d", newValue );

            iValue = newValue;
            if ( iParent )
                {
                iParent->Evaluate();
                }
            }
        else
            {
            INFO_1( "CCFIfClause::Evaluate - Value still (-1=undefined, 0=false, 1=true): %d", newValue );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCFIfClause::Cleanup
// -----------------------------------------------------------------------------
//
void CCFIfClause::Cleanup()
    {
    FUNC_LOG;

    if ( iChild )
        {
        iChild->Cleanup();
        }
    }
