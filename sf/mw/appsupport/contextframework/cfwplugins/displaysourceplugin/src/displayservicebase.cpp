/*
* Copyright (c) 2009-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   CDisplayContextBase class implementation.
*
*/


// SYSTEM INCLUDES
#include <cfcontextobject.h>

// USER INCLUDES
#include "displayservicebase.h"
#include "displaysourcecontextdef.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CDisplayServiceBase::~CDisplayServiceBase()
    {
    FUNC_LOG;
    
    iThread.Close();
    delete iContext;
    }

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CDisplayServiceBase::CDisplayServiceBase( TDisplayServiceType aType,
    MCFContextInterface& aCF ):
    iCF( aCF ),
    iType( aType )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CDisplayServiceBase::ConstructL()
    {
    FUNC_LOG;
    
    iContext = CCFContextObject::NewL();
    }

//------------------------------------------------------------------------------
// CDisplayServiceBase::DisplayServiceType
//------------------------------------------------------------------------------
//
TInt CDisplayServiceBase::DisplayServiceType() const
    {
    FUNC_LOG;
    
    return iType;
    }

// End of file
