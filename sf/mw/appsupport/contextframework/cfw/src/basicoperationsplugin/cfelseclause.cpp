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
* Description:  CCFElseClause class implementation.
*
*/



// INCLUDE FILES
#include "cfelseclause.h"
#include "cfscriptaction.h"
#include "cfbasicoptrace.h"

#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptElseName16,    "else"      );
_LIT( KScriptActionsName16, "actions"   );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFElseClause::CCFElseClause
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFElseClause::CCFElseClause( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    :   CCFClauseNode( aServices, aParent )
    {
    FUNC_LOG;

    iValue = ECFConditionTrue; // Else clause is always true.
    }

// -----------------------------------------------------------------------------
// CCFElseClause::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFElseClause::ConstructL()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFElseClause::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFElseClause* CCFElseClause::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFElseClause* self = NewLC( aServices, aParent );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFElseClause::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFElseClause* CCFElseClause::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFElseClause* self = new( ELeave ) CCFElseClause( aServices, aParent );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CCFElseClause::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFElseClause* CCFElseClause::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptElseName16 ) != 0 )
        {
        return NULL; // Cannot create else clause from the given node.
        }

    CCFElseClause* self = NewLC( aServices, aParent );      // CLEANUP<< self

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
                    INFO( "CCFElseClause::ParseL - Redefinition not allowed, actions already parsed" );
                    unsupportedNode = ETrue;
                    break;
                    }
                self->ParseActionsL( *child, self->iActions );
                actionsParsed = ETrue;
                }
            else
                {
                TPtrC nodeName( child->NodeName() );
                INFO_1( "CCFElseClause::ParseL - Unsupported node [%S]",
                        &nodeName );
                unsupportedNode = ETrue; // Unsupported node.
                break;
                }
            }
        else if ( child->NodeType() != CMDXMLNode::ECommentNode )
            {
            TPtrC nodeName( child->NodeName() );
            INFO_1( "CCFElseClause::ParseL - Unsupported node [%S]",
                    &nodeName );
            unsupportedNode = ETrue;
            break;
            }
        child = child->NextSibling();
        }

    CleanupStack::Pop( self );                              // CLEANUP>> self
    if ( unsupportedNode )
        {
        delete self; // Discard unsupported else clause.
        self = NULL;
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFElseClause::~CCFElseClause()
    {
    iActions.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// CCFElseClause::FireActionsL
// -----------------------------------------------------------------------------
//
void CCFElseClause::FireActionsL()
    {
    FUNC_LOG;

    ACTION_INFO_2( "Firing [%d] ELSE-actions for script ID=[%d]",
            iActions.Count(),
            iServices.ScriptId() );

    for ( TInt i = 0; i < iActions.Count(); ++i )
        {
        iActions[ i ]->ActL();
        }
    }

// ---------------------------------------------------------------------------
// CCFElseClause::ActivateL
// Else clause can have only actions, nothing must be done.
// ---------------------------------------------------------------------------
//
void CCFElseClause::ActivateL()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCFElseClause::Deactivate
// Else clause can have only actions, nothing must be done.
// ---------------------------------------------------------------------------
//
void CCFElseClause::Deactivate()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// CCFElseClause::CheckSecurity
// ---------------------------------------------------------------------------
//
TInt CCFElseClause::CheckSecurity()
    {
    FUNC_LOG;

    // Check security for actions.
    for ( TInt i = 0; i < iActions.Count(); ++i )
        {
        TSecurityPolicy securityPolicy;
        TInt err = iActions[ i ]->GetSecurityPolicy( securityPolicy );
        if ( err != KErrNone )
            {
            ERROR( err, "CCFElseClause::CheckSecurity - Getting action security policy failed!" );
            return err;
            }

        err = iServices.CheckScriptOwnerAccess( securityPolicy );
        if ( err != KErrNone )
            {
            ERROR( err, "CCFElseClause::CheckSecurity - Checking action security policy failed!" );
            return err;
            }
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCFElseClause::Evaluate
// Should never get called.
// -----------------------------------------------------------------------------
//
void CCFElseClause::Evaluate()
    {
    FUNC_LOG;

    DOM_INFO( "CCFElseClause::Evaluate (ALWAYS TRUE)" );

    iValue = ECFConditionTrue; // Else clause is always true.
    }
