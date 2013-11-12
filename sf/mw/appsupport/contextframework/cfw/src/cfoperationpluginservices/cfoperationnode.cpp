/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*
*/


// INCLUDE FILES
#include "cfoperationnode.h"
#include "cftrace.h"

#include <gmxmlnode.h>
#include <gmxmldomconstants.h>

// CONSTANTS
static const TInt KDomTreeLocationInfoSize = 64;
static const TInt KInfoPrintExtraLen = 4;

_LIT( KDebugLocationSeparator, "." );
_LIT( KDebugLocationNameStart, "[" );
_LIT( KDebugLocationNameEnd, "]" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFOperationNode::CCFOperationNode
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CCFOperationNode::CCFOperationNode( MCFOperationServices& aServices,
    CCFOperationNode* aParent )
    :   iServices( aServices ),
        iParent( aParent ),
        iValue( ECFConditionUndefined )
    {
    FUNC_LOG;
    }


// Destructor
EXPORT_C CCFOperationNode::~CCFOperationNode()
    {
    FUNC_LOG;

    delete iDomTreeLocationInfo;
    }


// -----------------------------------------------------------------------------
// CCFOperationNode::Value
// -----------------------------------------------------------------------------
//
EXPORT_C CCFOperationNode::TCFConditionValue CCFOperationNode::Value() const
    {
    FUNC_LOG;

    return iValue;
    }

// -----------------------------------------------------------------------------
// CCFOperationNode::InternalizeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CCFOperationNode::InternalizeL( RReadStream& /*aStream*/ )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFOperationNode::ExternalizeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CCFOperationNode::ExternalizeL( RWriteStream& /*aStream*/ )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFOperationNode::Cleanup
// -----------------------------------------------------------------------------
//
EXPORT_C void CCFOperationNode::Cleanup()
	{
	FUNC_LOG;
	}

// -----------------------------------------------------------------------------
// CCFOperationNode::GenerateDomTreeLocationInfoL
// -----------------------------------------------------------------------------
//
EXPORT_C void CCFOperationNode::GenerateDomTreeLocationInfoL(
    CMDXMLNode& aNode )
    {
    FUNC_LOG;

    iDomTreeLocationInfo = HBufC::NewL( KDomTreeLocationInfoSize );
    TPtr info( iDomTreeLocationInfo->Des() );
    CMDXMLNode* domNode = &aNode;

    while ( domNode && domNode->NodeName() != KXMLDocumentElementNodeName )
        {
        TPtrC nodeName( domNode->NodeName() );
        TInt sequenceNumber( 1 );
        TBuf< 16 > seqNumBuffer;

        CMDXMLNode* prevSibling = domNode->PreviousSibling();
        while ( prevSibling )
            {
            if ( prevSibling->NodeType() != CMDXMLNode::ECommentNode )
                {
                ++sequenceNumber;
                }
            prevSibling = prevSibling->PreviousSibling();
            }
        seqNumBuffer.AppendNum( sequenceNumber );

        TInt requiredLen = nodeName.Length() + seqNumBuffer.Length()
                + KInfoPrintExtraLen;
        TInt freeLen = info.MaxLength() - info.Length();
        if ( requiredLen > freeLen )
            {
            iDomTreeLocationInfo = iDomTreeLocationInfo->ReAllocL(
                    info.MaxLength() + KDomTreeLocationInfoSize );
            info.Set( iDomTreeLocationInfo->Des() );
            }

        if ( domNode != &aNode )
            {
            info.Insert( 0, KDebugLocationSeparator );
            }

        info.Insert( 0, KDebugLocationNameEnd );
        info.Insert( 0, nodeName );
        info.Insert( 0, KDebugLocationNameStart );
        info.Insert( 0, KDebugLocationSeparator );
        info.Insert( 0, seqNumBuffer );

        domNode = domNode->ParentNode();
        }
    }

// -----------------------------------------------------------------------------
// CCFOperationNode::Extension
// -----------------------------------------------------------------------------
//
EXPORT_C TAny* CCFOperationNode::Extension( const TUid& /*aUid*/ ) const
    {
    FUNC_LOG;
    
    return NULL;
    }
