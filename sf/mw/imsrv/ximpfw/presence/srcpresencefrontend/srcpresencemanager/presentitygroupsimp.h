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

#ifndef CPRESENTITYGROUPSIMP_H
#define CPRESENTITYGROUPSIMP_H

#include <presentitygroups.h>
#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"
#include "presenceoperationdefs.h"

class MXIMPContextInternal;
class CPresentityGroupInfoImp;

/**
 * MPresencePublishing API object implementation.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresentityGroupsImp ): public CXIMPApiObjBase,
                                           public MPresentityGroups
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENTITYGROUPSIMP };


public:
    static CPresentityGroupsImp* NewL( MXIMPContextInternal& aContext );
    ~CPresentityGroupsImp();
    
    
public:
    CPresentityGroupsImp( MXIMPContextInternal& aContext );


public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MPresentityGroups

    TXIMPRequestId SubscribePresentityGroupListL();
    TXIMPRequestId UnsubscribePresentityGroupListL();
    TXIMPRequestId CreatePresentityGroupL( const MXIMPIdentity& aGroupId,
                                                   const TDesC16& aGroupDisplayName );
    TXIMPRequestId DeletePresentityGroupL( const MXIMPIdentity& aGroupId );
    TXIMPRequestId UpdatePresentityGroupDisplayNameL( const MXIMPIdentity& aGroupId,
                                                      const TDesC16& aGroupDisplayName );
                                                      
    TXIMPRequestId SubscribePresentityGroupContentL( const MXIMPIdentity& aGroupId );
    TXIMPRequestId UnsubscribePresentityGroupContentL( const MXIMPIdentity& aGroupId );
    TXIMPRequestId AddPresentityGroupMemberL( const MXIMPIdentity& aGroupId,
                                              const MXIMPIdentity& aMemberId,
                                              const TDesC16& aMemberDisplayName );
    TXIMPRequestId RemovePresentityGroupMemberL( const MXIMPIdentity& aGroupId,
                                                 const MXIMPIdentity& aMemberId );
    TXIMPRequestId UpdatePresentityGroupMemberDisplayNameL( const MXIMPIdentity& aGroupId,
                                                            const MXIMPIdentity& aMemberId,
                                                            const TDesC16& aMemberDisplayName );
                                                          
public: //Helpers

    HBufC8* PackGroupInfoLC( const CPresentityGroupInfoImp& infoImp );
    HBufC8* PackIdentityLC( const MXIMPIdentity& aIdentity );
    TXIMPRequestId QueueOperationL( NPresenceOps::TPresenceOpTypes aOperation,
                                    const MXIMPIdentity& aGroupId,
                                    const TDesC16& aGroupDisplayName );
    TXIMPRequestId QueueOperationL( NPresenceOps::TPresenceOpTypes aOperation,
                                    const MXIMPIdentity& aGroupId,
                                    const MXIMPIdentity& aMemberId,
                                    const TDesC16& aMemberDisplayName );
    TXIMPRequestId QueueOperationL( NPresenceOps::TPresenceOpTypes aOperation,
                                    const MXIMPIdentity& aGroupId,
                                    const MXIMPIdentity& aMemberId );

private: // data

    /**
     * Context session proxy for accessing context server.
     * Ref.
     */
    MXIMPContextInternal& iContext;

    };


#endif // CPRESENTITYGROUPSIMP_H
