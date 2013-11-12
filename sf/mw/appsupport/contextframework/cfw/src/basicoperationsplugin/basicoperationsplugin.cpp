/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CBasicOperationsPlugIn class implementation.
*
*/



// INCLUDE FILES
#include "basicoperationsplugin.h"
#include "cfoperationservices.h"

#include "cfbasicscriptroot.h"

#include "cfandoperation.h"
#include "cfnotoperation.h"
#include "cforoperation.h"

#include "cfequal.h"
#include "cfnotequal.h"
#include "cfless.h"
#include "cflessorequal.h"
#include "cfgreater.h"
#include "cfgreaterorequal.h"
#include "cfcontains.h"
#include "cfinrange.h"
#include "cfcontextchanged.h"
#include "cfcontextupdated.h"
#include "cfcount.h"

#include "cfbasicoptrace.h"

#include <cfscriptroot.h>
#include <gmxmlnode.h>

// CONSTANTS
_LIT( KScriptRootName,              "script"                );

_LIT( KScriptAndName,               "and"                   );
_LIT( KScriptNotName,               "not"                   );
_LIT( KScriptOrName,                "or"                    );

_LIT( KScriptEqualName,             "equal"                 );
_LIT( KScriptEqualsName,            "equals"                );
_LIT( KScriptNotEqualName,          "notEqual"              );
_LIT( KScriptLessName,              "less"                  );
_LIT( KScriptLessOrEqualName,       "lessOrEqual"           );
_LIT( KScriptGreaterName,           "greater"               );
_LIT( KScriptGreaterOrEqualName,    "greaterOrEqual"        );
_LIT( KScriptContainsName,          "contains"              );
_LIT( KScriptInRangeName,           "inRange"               );
_LIT( KScriptChangedName,           "contextChanged"        );
_LIT( KScriptUpdatedName,           "contextUpdated"        );
_LIT( KScriptCountName,             "count"                 );

//#ifdef _DEBUG
//
//// Panic category
//_LIT( KPanicCat, "CBasicOperationsPlugIn" );
//
//// Panic codes
//enum TPanicCode
//    {
//    EInvalidUid,
//    EInvalidAttribute
//    };
//
//// Local panic function
//LOCAL_C void Panic( TPanicCode aCode )
//    {
//    User::Panic( KPanicCat, aCode );
//    }
//
//#endif



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CBasicOperationsPlugIn::CBasicOperationsPlugIn
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CBasicOperationsPlugIn::CBasicOperationsPlugIn( 
	TOperationPluginInitParams* aParams )
    :   CCFOperationPlugIn( aParams )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CBasicOperationsPlugIn::ConstructL
// Symbian second-phase constructor that may leave.
// -----------------------------------------------------------------------------
//
void CBasicOperationsPlugIn::ConstructL()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CBasicOperationsPlugIn::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CBasicOperationsPlugIn* CBasicOperationsPlugIn::NewL( 
		TOperationPluginInitParams* aParams )
    {
    FUNC_LOG;

    CBasicOperationsPlugIn* self 
        = CBasicOperationsPlugIn::NewLC( aParams );

    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CBasicOperationsPlugIn::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CBasicOperationsPlugIn* CBasicOperationsPlugIn::NewLC( 
		TOperationPluginInitParams* aParams )
    {
    FUNC_LOG;

    CBasicOperationsPlugIn* self 
        = new( ELeave ) CBasicOperationsPlugIn( aParams );

    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CBasicOperationsPlugIn::~CBasicOperationsPlugIn
// -----------------------------------------------------------------------------
//
CBasicOperationsPlugIn::~CBasicOperationsPlugIn()
    {
    FUNC_LOG;
    }


// -----------------------------------------------------------------------------
// CBasicOperationsPlugIn::ParseL
// -----------------------------------------------------------------------------
//
CCFOperationNode* CBasicOperationsPlugIn::ParseL( CCFOperationNode* aParent,
    CMDXMLNode& aDOMNode,
    MCFOperationServices& aOperationServices )
    {
    FUNC_LOG;

    CCFOperationNode* op = NULL;

    if ( aDOMNode.NodeName().CompareF( KScriptAndName ) == 0 )
        {
        op = CCFAndOperation::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptOrName ) == 0 )
        {
        op = CCFOrOperation::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptNotName ) == 0 )
        {
        op = CCFNotOperation::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptEqualName ) == 0
        || aDOMNode.NodeName().CompareF( KScriptEqualsName ) == 0 )
        {
        op = CCFEqual::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptNotEqualName ) == 0 )
        {
        op = CCFNotEqual::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptLessName ) == 0 )
        {
        op = CCFLess::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptLessOrEqualName ) == 0 )
        {
        op = CCFLessOrEqual::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptGreaterName ) == 0 )
        {
        op = CCFGreater::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptGreaterOrEqualName ) == 0 )
        {
        op = CCFGreaterOrEqual::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptContainsName ) == 0 )
        {
        op = CCFContains::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptInRangeName ) == 0 )
        {
        op = CCFInRange::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptChangedName ) == 0 )
        {
        op = CCFContextChanged::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptUpdatedName ) == 0 )
        {
        op = CCFContextUpdated::ParseL( aOperationServices, aParent, aDOMNode );
        }
    else if ( aDOMNode.NodeName().CompareF( KScriptCountName ) == 0 )
        {
        op = CCFCount::ParseL( aOperationServices, aParent, aDOMNode );
        }

    return op;
    }

// -----------------------------------------------------------------------------
// CBasicOperationsPlugIn::ParseScriptRootL
// -----------------------------------------------------------------------------
//
CCFScriptRoot* CBasicOperationsPlugIn::ParseScriptRootL(
    CCFOperationNode* aParent,
    CMDXMLNode& aDOMNode, 
    MCFOperationServices& aOperationServices )
    {
    FUNC_LOG;

    CCFBasicScriptRoot* root = NULL;
    if ( aDOMNode.NodeName().CompareF( KScriptRootName ) == 0 )
        {
        root = CCFBasicScriptRoot::ParseL( aOperationServices,
                aParent,
                aDOMNode );
        }

    return root;
    }
