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
* Description:  CCFClauseNode class implementation.
*
*/



// INCLUDES
#include "cfclausenode.h"
#include "cfscriptaction.h"
#include "cfnotifyaction.h"
#include "cfpublishcontextaction.h"
#include "cfsourcecommandaction.h"
#include "cfbasicoptrace.h"

#include <cfcontextinterface.h>
#include <cfcontextobject.h>
#include <cfkeyvaluepair.h>
#include <cfcontextsourcecommand.h>
#include <cfcontextsourcecommandparameter.h>
#include <gmxmlnode.h>
#include <gmxmlelement.h>

// CONSTANTS
_LIT( KScriptActionsName,               "actions"               );

_LIT( KScriptPublishContextName,        "publishContext"        );
_LIT( KScriptDefineName,                "define"                );
_LIT( KScriptTrueName,                  "true"                  );
_LIT( KScriptContextRefName,            "contextRef"            );
_LIT( KScriptSourceAttribute,           "source"                );
_LIT( KScriptTypeAttribute,             "type"                  );
_LIT( KScriptValueAttribute,            "value"                 );
_LIT( KScriptSourceCommandActionName,   "sourceCommand"         );

// ======== LOCAL FUNCTIONS ========

// Cleans up RKeyValueArray instance.
LOCAL_C void CleanupKeyValueArray( TAny* aArray )
    {
    static_cast< RKeyValueArray* >( aArray )->ResetAndDestroy();
    }

