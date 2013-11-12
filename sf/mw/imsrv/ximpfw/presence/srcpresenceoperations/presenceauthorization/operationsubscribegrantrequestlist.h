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
* Description:  Presence grant request list subscribe operation
 *
*/

#ifndef CPSCOPERATIONSUBSCRIBEGRANTREQUESTLIST_H
#define CPSCOPERATIONSUBSCRIBEGRANTREQUESTLIST_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "grantrequestlistsubscriptionitem.h"
#include "presenceoperationdefs.h"

/**
 * Presence grant request list subscribe operation.
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationSubscribeGrantRequestList : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationSubscribeGrantRequestList();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationSubscribeGrantRequestList();


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
         * Grant request list access.
         */
        CGrantRequestListSubscriptionItem* iGrantRequestList;

        /**
         * Subscription status
         */
        CGrantRequestListSubscriptionItem::TSubscriptionStatus iSubscriptionStatus;
    };


#endif // CPSCOPERATIONSUBSCRIBEGRANTREQUESTLIST_H
