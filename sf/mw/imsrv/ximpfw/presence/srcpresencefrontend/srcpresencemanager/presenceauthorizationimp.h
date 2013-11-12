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
* Description:  Implementation of MPresenceWatching
*
*/

#ifndef CPRESENCEAUTHORIZATIONIMP_H
#define CPRESENCEAUTHORIZATIONIMP_H

#include <presenceauthorization.h>
#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"
#include "presenceoperationdefs.h"

class MXIMPContextInternal;

/**
 * MPresencePublishing API object implementation.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceAuthorizationImp ): public CXIMPApiObjBase,
                                                    public MPresenceAuthorization
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEAUTHORIZATIONIMP };


public:
    static CPresenceAuthorizationImp* NewL( MXIMPContextInternal& aContext );
    ~CPresenceAuthorizationImp();


public:
    CPresenceAuthorizationImp( MXIMPContextInternal& aContext );


public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MPresenceAuthorization

    TXIMPRequestId SubscribePresenceGrantRequestListL();
    
    TXIMPRequestId UnsubscribePresenceGrantRequestListL();
    
    TXIMPRequestId GrantPresenceForPresentityL(
                    const MXIMPIdentity& aPresentityId,
                    const MPresenceInfoFilter& aPif );
    
    TXIMPRequestId WithdrawPresenceGrantFromPresentityL(
                    const MXIMPIdentity& aPresentityId );

    TXIMPRequestId GrantPresenceForPresentityGroupMembersL(
                    const MXIMPIdentity& aGroupId,
                    const MPresenceInfoFilter& aPif );

    TXIMPRequestId WithdrawPresenceGrantFromPresentityGroupMembersL(
                    const MXIMPIdentity& aGroupId );

    TXIMPRequestId GrantPresenceForEveryoneL(
                    const MPresenceInfoFilter& aPif );
    
    TXIMPRequestId WithdrawPresenceGrantFromEveryoneL();


    TXIMPRequestId SubscribePresenceBlockListL();

    TXIMPRequestId UnsubscribePresenceBlockListL();

    TXIMPRequestId BlockPresenceForPresentityL(
                    const MXIMPIdentity& aPresentityId );

    TXIMPRequestId CancelPresenceBlockFromPresentityL(
                    const MXIMPIdentity& aPresentityId );


private: //Helpers

    /**
     * Pack info filter for queue.
     */
    HBufC8* PackPresenceInfoFilterLC( const MPresenceInfoFilter& aPif );

    /**
     * Pack pif and identity for operation
     */
    TXIMPRequestId QueueOperationL( NPresenceOps::TPresenceOpTypes aOperation,
                                    const MPresenceInfoFilter& aPif,
                                    const MXIMPIdentity& aIdentity );
                                    
    /**
     * Pack presentity
     */
    HBufC8* PackPresentityLC( const MXIMPIdentity& aIdentity );

private: // data

    /**
     * Context session proxy for accessing context server.
     * Ref.
     */
    MXIMPContextInternal& iContext;

    };


#endif // CPRESENCEAUTHORIZATIONIMP_H
