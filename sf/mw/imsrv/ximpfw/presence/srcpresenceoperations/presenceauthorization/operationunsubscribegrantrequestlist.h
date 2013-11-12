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
* Description:  Presence grant request list unsubscription operation
 *
*/

#ifndef CPSCOPERATIONUNSUBSCRIBEGRANTREQUESTLIST_H
#define CPSCOPERATIONUNSUBSCRIBEGRANTREQUESTLIST_H


#include <e32base.h>

#include <ximpbase.h>
#include "ximpoperationbase.h"
#include "grantrequestlistsubscriptionitem.h"
#include "presenceoperationdefs.h"

/**
 * Presence grant request list unsubscribe operation.
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationUnsubscribeGrantRequestList : public CXIMPOperationBase
    {
    public:

        IMPORT_C COperationUnsubscribeGrantRequestList();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationUnsubscribeGrantRequestList();


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
         * Group list access.
         */
        CGrantRequestListSubscriptionItem* iGrantRequestList;

        /**
         * Subscription status
         */
        CGrantRequestListSubscriptionItem::TSubscriptionStatus iSubscriptionStatus;
    };


#endif // CPSCOPERATIONUNSUBSCRIBEGRANTREQUESTLIST_H
