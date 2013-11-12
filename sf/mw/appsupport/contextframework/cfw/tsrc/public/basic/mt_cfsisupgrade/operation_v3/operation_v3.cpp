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
* Description:  Operation plug-in V3 implementation.
*
*/


// SYSTEM INCLUDES
#include <GMXMLNode.h>

// USER INCLUDES
#include "operation_V3.h"
#include "operationdef_V3.h"
#include "operationnode_V3.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
COperation_V3::COperation_V3( TOperationPluginInitParams* aParams ):
    CCFOperationPlugIn( aParams )
    {

    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void COperation_V3::ConstructL()
    {

    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
COperation_V3* COperation_V3::NewL( TOperationPluginInitParams* aParams )
    {
    COperation_V3* self = COperation_V3::NewLC ( aParams );
    CleanupStack::Pop ( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
COperation_V3* COperation_V3::NewLC( TOperationPluginInitParams* aParams )
    {
    COperation_V3* self = new( ELeave ) COperation_V3( aParams );
    CleanupStack::PushL ( self );
    self->ConstructL ( );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
COperation_V3::~COperation_V3( )
    {

    }

//------------------------------------------------------------------------------
// COperation_V3::ParseL
//------------------------------------------------------------------------------
//
CCFOperationNode* COperation_V3::ParseL( CCFOperationNode* aParent,
    CMDXMLNode& aDOMNode,
    MCFOperationServices& aOperationServices )
    {
    CCFOperationNode* node = NULL;
    
    // <operation_V3>
    if( aDOMNode.NodeName().CompareF( KOperationNode_V3Name ) == KErrNone )
        {
        node = COperationNode_V3::ParseL(
            aOperationServices, aParent, aDOMNode );
        }
    
    return node;
    }

//------------------------------------------------------------------------------
// COperation_V3::ParseScriptRootL
//------------------------------------------------------------------------------
//
CCFScriptRoot* COperation_V3::ParseScriptRootL( CCFOperationNode* /*aParent*/,
    CMDXMLNode& /*aDOMNode*/,
    MCFOperationServices& /*aOperationServices */)
    {
    return NULL;
    }

// End of file
