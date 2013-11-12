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
* Description:  Operation plug-in v2 implementation.
*
*/


// SYSTEM INCLUDES
#include <GMXMLNode.h>

// USER INCLUDES
#include "operation_v2.h"
#include "operationdef_v2.h"
#include "operationnode_v2.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
COperation_V2::COperation_V2( TOperationPluginInitParams* aParams ):
    CCFOperationPlugIn( aParams )
    {

    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void COperation_V2::ConstructL()
    {

    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
COperation_V2* COperation_V2::NewL( TOperationPluginInitParams* aParams )
    {
    COperation_V2* self = COperation_V2::NewLC ( aParams );
    CleanupStack::Pop ( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
COperation_V2* COperation_V2::NewLC( TOperationPluginInitParams* aParams )
    {
    COperation_V2* self = new( ELeave ) COperation_V2( aParams );
    CleanupStack::PushL ( self );
    self->ConstructL ( );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
COperation_V2::~COperation_V2( )
    {

    }

//------------------------------------------------------------------------------
// COperation_V2::ParseL
//------------------------------------------------------------------------------
//
CCFOperationNode* COperation_V2::ParseL( CCFOperationNode* aParent,
    CMDXMLNode& aDOMNode,
    MCFOperationServices& aOperationServices )
    {
    CCFOperationNode* node = NULL;
    
    // <operation_v2>
    if( aDOMNode.NodeName().CompareF( KOperationNode_V2Name ) == KErrNone )
        {
        node = COperationNode_V2::ParseL(
            aOperationServices, aParent, aDOMNode );
        }
    
    return node;
    }

//------------------------------------------------------------------------------
// COperation_V2::ParseScriptRootL
//------------------------------------------------------------------------------
//
CCFScriptRoot* COperation_V2::ParseScriptRootL( CCFOperationNode* /*aParent*/,
    CMDXMLNode& /*aDOMNode*/,
    MCFOperationServices& /*aOperationServices */)
    {
    return NULL;
    }

// End of file