// Cleanup item for RKeyValueArray object.
LOCAL_C void CleanupResetAndDestroyPushL( RKeyValueArray& aArray )
    {
    TCleanupItem item( CleanupKeyValueArray, &aArray );
    CleanupStack::PushL( item );
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCFClauseNode::CCFClauseNode
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CCFClauseNode::CCFClauseNode( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    : CCFOperationNode( aServices, aParent )
    {
    FUNC_LOG;
    }


// Destructor
CCFClauseNode::~CCFClauseNode()
    {
    FUNC_LOG;
    }


// -----------------------------------------------------------------------------
// CCFClauseNode::ParseActionsL
// -----------------------------------------------------------------------------
//
void CCFClauseNode::ParseActionsL( CMDXMLNode& aNode,
    RPointerArray< CCFScriptAction >& aActions )
    {
    FUNC_LOG;

    if ( aNode.NodeName().CompareF( KScriptActionsName ) != 0 )
        {
        TPtrC nodeName( aNode.NodeName() );
        ERROR_GEN_1( "CCFClauseNode::ParseActionsL - Unknown actions name [%S]",
                &nodeName );
        User::Leave( KErrNotSupported );
        }

    // Parse actions.
    CMDXMLNode* actionNode = aNode.FirstChild();
    while ( actionNode )
        {
        if ( actionNode->NodeType() == CMDXMLNode::EElementNode )
            {
            if ( actionNode->NodeName().CompareF( 
                    KScriptPublishContextName ) == 0 )
                {
                CMDXMLNode* publishNode = actionNode->FirstChild();
                while ( publishNode )
                    {
                    if ( publishNode->NodeType() == CMDXMLNode::EElementNode
                        && publishNode->NodeName().CompareF(
                                KScriptContextRefName ) == 0 )
                        {
                        CCFScriptAction* action
                            = ParsePublishContextActionL( *publishNode );
                        if ( action )
                            {
                            CleanupStack::PushL( action );  // CLEANUP<< action
                            aActions.AppendL( action );
                            CleanupStack::Pop( action );    // CLEANUP>> action
                            }
                        else
                            {
                            TPtrC nodeName( publishNode->NodeName() );
                            ERROR_GEN_1( "CCFClauseNode::ParseActionsL - Skipping publish context action node [%S]",
                                    &nodeName );
                            }
                        }
                    else if ( publishNode->NodeType()
                        != CMDXMLNode::ECommentNode )
                        {
                        TPtrC nodeName( publishNode->NodeName() );
                        ERROR_GEN_1( "CCFClauseNode::ParseActionsL - Unknown publish context action node [%S]",
                                &nodeName );
                        User::Leave( KErrNotSupported );
                        }
                    publishNode = publishNode->NextSibling();
                    }
                }
            else
                {
                CCFScriptAction* action = NULL;
                if ( actionNode->NodeName().CompareF(
                        KScriptSourceCommandActionName ) == 0 )
                    {
                    action = ParseSourceCommandActionL( *actionNode );
                    }
                else
                    {
                    action = ParseNotifyActionL( *actionNode );
                    }

                if ( action )
                    {
                    CleanupStack::PushL( action );          // CLEANUP<< action
                    aActions.AppendL( action );
                    CleanupStack::Pop( action );            // CLEANUP>> action
                    }
                else
                    {
                    TPtrC nodeName( actionNode->NodeName() );
                    ERROR_GEN_1( "CCFClauseNode::ParseActionsL - Skipping action node [%S]",
                            &nodeName );
                    }
                }
            }
        actionNode = actionNode->NextSibling();
        }
    }

// -----------------------------------------------------------------------------
// CCFClauseNode::ParseNotifyActionL
// -----------------------------------------------------------------------------
//
CCFScriptAction* CCFClauseNode::ParseNotifyActionL( CMDXMLNode& aNode )
    {
    FUNC_LOG;

    // Notify action.
    // below                                    // CLEANUP<< notifyAction
    HBufC* notifyAction = aNode.NodeName().AllocLC();

    INFO_1( "Reading action definition for [%S]", &( *notifyAction ) );

    RKeyValueArray keysAndValues;
    CleanupResetAndDestroyPushL( keysAndValues ); // CLEANUP<< keysAndValues
    if ( aNode.NodeType() == CMDXMLNode::EElementNode )
        {
        // Set attributes for the action.
        CMDXMLElement& element = static_cast< CMDXMLElement& >( aNode );
        TInt numAttributes = element.NumAttributes();
        TPtrC attributeName( KNullDesC );
        TPtrC attributeValue( KNullDesC );
        for ( TInt i = 0; i < numAttributes; ++i )
            {
            if ( KErrNone == element.AttributeDetails(
                    i, attributeName, attributeValue ) )
                {
                INFO_2( "Found action attribute: '%S'='%S'",
                    &attributeName,
                    &attributeValue );

                // below                        // CLEANUP<< keyValue
                CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC(
                    attributeName,
                    attributeValue );
                keysAndValues.AppendL( keyValue );
                CleanupStack::Pop( keyValue );  // CLEANUP>> keyValue
                }
            }
        }

    CCFScriptAction* action = CCFNotifyAction::NewL( iServices,
            *notifyAction,
            keysAndValues );

    INFO_1( "Created Notify Action for [%S]", &( *notifyAction ) );

    CleanupStack::PopAndDestroy( &keysAndValues );// CLEANUP>> keysAndValues
    CleanupStack::PopAndDestroy( notifyAction );// CLEANUP>> notifyAction

    return action;
    }

// -----------------------------------------------------------------------------
// CCFClauseNode::ParsePublishContextActionL
// -----------------------------------------------------------------------------
//
CCFScriptAction* CCFClauseNode::ParsePublishContextActionL( CMDXMLNode& aNode )
    {
    FUNC_LOG;

    CCFScriptAction* action = NULL;
    CMDXMLNode* parentNode = aNode.ParentNode();
    if ( parentNode
        && parentNode->NodeName().CompareF( KScriptPublishContextName ) == 0
        && aNode.NodeName().CompareF( KScriptContextRefName ) == 0 )
        {
        // Publish context action.
        if ( aNode.NodeType() != CMDXMLNode::EElementNode )
            {
            TPtrC nodeName( aNode.NodeName() );
            ERROR_GEN_1( "CCFClauseNode::ParsePublishContextActionL - Unknown publish context action node [%S]",
                    &nodeName );
            User::Leave( KErrNotSupported );
            }

        TPtrC contextSource;
        TPtrC contextType;
        TPtrC contextValue;
        CMDXMLElement& element = static_cast< CMDXMLElement& >( aNode );
        element.GetAttribute( KScriptSourceAttribute, contextSource );
        element.GetAttribute( KScriptTypeAttribute, contextType );
        element.GetAttribute( KScriptValueAttribute, contextValue );
        if ( contextSource.Length() == 0 || contextType.Length() == 0
            || contextValue.Length() == 0 )
            {
            ERROR_GEN( "CCFClauseNode::ParsePublishContextActionL - Publish context action contextRef element is missing attributes" );
            User::Leave( KErrNotSupported );
            }

        CCFContextObject* co = CCFContextObject::NewLC();   // CLEANUP<< co
        co->SetSourceL( contextSource );
        co->SetTypeL( contextType );
        co->SetValueL( contextValue );
        co->SetTimestampToHomeTime();
        
        // Check from the parent node '<publishContext>' if there are attributes
        // set for 'define'
        TBool autoDefine( EFalse );
        if( parentNode->NodeType() == CMDXMLNode::EElementNode )
            {
            // Check if the attribute is 'define'
            CMDXMLElement& parentElement = static_cast< CMDXMLElement& >( *parentNode );
            TInt err( KErrNone );
            TPtrC name( KNullDesC );
            TPtrC value( KNullDesC );
            TInt numOfAttribs( parentElement.NumAttributes() );
            for( TInt i = 0; i < numOfAttribs; i++ )
                {
                err = parentElement.AttributeDetails( i, name, value );
                if( err == KErrNone )
                    {
                    // Check for 'define' attribute
                    if( name.CompareF( KScriptDefineName ) == KErrNone )
                        {
                        // Check if the value is 'true'
                        if( value.CompareF( KScriptTrueName ) == KErrNone )
                            {
                            autoDefine = ETrue;
                            }
                        }
                    }
                }
            }
        
        // Create a new action object giving it context object ownership.
        action = CCFPublishContextAction::NewL( iServices, co, autoDefine );
        CleanupStack::Pop( co );                            // CLEANUP>> co

        INFO_4( "Created PublishContext Action for [%S: %S: %S], Auto define: [%d]",
                &contextSource, &contextType, &contextValue, autoDefine );
        }
    else
        {
        TPtrC nodeName( aNode.NodeName() );
        if ( parentNode )
            {
            nodeName.Set( parentNode->NodeName() );
            }
        ERROR_GEN_1( "CCFClauseNode::ParsePublishContextActionL - Unknown publish context action node [%S]",
                &nodeName );
        }

    return action;
    }

// -----------------------------------------------------------------------------
// CCFClauseNode::ParseSourceCommandActionL
// -----------------------------------------------------------------------------
//
CCFScriptAction* CCFClauseNode::ParseSourceCommandActionL( CMDXMLNode& aNode )
    {
    FUNC_LOG;

    TPtrC commandNodeName( aNode.NodeName() );
    if ( commandNodeName.CompareF( KScriptSourceCommandActionName ) != 0 )
        {
        ERROR_GEN_1( "CCFClauseNode::ParseSourceCommandActionL - Unknown source command action node [%S]",
                &commandNodeName );
        return NULL;
        }

    INFO_1( "Reading action definition for [%S]", &commandNodeName );

    RKeyValueArray keysAndValues;
    CleanupResetAndDestroyPushL( keysAndValues );   // CLEANUP<< keysAndValues

    GetAttributesL( aNode, keysAndValues );

    // below                                        // CLEANUP<< command
    CCFContextSourceCommand* command = CCFContextSourceCommand::NewLC();
    command->SetNameL( commandNodeName );
    command->AddAttributesL( keysAndValues );

    // Get command parameters.
    CMDXMLNode* paramNode = aNode.FirstChild();
    while ( paramNode )
        {
        TPtrC paramNodeName( paramNode->NodeName() );
        if ( paramNode->NodeType() == CMDXMLNode::EElementNode )
            {
            GetAttributesL( *paramNode, keysAndValues );

            // below                                // CLEANUP<< param
            CCFContextSourceCommandParameter* param
                = CCFContextSourceCommandParameter::NewLC();
            param->SetNameL( paramNodeName );
            param->AddAttributesL( keysAndValues );

            GetNestedSourceCommandParametersL( *paramNode, *param );

            command->AddParameterL( param );

            CleanupStack::Pop( param );             // CLEANUP>> param
            }
        else if ( paramNode->NodeType() != CMDXMLNode::ECommentNode )
            {
            ERROR_GEN_1( "CCFClauseNode::ParseSourceCommandActionL - Unsupported node type [%S]",
                    &paramNodeName );
            User::Leave( KErrNotSupported );
            }
        paramNode = paramNode->NextSibling();
        }

    // Create command's sender identity.
    TCFSourceCommandSenderId senderId;
    RThread thread;
    senderId.iSender = thread.SecureId();
    thread.Close();
    senderId.iScriptId = iServices.ScriptId();
    command->SetSender( senderId );

    CCFSourceCommandAction* action = CCFSourceCommandAction::NewL( iServices,
            command );

    CleanupStack::Pop( command );                   // CLEANUP>> command
    CleanupStack::PopAndDestroy( &keysAndValues );  // CLEANUP>> keysAndValues

    INFO_1( "Created Source Command Action for [%S]", &commandNodeName );

    return action;
    }

// -----------------------------------------------------------------------------
// CCFClauseNode::GetAttributesL
// -----------------------------------------------------------------------------
//
void CCFClauseNode::GetAttributesL( CMDXMLNode& aNode,
    RKeyValueArray& aAttributes )
    {
    FUNC_LOG;

    if ( aNode.NodeType() == CMDXMLNode::EElementNode )
        {
        CMDXMLElement& element = static_cast< CMDXMLElement& >( aNode );
        TInt numAttributes = element.NumAttributes();
        TPtrC attributeName( KNullDesC );
        TPtrC attributeValue( KNullDesC );
        for ( TInt i = 0; i < numAttributes; ++i )
            {
            if ( KErrNone == element.AttributeDetails(
                    i, attributeName, attributeValue ) )
                {
                INFO_2( "Found action attribute: '%S'='%S'",
                    &attributeName,
                    &attributeValue );

                // below                        // CLEANUP<< keyValue
                CCFKeyValuePair* keyValue = CCFKeyValuePair::NewLC(
                    attributeName,
                    attributeValue );
                aAttributes.AppendL( keyValue );
                CleanupStack::Pop( keyValue );  // CLEANUP>> keyValue
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CCFClauseNode::GetNestedSourceCommandParametersL
// -----------------------------------------------------------------------------
//
void CCFClauseNode::GetNestedSourceCommandParametersL( CMDXMLNode& aNode,
    CCFContextSourceCommandParameter& aParent )
    {
    FUNC_LOG;

    RKeyValueArray keysAndValues;
    CleanupResetAndDestroyPushL( keysAndValues );   // CLEANUP<< keysAndValues

    CMDXMLNode* nestedNode = aNode.FirstChild();
    while ( nestedNode )
        {
        TPtrC nestedNodeName( nestedNode->NodeName() );
        if ( nestedNode->NodeType() == CMDXMLNode::EElementNode )
            {
            GetAttributesL( *nestedNode, keysAndValues );

            // below                                // CLEANUP<< parameter
            CCFContextSourceCommandParameter* parameter
                = CCFContextSourceCommandParameter::NewLC();
            parameter->SetNameL( nestedNodeName );
            parameter->AddAttributesL( keysAndValues );

            aParent.AddParameterL( parameter );

            CleanupStack::Pop( parameter );         // CLEANUP>> parameter

            GetNestedSourceCommandParametersL( *nestedNode, *parameter );
            }
        else if ( nestedNode->NodeType() != CMDXMLNode::ECommentNode )
            {
            ERROR_GEN_1( "CCFClauseNode::GetNestedSourceCommandParameterL - Unsupported node type [%S]",
                    &nestedNodeName );
            User::Leave( KErrNotSupported );
            }
        nestedNode = nestedNode->NextSibling();
        }

    CleanupStack::PopAndDestroy( &keysAndValues );  // CLEANUP>> keysAndValues
    }
