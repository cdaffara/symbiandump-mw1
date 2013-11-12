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
* Description:  Root server session adaptation
 *
*/

#include "ximprootserversessionadapter.h"
#include "ximprootserverdefs.h"
#include "ximpsrvmessage.h"
#include "ximprootsession.h"
#include "ximpsrv.h"

#include <e32base.h>


//Include Root server namespace
using namespace NRootSrv;


// -----------------------------------------------------------------------------
// CXIMPRootServerSessionAdapter::NewL()
// -----------------------------------------------------------------------------
//
CXIMPRootServerSessionAdapter*
    CXIMPRootServerSessionAdapter::NewL( MXIMPSrv& aServer )
    {
    CXIMPRootServerSessionAdapter* self = 
        new( ELeave ) CXIMPRootServerSessionAdapter( aServer );
    return self;
    }


// -----------------------------------------------------------------------------
// CXIMPRootServerSessionAdapter::~CXIMPRootServerSessionAdapter()
// -----------------------------------------------------------------------------
//
CXIMPRootServerSessionAdapter::~CXIMPRootServerSessionAdapter()
    {
    }


// -----------------------------------------------------------------------------
// CXIMPRootServerSessionAdapter::CXIMPRootServerSessionAdapter()
// -----------------------------------------------------------------------------
//
CXIMPRootServerSessionAdapter::CXIMPRootServerSessionAdapter( 
    MXIMPSrv& aServer ) 
    : CXIMPSrvSessionAdapter( aServer )
    {
    }


// -----------------------------------------------------------------------------
// CXIMPRootServerSessionAdapter::DoInstantiateSessionL()
// -----------------------------------------------------------------------------
//
MXIMPSrvSession* CXIMPRootServerSessionAdapter::DoInstantiateSessionL( 
    const MXIMPSrvMessage& aMessage )
    {
    switch( aMessage.Function() )
        {
        case NRequest::EBecomeRootSession:
            {
            return CXIMPRootSession::NewL();
            }
        default:
            {
            return NULL;
            }        
        }    
    }

// End of file
