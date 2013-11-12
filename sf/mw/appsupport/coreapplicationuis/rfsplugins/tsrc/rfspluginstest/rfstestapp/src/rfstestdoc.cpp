/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CRfsTestDoc class
*
*/


#include "rfstestdoc.h"
#include "rfstestappui.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CRfsTestDoc::NewL
// ---------------------------------------------------------------------------
//
CRfsTestDoc* CRfsTestDoc::NewL( CEikApplication& aApp )
    {
    FUNC_LOG;

    return new( ELeave ) CRfsTestDoc( aApp );
    }


// ---------------------------------------------------------------------------
// CRfsTestDoc::~CRfsTestDoc
// ---------------------------------------------------------------------------
//
CRfsTestDoc::~CRfsTestDoc()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CRfsTestDoc::CRfsTestDoc
// ---------------------------------------------------------------------------
//
CRfsTestDoc::CRfsTestDoc( CEikApplication& aApp )
  : CAknDocument( aApp )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CRfsTestDoc::CreateAppUiL
// ---------------------------------------------------------------------------
//
CEikAppUi* CRfsTestDoc::CreateAppUiL()
    {
    FUNC_LOG;

    return new( ELeave ) CRfsTestAppUi;
    }
