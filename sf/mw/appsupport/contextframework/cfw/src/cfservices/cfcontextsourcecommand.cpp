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
* Description:  CCFContextSourceCommand class implementation.
 *
*/


// SYSTEM INCLUDES
#include <cfcontextsourcecommand.h>

// USER INCLUDES
#include "cfcontextsourcecommandimpl.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CCFContextSourceCommand* CCFContextSourceCommand::NewL()
    {
    return CCFContextSourceCommandImpl::NewL();
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
EXPORT_C CCFContextSourceCommand* CCFContextSourceCommand::NewLC()
    {
    return CCFContextSourceCommandImpl::NewLC();
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFContextSourceCommand::~CCFContextSourceCommand()
    {
    }

// End of file
