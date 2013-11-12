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
* Description:  Grant presence to presentity group members operation
 *
*/

#ifndef CPSCOPERATIONGRANTPRESENCETOPRESENTITYGROUPMEMBERS_H
#define CPSCOPERATIONGRANTPRESENCETOPRESENTITYGROUPMEMBERS_H


#include <e32base.h>

#include <ximpbase.h>
#include "ximpoperationbase.h"
#include "groupmemberspresenceconfigurationitem.h"

#include "presenceoperationdefs.h"

class CXIMPIdentityImp;

/**
 * Grant presence to presentity group members operation
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationGrantPresenceToPresentityGroupMembers : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationGrantPresenceToPresentityGroupMembers();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationGrantPresenceToPresentityGroupMembers();


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
         * Presence information filter
         */
        CPresenceInfoFilterImp* iPif;

        /**
         * Identity 
         */
        CXIMPIdentityImp* iGroupId;
        
        /**
         * Subscription item
         */
        CGroupMembersPresenceConfigurationItem* iConfItem;
        
        /**
         * Presence info filter having new presence info filter for context.
         * Own.
         */
        CPresenceInfoFilterImp* iMergedPif; 
        
        /**
         * Subscription status
         */
        CGroupMembersPresenceConfigurationItem::TConfigurationStatus iConfigurationStatus;
        
        /**
         * Don't force event.
         */
        TBool iDoNotForce;        

    };


#endif // CPSCOPERATIONGRANTPRESENCETOPRESENTITYGROUPMEMBERS_H
