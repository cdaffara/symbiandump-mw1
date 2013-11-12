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
* Description:  Presentity presence unsubscription operation
 *
*/

#ifndef CPSCOPERATIONUNSUBSCRIBEPRESENTITYPRESENCE_H
#define CPSCOPERATIONUNSUBSCRIBEPRESENTITYPRESENCE_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presentitypresencesubscriptionitem.h"
#include "presenceoperationdefs.h"

/**
 * Presence interest registration operation.
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationUnsubscribePresentityPresence : public CXIMPOperationBase
    {
    public:

        IMPORT_C COperationUnsubscribePresentityPresence();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationUnsubscribePresentityPresence();


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
        CPresentityPresenceSubscriptionItem* iSubItem;

        /**
         * Presentity address
         * Own.
         */
        CXIMPIdentityImp* iIdentity;

        /**
         * Subscription status
         */
        CPresentityPresenceSubscriptionItem::TSubscriptionStatus iSubscriptionStatus;
    };


#endif // CPSCOPERATIONUNSUBSCRIBEPRESENTITYPRESENCE_H
