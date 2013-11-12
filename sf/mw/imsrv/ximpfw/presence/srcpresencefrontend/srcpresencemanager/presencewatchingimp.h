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
* Description:  Implementation of MPresenceWatching
*
*/

#ifndef CPRESENCEWATCHINGIMP_H
#define CPRESENCEWATCHINGIMP_H

#include <presencewatching.h>
#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"

class MXIMPContextInternal;
class MXIMPIdentity;
class MPresenceInfoFilter;

/**
 * MPresencePublishing API object implementation.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceWatchingImp ): public CXIMPApiObjBase,
                                               public MPresenceWatching
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEWATCHINGIMP };


public:
    static CPresenceWatchingImp* NewL( MXIMPContextInternal& aContext );
    ~CPresenceWatchingImp();


public:
    CPresenceWatchingImp( MXIMPContextInternal& aContext );


public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MPresenceWatching

    TXIMPRequestId SubscribePresentityPresenceL( const MXIMPIdentity& aPresentityId,
                                                 const MPresenceInfoFilter& aPif );
    TXIMPRequestId UnsubscribePresentityPresenceL( const MXIMPIdentity& aPresentityId );
    TXIMPRequestId SubscribePresentityGroupMembersPresenceL( const MXIMPIdentity& aGroupId,
                                                             const MPresenceInfoFilter& aPif );
    TXIMPRequestId UnsubscribePresentityGroupMembersPresenceL( const MXIMPIdentity& aGroupId );


private: //Helpers

    HBufC8* PackPresentityLC( const MXIMPIdentity& aIdentity );

    HBufC8* PackFilterLC( const MPresenceInfoFilter& aFilter );

private: // data

    /**
     * Context session proxy for accessing context server.
     * Ref.
     */
    MXIMPContextInternal& iContext;

    };


#endif // CPRESENCEWATCHINGIMP_H
