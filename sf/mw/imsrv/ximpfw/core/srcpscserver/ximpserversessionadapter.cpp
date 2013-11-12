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
* Description:  PSC server session adapter
 *
*/

#include "ximpserversessionadapter.h"
#include "ximpserverdefs.h"
#include "ximpsrvmessage.h"
#include "ximpcontextsession.h"
#include "ximpsrv.h"
#include <e32base.h>


//Include PSC server namespace
using namespace NXIMPSrv;



// -----------------------------------------------------------------------------
// CXIMPServerSessionAdapter::NewL()
// -----------------------------------------------------------------------------
//
CXIMPServerSessionAdapter* CXIMPServerSessionAdapter::NewL( 
    TUint32 aSessionId,
    MXIMPSrv& aServer )
    {
    CXIMPServerSessionAdapter* self = 
            new( ELeave ) CXIMPServerSessionAdapter( aSessionId,
                                                        aServer );
    return self;
    }


// -----------------------------------------------------------------------------
// CXIMPServerSessionAdapter::~CXIMPServerSessionAdapter()
// -----------------------------------------------------------------------------
//
CXIMPServerSessionAdapter::~CXIMPServerSessionAdapter()
    {
    }


// -----------------------------------------------------------------------------
// CXIMPServerSessionAdapter::CXIMPServerSessionAdapter()
// -----------------------------------------------------------------------------
//
CXIMPServerSessionAdapter::CXIMPServerSessionAdapter( 
    TUint32 aSessionId,
    MXIMPSrv& aServer )
    : CXIMPSrvSessionAdapter( aServer ),
      iSessionId( aSessionId )
    {
    }


// -----------------------------------------------------------------------------
// CXIMPServerSessionAdapter::DoInstantiateSessionL()
// -----------------------------------------------------------------------------
//
MXIMPSrvSession* CXIMPServerSessionAdapter::DoInstantiateSessionL( 
    const MXIMPSrvMessage& aMessage )
    {
    switch( aMessage.Function() )
        {
        case NRequest::EBecomeCtxSession:
            {
            return CXIMPContextSession::NewL( iSessionId );
            }
        default:
            {
            return NULL;
            }        
        }
    }

// End of file
