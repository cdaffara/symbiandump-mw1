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
* Description:  Set subscription data
 *
*/

#ifndef CPSCOPERATIONSETSUBSCRIPTIONDATA_H
#define CPSCOPERATIONSETSUBSCRIPTIONDATA_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presenceoperationdefs.h"

class CXIMPIdentityImp;

class CXIMPDataSubscriptionStateImp;

/**
 * Set subscription data for items
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationSetSubscriptionData : public CXIMPOperationBase
    {
    public: // Definitions

    public:

        IMPORT_C COperationSetSubscriptionData( NPresenceOps::TPresenceOpTypes aType );

        void ConstructL( const TDesC8& aParamPck );

        ~COperationSetSubscriptionData();

    public: // from CXIMPOperationBase

        void ProcessL();

        void RequestCompletedL();

        TInt Type() const;

    private: // data

    /**
     * Identity for presentity presence synthetization.
     */
    CXIMPIdentityImp* iIdentity;
    
    /**
     * Type of operation
     */
    NPresenceOps::TPresenceOpTypes iType;
    
    /**
     * New subscription state
     */
    CXIMPDataSubscriptionStateImp* iSubscriptionState;
    
    /**
     * Status reason for change.
     */
    CXIMPStatusImp* iStatus;

    };


#endif // CPSCOPERATIONSETSUBSCRIPTIONDATA_H
