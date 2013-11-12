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
* Description:  Withdraw presence from presentity group members operation
 *
*/

#ifndef CPSCOPERATIONWITHDRAWPRESENCEFROMPRESENTITYGROUPMEMBERS_H
#define CPSCOPERATIONWITHDRAWPRESENCEFROMPRESENTITYGROUPMEMBERS_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "groupmemberspresenceconfigurationitem.h"
#include "presenceoperationdefs.h"

/**
 * Withdraw presence from presentity group members operation
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationWithdrawPresenceFromPresentityGroupMembers : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationWithdrawPresenceFromPresentityGroupMembers();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationWithdrawPresenceFromPresentityGroupMembers();


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
        CXIMPIdentityImp* iGroupId;
        
        /**
         * Presence configuration item
         * Own.
         */
        CGroupMembersPresenceConfigurationItem* iConfItem;

        /**
         * Configuration status
         */
        CGroupMembersPresenceConfigurationItem::TConfigurationStatus iConfigurationStatus;

    };


#endif // CPSCOPERATIONWITHDRAWPRESENCEFROMPRESENTITYGROUPMEMBERS_H
