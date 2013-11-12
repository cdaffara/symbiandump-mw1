/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Testing context wrapper
 *
*/



#include <e32std.h>

#include <ximpclient.h>
#include <ximpbase.h>
#include "prfwtestcontextwrappermgr.h"
#include "prfwtestcontextwrapper.h"
#include <ximpcontext.h>
#include "prfwteststatuseventlistener.h"
#include "prfwtestmessenger.h"
#include "prfwtestmessaging.h"

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestContextWrapperMgr::CXIMPTestContextWrapperMgr() 
    {
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void CXIMPTestContextWrapperMgr::ConstructL()
    {
    // TODO
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestContextWrapperMgr* CXIMPTestContextWrapperMgr::NewL()
    {
    CXIMPTestContextWrapperMgr* self = new( ELeave ) CXIMPTestContextWrapperMgr();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestContextWrapperMgr::~CXIMPTestContextWrapperMgr()
    {
    iWrappers.Close();
    iPrClients.Close();
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestContextWrapperMgr::CreateWrapperL( TInt aClientIndex /* = KErrNotFound */ )
    {
    // make sure we've the client
    MXIMPClient* client = NULL;

    if ( aClientIndex == KErrNotFound )
        {
        if( iPrClients.Count() == 0 )
            {
            // make a new one
            client = MXIMPClient::NewClientL();
            iPrClients.AppendL( client );
            }
        else
            {
            client = iPrClients[ 0 ];
            }
        }
    else
        {
        // if the index was bad, everything will blow. it's up to the client to instruct
        // us correctly
        client = iWrappers[ aClientIndex ]->GetClient();
        }

    CXIMPTestContextWrapper* tmp = CXIMPTestContextWrapper::NewL( iIndex, client );
    iWrappers.AppendL( tmp );

    iIndex++;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestContextWrapper* CXIMPTestContextWrapperMgr::GetWrapperL( TInt aIndex )
    {
    return iWrappers[ aIndex ];
    }

EXPORT_C TInt CXIMPTestContextWrapperMgr::WrapperCount() const
    {
    return iWrappers.Count();
    } 

// End of file
