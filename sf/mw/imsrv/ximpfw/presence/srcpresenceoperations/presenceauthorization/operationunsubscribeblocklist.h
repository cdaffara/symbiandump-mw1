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
* Description:  Presence block list unsubscription operation
 *
*/

#ifndef CPSCOPERATIONUNSUBSCRIBEBLOCKLIST_H
#define CPSCOPERATIONUNSUBSCRIBEBLOCKLIST_H


#include <e32base.h>

#include "ximpbase.h"
#include "ximpoperationbase.h"
#include "blocklistsubscriptionitem.h"

#include "presenceoperationdefs.h"

class MXIMPHost;

/**
 * Presence grant request list unsubscribe operation.
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationUnsubscribeBlockList : public CXIMPOperationBase
    {
    public:

        IMPORT_C COperationUnsubscribeBlockList();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationUnsubscribeBlockList();


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
        CBlockListSubscriptionItem* iBlockList;

        /**
         * Subscription status
         */
        CBlockListSubscriptionItem::TSubscriptionStatus iSubscriptionStatus;
    };


#endif // CPSCOPERATIONUNSUBSCRIBEBLOCKLIST_H
