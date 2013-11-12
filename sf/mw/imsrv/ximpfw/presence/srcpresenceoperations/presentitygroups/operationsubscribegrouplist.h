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
* Description:  Presentity group list subscription operator
 *
*/

#ifndef CPSCOPERATIONSUBSCRIBEGROUPLIST_H
#define CPSCOPERATIONSUBSCRIBEGROUPLIST_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "grouplistsubscriptionitem.h"
#include "presenceoperationdefs.h"

/**
 * Presence interest registration operation.
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationSubscribeGroupList : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationSubscribeGroupList();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationSubscribeGroupList();


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
        CGroupListSubscriptionItem* iGroupList;
        
        /**
         * Subscription status
         */
        CGroupListSubscriptionItem::TSubscriptionStatus iSubscriptionStatus;
    };


#endif // CPSCOPERATIONSUBSCRIBEGROUPLIST_H
