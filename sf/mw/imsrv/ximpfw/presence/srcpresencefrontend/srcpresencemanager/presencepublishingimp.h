/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of MPresencePublishing
*
*/

#ifndef CPRESENCEPUBLISHINGIMP_H
#define CPRESENCEPUBLISHINGIMP_H

#include <presencepublishing.h>
#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"

class MXIMPContextInternal;


/**
 * MPresencePublishing API object implementation.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresencePublishingImp ): public CXIMPApiObjBase,
                                             public MPresencePublishing
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEPUBLISHINGIMP };


public:
    static CPresencePublishingImp* NewL( MXIMPContextInternal& aContext );
    ~CPresencePublishingImp();
    
    
public:
    CPresencePublishingImp( MXIMPContextInternal& aContext);


public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MPresencePublishing

    TXIMPRequestId SubscribeOwnPresenceL( const MPresenceInfoFilter& aPif );
    TXIMPRequestId UnsubscribeOwnPresenceL();
    TXIMPRequestId PublishOwnPresenceL( const MPresenceInfo& aPresence );
    TXIMPRequestId SubscribePresenceWatcherListL();
    TXIMPRequestId UnsubscribePresenceWatcherListL();
    TXIMPRequestId SubscribePresenceGrantRequestListL();
    TXIMPRequestId UnsubscribePresenceGrantRequestListL();


public: //Helpers

    HBufC8* PackPresenceDocLC( const MPresenceInfo& aPresence );
    HBufC8* PackPresenceInfoFilterLC( const MPresenceInfoFilter& aPif );


private: // data

    /**
     * Context session proxy for accessing context server.
     * Ref.
     */
    MXIMPContextInternal& iContext;

    };


#endif // CPRESENCEPUBLISHINGIMP_H
