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
* Description:  CCFScriptAction class implementation.
*
*/



// INCLUDES
#include "cfscriptaction.h"
#include "cfbasicoptrace.h"

#include <cfoperationservices.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFScriptAction::CCFScriptAction
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFScriptAction::CCFScriptAction( MCFOperationServices& aServices,
    TActionType aType )
    :   iServices( aServices ),
        iType( aType )
    {
    FUNC_LOG;
    }

// Destructor
CCFScriptAction::~CCFScriptAction()
    {
    FUNC_LOG;
    }


// -----------------------------------------------------------------------------
// CCFScriptAction::ActionType
// -----------------------------------------------------------------------------
//
const CCFScriptAction::TActionType& CCFScriptAction::ActionType()
    {
    FUNC_LOG;

    return iType;
    }
