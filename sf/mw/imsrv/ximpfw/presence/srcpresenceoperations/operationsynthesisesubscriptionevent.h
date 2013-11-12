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
* Description:  Internal synthesise subscription event operation
 *
*/

#ifndef CPSCOPERATIONSYNTHESISESUBSCRIPTIONEVENT_H
#define CPSCOPERATIONSYNTHESISESUBSCRIPTIONEVENT_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presenceoperationdefs.h"

class CXIMPIdentityImp;

/**
 * Internal synthesise own subscription event operation
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationSynthesiseSubscriptionEvent : public CXIMPOperationBase
    {
    public: // Definitions

        enum TSubscriptionType
            {
            EOwnPresence,
            EPresenceBlockList,
            EPresenceGrantRequestList,
            EPresenceWatcherList,
            EPresentityGroupList,
            EPresentityGroupContent,
            EPresentityPresence,
            EPresentityGroupMembersPresence,
            };

    public:

        IMPORT_C COperationSynthesiseSubscriptionEvent( TSubscriptionType aType,
                                                           TBool aForceEvent );

        void ConstructL( const CXIMPIdentityImp& aIdentity );
        void ConstructL( const TDesC8& aParamPck );

        ~COperationSynthesiseSubscriptionEvent();


    public: // from CXIMPOperationBase

        void ProcessL();

        void RequestCompletedL();

        TInt Type() const;

    private: // data

    /**
     * Type of subscription to synthesise.
     */
    TSubscriptionType iType;

    /**
     * Identity for presentity presence synthetization.
     */
    CXIMPIdentityImp* iIdentity;

    /**
     * If event should be forced.
     */
    TBool iForceEvent;

    };


#endif // CPSCOPERATIONSYNTHESISESUBSCRIPTIONEVENT_H
