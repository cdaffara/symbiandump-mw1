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
* Description:  Implementation of CRfsTestApp class
*
*/


#include "rfstestapp.h"
#include "rfstestdoc.h"
#include "trace.h"

static const TUid KAppUid = { 0x0AA01498 };

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CRfsTestApp::~CRfsTestApp
// ---------------------------------------------------------------------------
//
CRfsTestApp::~CRfsTestApp()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CRfsTestApp::CreateDocumentL
// ---------------------------------------------------------------------------
//
CApaDocument* CRfsTestApp::CreateDocumentL()
    {
    FUNC_LOG;

    return CRfsTestDoc::NewL( *this );
    }


// ---------------------------------------------------------------------------
// CRfsTestApp::AppDllUid
// ---------------------------------------------------------------------------
//
TUid CRfsTestApp::AppDllUid() const
    {
    FUNC_LOG;

    return KAppUid;
    }
