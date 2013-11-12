/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Operation node v1 implementation.
*
*/


// SYSTEM INCLUDES
#include <cfcontextobject.h>
#include <GMXMLNode.H>
#include <GMXMLText.h>

// USER INCLUDES
#include "operationnode_v1.h"
#include "operationdef_v1.h"

// CONSTANTS

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
COperationNode_V1::COperationNode_V1( MCFOperationServices& aServices,
    CCFOperationNode* aParent ):
    CCFOperationNode( aServices, aParent )
    {

    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void COperationNode_V1::ConstructL( const TDesC& aSource,
    const TDesC& aType,
    const TDesC& aValue )
    {
    iCo = CCFContextObject::NewL( aSource, aType, aValue );
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
COperationNode_V1* COperationNode_V1::NewL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    const TDesC& aSource,
    const TDesC& aType,
    const TDesC& aValue )
    {
    COperationNode_V1* self = COperationNode_V1::NewLC(
        aServices, aParent, aSource, aType, aValue );
    CleanupStack::Pop ( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
COperationNode_V1* COperationNode_V1::NewLC( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    const TDesC& aSource,
    const TDesC& aType,
    const TDesC& aValue )
    {
    COperationNode_V1* self = new( ELeave ) COperationNode_V1(
        aServices, aParent );
    CleanupStack::PushL ( self );
    self->ConstructL( aSource, aType, aValue );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
COperationNode_V1::~COperationNode_V1( )
    {

    }

//------------------------------------------------------------------------------
// COperationNode_V1::ParseL
//------------------------------------------------------------------------------
//
CCFOperationNode* COperationNode_V1::ParseL( MCFOperationServices& aServices,
    CCFOperationNode* aParent,
    CMDXMLNode& aNode )
    {
    TInt err = KErrNone;
    COperationNode_V1* self = NULL;
    TPtrC source( KNullDesC );
    TPtrC type( KNullDesC );
    TPtrC value( KNullDesC );

    // <operation_v1>
    if( aNode.NodeName().CompareF( KOperationNode_V1Name ) == KErrNone )
        {
        // Go through <operation_v1> node
        CMDXMLNode* child = aNode.FirstChild();
        while( child )
            {
            if( child->NodeType() == CMDXMLNode::EElementNode )
                {
                TPtrC nodeName( child->NodeName() );
                // <source>
                if( nodeName.CompareF( KOperationNode_V1SourceName ) == KErrNone )
                    {
                    ParseTextNode( *child, source );
                    }
                // <type>
                else if( nodeName.CompareF( KOperationNode_V1TypeName ) == KErrNone )
                    {
                    ParseTextNode( *child, type );
                    }
                // <value>
                else if( nodeName.CompareF( KOperationNode_V1ValueName ) == KErrNone )
                    {
                    ParseTextNode( *child, value );
                    }
                else
                    {
                    // Invalid node
                    err = KErrArgument;
                    break;
                    }
                }
            else
                {
                // Invalid node
                err = KErrArgument;
                break;
                }
            child = child->NextSibling();
            }
        }
    else
        {
        err = KErrArgument;
        }
    
    // Check that all required fields are ok
    if( err == KErrNone )
        {
        if( source.Length() && type.Length() && value.Length() )
            {
            self = NewLC( aServices, aParent, source, type, value );
            }
        else
            {
            err = KErrArgument;
            }
        }

    // Leave if error - this is also tested to check what happens if node
    // parsing leaves
    User::LeaveIfError( err );
    
    // Clean up and return
    CleanupStack::Pop( self );
    return self;
    }

//------------------------------------------------------------------------------
// COperationNode_V1::ActivateL
//------------------------------------------------------------------------------
//
void COperationNode_V1::ActivateL()
    {
    iServices.SubscribeContextL( this );
    }

//------------------------------------------------------------------------------
// COperationNode_V1::Deactivate
//------------------------------------------------------------------------------
//
void COperationNode_V1::Deactivate()
    {
    iServices.RemoveSubscription( this );
    }

//------------------------------------------------------------------------------
// COperationNode_V1::CheckSecurity
//------------------------------------------------------------------------------
//
TInt COperationNode_V1::CheckSecurity()
    {
    return KErrNone;
    }

//------------------------------------------------------------------------------
// COperationNode_V1::Evaluate
//------------------------------------------------------------------------------
//
void COperationNode_V1::Evaluate()
    {
    // nothing to do here
    }

//------------------------------------------------------------------------------
// COperationNode_V1::Evaluate
//------------------------------------------------------------------------------
//
TBool COperationNode_V1::Evaluate( const CCFContextObject& aContext,
    TInt& /*aContextLevelDelay */)
    {
    TBool evaluated = EFalse;
    
    // Set as true only if the context source, type and value matches
    if( aContext.Source().CompareF( iCo->Source() ) == KErrNone &&
        aContext.Type().CompareF( iCo->Type() ) == KErrNone &&
        aContext.Value().CompareF( iCo->Value() ) == KErrNone )
        {
        evaluated = ETrue;
        iValue = ECFConditionTrue;
        if( iParent )
            {
            iParent->Evaluate();
            }
        }
    else
        {
        iValue = ECFConditionFalse;
        }
    
    return evaluated;
    }

//------------------------------------------------------------------------------
// COperationNode_V1::Source
//------------------------------------------------------------------------------
//
TPtrC COperationNode_V1::Source() const
    {
    TPtrC source( iCo->Source() );
    return source;
    }

//------------------------------------------------------------------------------
// COperationNode_V1::Type
//------------------------------------------------------------------------------
//
TPtrC COperationNode_V1::Type() const
    {
    TPtrC type( iCo->Type() );
    return type;
    }

//------------------------------------------------------------------------------
// COperationNode_V1::IsAllRequired
//------------------------------------------------------------------------------
//
TBool COperationNode_V1::IsAllRequired() const
    {
    return EFalse;
    }

//------------------------------------------------------------------------------
// COperationNode_V1::ParseTextNode
//------------------------------------------------------------------------------
//
void COperationNode_V1::ParseTextNode( CMDXMLNode& aNode, TPtrC& aData )
    {
    CMDXMLNode* textNode = aNode.FirstChild();
    if( textNode )
        {
        if( textNode->NodeType() == CMDXMLNode::ETextNode )
            {
            CMDXMLText* text = static_cast<CMDXMLText*>( textNode );
            aData.Set( text->Data() );
            }
        }
    }

// End of file
