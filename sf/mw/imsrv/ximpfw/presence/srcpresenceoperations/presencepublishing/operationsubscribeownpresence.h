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
* Description:  Presence interest registration operation
 *
*/

#ifndef CPSCOPERATIONSUBSCRIBEOWNPRESENCE_H
#define CPSCOPERATIONSUBSCRIBEOWNPRESENCE_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "ownpresencesubscriptionitem.h"
#include "presenceoperationdefs.h"

/**
 * Presence interest registration operation.
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationSubscribeOwnPresence : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationSubscribeOwnPresence();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationSubscribeOwnPresence();


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
         * Presence info filter having new presence info filter for context.
         * Own.
         */
        CPresenceInfoFilterImp* iPif;
        CPresenceInfoFilterImp* iMergedPif;
        
        /**
         *
         */
        COwnPresenceSubscriptionItem::TSubscriptionStatus iSubscriptionStatus;
        
        /**
         *
         */
        COwnPresenceSubscriptionItem* iOwnPresence;
        
        /**
         * Don't force event.
         */
        TBool iDoNotForce;
    };


#endif // CPSCOPERATIONSUBSCRIBEOWNPRESENCE_H
