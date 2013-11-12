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
* Description:  Presentity group members presence unsubscription operation
 *
*/

#ifndef CPSCOPERATIONUNSUBSCRIBEPRESENTITYGROUPMEMBERSPRESENCE_H
#define CPSCOPERATIONUNSUBSCRIBEPRESENTITYGROUPMEMBERSPRESENCE_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presentitygroupmemberspresencesubscriptionitem.h"

/**
 * Group members presence unsubscription
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationUnsubscribePresentityGroupMembersPresence : public CXIMPOperationBase
    {
    public:

        IMPORT_C COperationUnsubscribePresentityGroupMembersPresence();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationUnsubscribePresentityGroupMembersPresence();


    public: // from CXIMPOperationBase

        void ProcessL();

        void RequestCompletedL();

        TInt Type() const;

    private: // Helper methods

        /**
         * Synthesise event after completion
         */
        void SynthesiseEventL();

    private: // data

        /**
         * Presence subscription item
         * Own.
         */
        CPresentityGroupMembersPresenceSubscriptionItem* iSubItem;

        /**
         * Presentity address
         * Own.
         */
        CXIMPIdentityImp* iGroupId;

        /**
         * Subscription status
         */
        CPresentityGroupMembersPresenceSubscriptionItem::TSubscriptionStatus iSubscriptionStatus;

    };


#endif // CPSCOPERATIONUNSUBSCRIBEPRESENTITYGROUPMEMBERSPRESENCE_H
