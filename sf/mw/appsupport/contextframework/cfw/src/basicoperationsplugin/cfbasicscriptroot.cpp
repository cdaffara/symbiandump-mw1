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
* Description:  CCFBasicScriptRoot class implementation.
*
*/


// Define this to get capability strings in to use
#define __INCLUDE_CAPABILITY_NAMES__

// INCLUDE FILES
#include "cfbasicscriptroot.h"
#include "cfifclause.h"
#include "cfelseifclause.h"
#include "cfelseclause.h"
#include "cfdelay.h"
#include "cfbasicoptrace.h"

#include <gmxmlelement.h>
#include <gmxmltext.h>
#include <e32capability.h>

// CONSTANTS
_LIT( KScriptRootName,                  "script"          );
_LIT( KScriptEvaluationDelayAttribute,  "evaluationDelay" );
_LIT( KScriptIfName,                    "if"              );
_LIT( KScriptElseIfName,                "elseIf"          );
_LIT( KScriptElseName,                  "else"            );
_LIT( KScriptUpgradeName,               "upgradeSecurity" );
_LIT( KScriptCapabilityName,            "capability"      );

const TInt KMillisecondsToMicroMultiplier = 1000;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::CCFBasicScriptRoot
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFBasicScriptRoot::CCFBasicScriptRoot( MCFOperationServices& aServices,
    CCFOperationNode* aParent ):
    CCFScriptRoot( aServices, aParent ),
    iCapabilitySet( ECapabilityAllFiles )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFBasicScriptRoot::ConstructL()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFBasicScriptRoot* CCFBasicScriptRoot::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFBasicScriptRoot* self = NewLC( aServices, aParent );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFBasicScriptRoot* CCFBasicScriptRoot::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    {
    FUNC_LOG;

    CCFBasicScriptRoot* self
        = new( ELeave ) CCFBasicScriptRoot( aServices, aParent );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::ParseL
// Construction with parsing from a DOM node.
// -----------------------------------------------------------------------------
//
CCFBasicScriptRoot* CCFBasicScriptRoot::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    if ( aNode.NodeName().CompareF( KScriptRootName ) != 0
        || aNode.NodeType() != CMDXMLNode::EElementNode )
        {
        return NULL; // Cannot create script root from the given node.
        }

    CCFBasicScriptRoot* self = NewLC( aServices, aParent ); // CLEANUP<< self

    // Try getting evaluation delay.
    CMDXMLElement& element = static_cast< CMDXMLElement& >( aNode );
    TPtrC evaluationDelay;
    if ( KErrNone == element.GetAttribute( KScriptEvaluationDelayAttribute,
            evaluationDelay ) )
        {
        if ( evaluationDelay.Length() )
            {
            // Convert delay to integer.
            TLex parseInt( evaluationDelay );
            parseInt.SkipSpace();
            TInt err = parseInt.Val( self->iScriptLevelDelay );
            if ( err != KErrNone )
                {
                ERROR_1( err, "CCFBasicScriptRoot::ParseL - Converting script level evaluation delay failed for [%S]",
                        &evaluationDelay );
                User::LeaveIfError( err );
                }
            self->iDelayActionFiring = CCFDelay::NewL( *self );
            }
        }

    TBool ifFound( EFalse );
    TBool elseFound( EFalse );
    TBool unsupportedNode( EFalse );
    // Parse clause nodes (children).
    CMDXMLNode* child = aNode.FirstChild();
    while ( child )
        {
        TPtrC nodeName( child->NodeName() );
        if ( child->NodeType() == CMDXMLNode::EElementNode )
            {
            if ( !ifFound )
                {
                // <upgradeSecurity>
                if ( nodeName.CompareF( KScriptUpgradeName ) == 0 )
                    {
                    ResolveSecurity( *self, *child );
                    }
                // <if>
                else
                    {
                    if ( nodeName.CompareF( KScriptIfName ) != 0 )
                        {
                        TPtrC nodeName( child->NodeName() );
                        ERROR_GEN_1( "CCFBasicScriptRoot::ParseL - Expecting if, encountered %S",
                            &nodeName );
                        unsupportedNode = ETrue;
                        break;
                        }
                    ifFound = ETrue;
                    CCFClauseNode* clause
                        = CCFIfClause::ParseL( aServices, self, *child );
                    if ( clause )
                        {
                        self->iChildren.AppendL( clause );
                        }
                    else
                        {
                        ERROR_GEN( "CCFBasicScriptRoot::ParseL - Unknown if clause" );
                        unsupportedNode = ETrue;
                        break;
                        }
                    }
                }
            else
                {
                // <elseIf>
                if ( nodeName.CompareF( KScriptElseIfName ) == 0 )
                    {
                    if ( elseFound )
                        {
                        ERROR_GEN( "CCFBasicScriptRoot::ParseL - Clause sequence error, elseIf defined after else clause" );
                        unsupportedNode = ETrue;
                        break;
                        }
                    CCFClauseNode* clause
                        = CCFElseIfClause::ParseL( aServices, self, *child );
                    if ( clause )
                        {
                        self->iChildren.AppendL( clause );
                        }
                    else
                        {
                        ERROR_GEN( "CCFBasicScriptRoot::ParseL - Unknown elseIf clause" );
                        unsupportedNode = ETrue;
                        break;
                        }
                    }
                // <else>
                else if ( nodeName.CompareF( KScriptElseName ) == 0 )
                    {
                    if ( elseFound )
                        {
                        ERROR_GEN( "CCFBasicScriptRoot::ParseL - Only one else clause supported" );
                        unsupportedNode = ETrue;
                        break;
                        }
                    elseFound = ETrue;
                    CCFClauseNode* clause
                        = CCFElseClause::ParseL( aServices, self, *child );
                    if ( clause )
                        {
                        self->iChildren.AppendL( clause );
                        }
                    else
                        {
                        ERROR_GEN( "CCFBasicScriptRoot::ParseL - Unknown else clause" );
                        unsupportedNode = ETrue;
                        break;
                        }
                    }
                else
                    {
                    ERROR_GEN_1( "CCFBasicScriptRoot::ParseL - Expecting elseIf or else, encountered %S",
                        &nodeName );
                    unsupportedNode = ETrue;
                    break;
                    }
                }
            }
        else if ( child->NodeType() != CMDXMLNode::ECommentNode )
            {
            ERROR_GEN_1( "CCFBasicScriptRoot::ParseL - Unsupported node [%S]",
                    &nodeName );
            unsupportedNode = ETrue;
            break;
            }
        child = child->NextSibling();
        }

    CleanupStack::Pop( self );                              // CLEANUP>> self
    if ( unsupportedNode )
        {
        delete self;
        self = NULL; // Signal inability to parse.
        }

    CREATE_DOM_INFO( self, aNode );

    return self;
    }


