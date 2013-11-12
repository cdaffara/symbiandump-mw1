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
* Description: Operationon SubscribePresentityGroupMembers Presence
 *
*/

#ifndef CPSCOPERATIONSUBSCRIBEPRESENTITYGROUPMEMBERSPRESENCE_H
#define CPSCOPERATIONSUBSCRIBEPRESENTITYGROUPMEMBERSPRESENCE_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presentitygroupmemberspresencesubscriptionitem.h"

/**
 * Group members presence subscription
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationSubscribePresentityGroupMembersPresence : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationSubscribePresentityGroupMembersPresence();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationSubscribePresentityGroupMembersPresence();


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
         * Presentity address
         * Own.
         */
        CXIMPIdentityImp* iGroupId;

        /**
         * Subscription item
         */
        CPresentityGroupMembersPresenceSubscriptionItem* iSubItem;
        
        /**
         * Presence info filter having new presence info filter for context.
         * Own.
         */
        CPresenceInfoFilterImp* iMergedPif; 
        
        /**
         * Pif from call
         */
        CPresenceInfoFilterImp* iPif;
        
        /**
         * Subscription status
         */
        CPresentityGroupMembersPresenceSubscriptionItem::TSubscriptionStatus iSubscriptionStatus;
        
        /**
         * Don't force event.
         */
        TBool iDoNotForce;        
    };


#endif // CPSCOPERATIONSUBSCRIBEPRESENTITYGROUPMEMBERSPRESENCE_H
