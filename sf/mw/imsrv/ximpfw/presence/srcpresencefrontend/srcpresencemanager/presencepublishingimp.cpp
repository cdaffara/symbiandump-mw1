/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of MPresenceManagement
 *
*/

#include "presencepublishingimp.h"
#include "presenceinfoimp.h"
#include "presenceinfofilterimp.h"
#include "ximpobjecthelpers.h"
#include "presenceoperationdefs.h"
#include "ximpcontextinternal.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresencePublishingImp::NewL()
// ---------------------------------------------------------------------------
//
CPresencePublishingImp* CPresencePublishingImp::NewL( 
    MXIMPContextInternal& aContext)
    {
    CPresencePublishingImp* self = 
        new( ELeave ) CPresencePublishingImp( aContext);

    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresencePublishingImp, 
                                     MPresencePublishing )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresencePublishingImp, 
                                           MPresencePublishing )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresencePublishingImp::~CPresencePublishingImp()
// ---------------------------------------------------------------------------
//
CPresencePublishingImp::~CPresencePublishingImp()
    {
    //iContext not own
    }


// ---------------------------------------------------------------------------
// CPresencePublishingImp::CPresencePublishingImp()
// ---------------------------------------------------------------------------
//
CPresencePublishingImp::CPresencePublishingImp(
    MXIMPContextInternal& aContext)
    : iContext( aContext)
    {
    }
    
// ---------------------------------------------------------------------------
// From class MPresencePublishing.
// CPresencePublishingImp::SubscribeOwnPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresencePublishingImp::SubscribeOwnPresenceL( 
                                            const MPresenceInfoFilter& aPif )
    {
    HBufC8* pifPack = PackPresenceInfoFilterLC( aPif );

    TXIMPRequestId reqId =
        iContext.QueueOperationL( NPresenceOps::EPrSubscribeOwnPresence,
                                   *pifPack );
    CleanupStack::PopAndDestroy( pifPack );

    return reqId;    
    }

// ---------------------------------------------------------------------------
// From class MPresencePublishing.
// CPresencePublishingImp::UnsubscribeOwnPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresencePublishingImp::UnsubscribeOwnPresenceL()
    {
    return iContext.QueueOperationL( NPresenceOps::EPrUnsubscribeOwnPresence, 
                                      KNullDesC8 );
    }

// ---------------------------------------------------------------------------
// From class MPresencePublishing.
// CPresencePublishingImp::PublishOwnPresenceL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresencePublishingImp::PublishOwnPresenceL(
    const MPresenceInfo& aPresence )
    {
    HBufC8* docPack = PackPresenceDocLC( aPresence );

    TXIMPRequestId reqId =
        iContext.QueueOperationL( NPresenceOps::EPrPublishOwnPresence,
                                   *docPack );
    CleanupStack::PopAndDestroy( docPack );

    return reqId;
    }



// ---------------------------------------------------------------------------
// From class MPresencePublishing.
// CPresencePublishingImp::SubscribePresenceWatcherListL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresencePublishingImp::SubscribePresenceWatcherListL()
    {
    return iContext.QueueOperationL(
                           NPresenceOps::EPrSubscribePresenceWatcherList,
                           KNullDesC8 );    
    }

// ---------------------------------------------------------------------------
// From class MPresencePublishing.
// CPresencePublishingImp::UnsubscribePresenceWatcherListL()
// ---------------------------------------------------------------------------
//
TXIMPRequestId CPresencePublishingImp::UnsubscribePresenceWatcherListL()
    {
    return iContext.QueueOperationL(
                           NPresenceOps::EPrUnsubscribePresenceWatcherList,
                           KNullDesC8 );    
    }


// ---------------------------------------------------------------------------
// CPresencePublishingImp::PackPresenceDocLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresencePublishingImp::PackPresenceDocLC(
    const MPresenceInfo& aPresence )
    {
    const CPresenceInfoImp* docImp = 
    TXIMPGetImpClassOrPanic< const CPresenceInfoImp >::From( aPresence );
    
    HBufC8* pack = TXIMPObjectPacker< const CPresenceInfoImp >::PackL( *docImp );
    CleanupStack::PushL( pack );

    return pack;
    }

// ---------------------------------------------------------------------------
// CPresencePublishingImp::PackPresenceDocLC()
// ---------------------------------------------------------------------------
//
HBufC8* CPresencePublishingImp::PackPresenceInfoFilterLC(
    const MPresenceInfoFilter& aPif )
    {
    const CPresenceInfoFilterImp* pifImp = 
    TXIMPGetImpClassOrPanic< const CPresenceInfoFilterImp >::From( aPif );    
    
    HBufC8* pack = TXIMPObjectPacker< CPresenceInfoFilterImp>::PackL( *pifImp );
    CleanupStack::PushL( pack );

    return pack;
    }
