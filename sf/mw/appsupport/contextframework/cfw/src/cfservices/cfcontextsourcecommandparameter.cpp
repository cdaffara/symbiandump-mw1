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
* Description:  CCFContextSourceCommandParameter class implementation.
 *
*/


// SYSTEM INCLUDES
#include <s32strm.h>
#include <cfcontextsourcecommandparameter.h>

// USER INCLUDES
#include "cfcontextsourcecommandparameterimpl.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CCFContextSourceCommandParameter*
CCFContextSourceCommandParameter::NewL()
    {
    return CCFContextSourceCommandParameterImpl::NewL();
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
EXPORT_C CCFContextSourceCommandParameter*
CCFContextSourceCommandParameter::NewLC()
    {
    return CCFContextSourceCommandParameterImpl::NewLC();
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CCFContextSourceCommandParameter::~CCFContextSourceCommandParameter()
    {
    }

// End of file
