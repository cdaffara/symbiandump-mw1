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
* Description:  Withdraw presence from presentity
 *
*/

#ifndef CPSCOPERATIONWITHDRAWPRESENCEFROMPRESENTITY_H
#define CPSCOPERATIONWITHDRAWPRESENCEFROMPRESENTITY_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presenceconfigurationitem.h"
#include "presenceoperationdefs.h"

/**
 * Withdraw presence from presentity
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationWithdrawPresenceFromPresentity : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationWithdrawPresenceFromPresentity();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationWithdrawPresenceFromPresentity();


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
         * Identity 
         */
        CXIMPIdentityImp* iIdentity;

        /**
         * Presence configuration item
         * Own.
         */
        CPresenceConfigurationItem* iConfItem;

        /**
         * Subscription status
         */
        CPresenceConfigurationItem::TConfigurationStatus iConfigurationStatus;

    };


#endif // CPSCOPERATIONWITHDRAWPRESENCEFROMPRESENTITY_H