// Destructor
CCFBasicScriptRoot::~CCFBasicScriptRoot()
    {
    FUNC_LOG;

    delete iDelayActionFiring;
    iChildren.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::ContextEvaluatedL
// -----------------------------------------------------------------------------
//
void CCFBasicScriptRoot::ContextEvaluatedL( TInt aContextLevelDelay )
    {
    FUNC_LOG;

    INFO_1( "CCFBasicScriptRoot::ContextEvaluatedL - Root value is (-1=undefined, 0=false, 1=true): %d", iValue );

    // Delay if required
    if ( iScriptLevelDelay || aContextLevelDelay )
        {
        TTimeIntervalMicroSeconds32 delay( 0 );
        if ( aContextLevelDelay )
            {
            delay = aContextLevelDelay * KMillisecondsToMicroMultiplier;
            }
        else
            {
            delay = iScriptLevelDelay * KMillisecondsToMicroMultiplier;
            }

        if ( !iDelayActionFiring )
            {
            iDelayActionFiring = CCFDelay::NewL( *this );
            }
        iDelayActionFiring->Delay( delay );

        INFO( "CCFBasicScriptRoot::ContextEvaluatedL - Action firing delayed" );
        }
    else if ( Value() == CCFOperationNode::ECFConditionTrue
        && ( !iDelayActionFiring || !( iDelayActionFiring->IsActive() ) ) )
        {
        FireActionsL();
        }
    }

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::EvaluatedL
// Cancel evaluation delay if firing actions.
// -----------------------------------------------------------------------------
//
void CCFBasicScriptRoot::EvaluatedL()
    {
    FUNC_LOG;

    INFO_1( "CCFBasicScriptRoot::EvaluatedL - Root value is (-1=undefined, 0=false, 1=true): %d", iValue );

    if ( Value() == ECFConditionTrue )
        {
        if ( iDelayActionFiring )
            {
            iDelayActionFiring->Cancel();
            }
        FireActionsL();
        }
    }

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::UpgradeSecurity
// -----------------------------------------------------------------------------
//
const TCapabilitySet& CCFBasicScriptRoot::UpgradeSecurity() const
    {
    FUNC_LOG;

    return iCapabilitySet;
    }

// ---------------------------------------------------------------------------
// CCFBasicScriptRoot::ActivateL
// ---------------------------------------------------------------------------
//
void CCFBasicScriptRoot::ActivateL()
    {
    FUNC_LOG;

    for ( TInt i = 0; i < iChildren.Count(); ++i )
        {
        iChildren[ i ]->ActivateL();
        }
    }

// ---------------------------------------------------------------------------
// CCFBasicScriptRoot::Deactivate
// ---------------------------------------------------------------------------
//
void CCFBasicScriptRoot::Deactivate()
    {
    FUNC_LOG;

    for ( TInt i = 0; i < iChildren.Count(); ++i )
        {
        iChildren[ i ]->Deactivate();
        }
    }

// ---------------------------------------------------------------------------
// CCFBasicScriptRoot::CheckSecurity
// ---------------------------------------------------------------------------
//
TInt CCFBasicScriptRoot::CheckSecurity()
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
// CCFBasicScriptRoot::Cleanup
// -----------------------------------------------------------------------------
//
void CCFBasicScriptRoot::Cleanup()
    {
    FUNC_LOG;

    for ( TInt i = 0; i < iChildren.Count(); ++i )
        {
        iChildren[ i ]->Cleanup();
        }
    }

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::ExpiredL
// -----------------------------------------------------------------------------
//
void CCFBasicScriptRoot::ExpiredL()
    {
    FUNC_LOG;

    INFO_1( "CCFBasicScriptRoot::ExpiredL - Action firing delay expired, root value is (-1=undefined, 0=false, 1=true): %d", iValue );

    if ( Value() == ECFConditionTrue )
        {
        FireActionsL();
        }
    }


// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::Evaluate
// -----------------------------------------------------------------------------
//
void CCFBasicScriptRoot::Evaluate()
    {
    FUNC_LOG;

    DOM_INFO( "CCFBasicScriptRoot::Evaluate" );

    TCFConditionValue newValue = Value();
    TBool branchUndefined = EFalse;

    // New value is determined like for the OR operation.
    for ( TInt i = 0; i < iChildren.Count(); ++i )
        {
        newValue = iChildren[ i ]->Value();
        if ( newValue == ECFConditionTrue )
            {
            break;
            }
        else if ( newValue == ECFConditionUndefined )
            {
            branchUndefined = ETrue;
            }
        }

    if ( branchUndefined && newValue == ECFConditionFalse )
        {
        newValue = ECFConditionUndefined;
        }

    if ( newValue != Value() )
        {
        INFO_1( "CCFBasicScriptRoot::Evaluate - Value changed to (-1=undefined, 0=false, 1=true): %d", newValue );

        iValue = newValue;
        if ( iParent )
            {
            iParent->Evaluate(); // Script root should not have parents.
            }
        }
    else
        {
        INFO_1( "CCFBasicScriptRoot::Evaluate - Value still (-1=undefined, 0=false, 1=true): %d", newValue );
        }
    }

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::FireActionsL
// Fires actions of the first clause node having true value.
// -----------------------------------------------------------------------------
//
void CCFBasicScriptRoot::FireActionsL()
    {
    FUNC_LOG;

    // Fires actions of the first clause having true value.
    for ( TInt i = 0; i < iChildren.Count(); ++i )
        {
        if ( iChildren[ i ]->Value() == ECFConditionTrue )
            {
            iChildren[ i ]->FireActionsL();
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::SetCapabilitites
// -----------------------------------------------------------------------------
//
void CCFBasicScriptRoot::SetCapabilitites( TCapabilitySet& aSet )
    {
    FUNC_LOG;
    
    iCapabilitySet = aSet;
    }

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::CapabilityFromDesC
// -----------------------------------------------------------------------------
//
TCapability CCFBasicScriptRoot::CapabilityFromDesC(
    const TDesC8& aCapDesc )
    {
    FUNC_LOG;

    TCapability capability = ECapability_None;
    for( TInt i = 0; i < ECapability_Limit; i++ )
        {
        const char* capName = CapabilityNames[i];
        TPtrC8 capabilityPtr( ( TUint8* )capName );
        if( capabilityPtr.CompareF( aCapDesc ) == KErrNone )
            {
            // Found capability
            capability = ( TCapability )i;
            break;
            }
        }
    
    return capability;
    }

// -----------------------------------------------------------------------------
// CCFBasicScriptRoot::ResolveSecurity
// -----------------------------------------------------------------------------
//
void CCFBasicScriptRoot::ResolveSecurity( CCFBasicScriptRoot& aRoot,
    CMDXMLNode& aNode )
    {
    FUNC_LOG;
    
    TBool capabilitiesFound = EFalse;

    TCapabilitySet capabilitySet;
    capabilitySet.SetEmpty();

    // Get capabilities (security level)
    HBufC8* textNodeData = HBufC8::New( KMaxFileName );
    if( textNodeData )
        {
        TPtr8 textNodeDataPtr( textNodeData->Des() );
        CMDXMLNode* capabilityNode = aNode.FirstChild();
        while( capabilityNode )
            {
            // <capability>
            if( capabilityNode->NodeName().CompareF(
                KScriptCapabilityName ) == KErrNone )
                {
                if( capabilityNode->NodeType() == CMDXMLNode::EElementNode )
                    {
                    CMDXMLNode* child = capabilityNode->FirstChild();
                    if( child->NodeType() == CMDXMLNode::ETextNode )
                        {
                        CMDXMLText* textNode = static_cast<CMDXMLText*>( child );
                        textNodeDataPtr.Copy( textNode->Data() );
                        capabilitySet.AddCapability( CapabilityFromDesC(
                            textNodeDataPtr ) );
                        capabilitiesFound = ETrue;
                        }
                    }
                }
            capabilityNode = capabilityNode->NextSibling();
            }
        
        delete textNodeData;
        textNodeData = NULL;
        }
    
    // Check if the script does not have an upgrade security level set
    if( !capabilitiesFound )
        {
        // Set AllFiles capability
        capabilitySet.Set( ECapabilityAllFiles );
        }
    aRoot.SetCapabilitites( capabilitySet );
    }

// End of file
