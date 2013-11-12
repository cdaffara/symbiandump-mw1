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
* Description:  Grant presence to presentity operation
 *
*/

#ifndef CPSCOPERATIONGRANTPRESENCETOPRESENTITY_H
#define CPSCOPERATIONGRANTPRESENCETOPRESENTITY_H


#include <e32base.h>

#include <ximpbase.h>
#include "ximpoperationbase.h"
#include "presenceconfigurationitem.h"

#include "presenceoperationdefs.h"

class CXIMPIdentityImp;

/**
 * Grant presence to presentity operation
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationGrantPresenceToPresentity : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationGrantPresenceToPresentity();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationGrantPresenceToPresentity();


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
         * Merged pif with other contexts.
         */
        CPresenceInfoFilterImp* iMergedPif;

        /**
         * Identity 
         */
        CXIMPIdentityImp* iIdentity;
        
        /**
         * Authorization item for configuration
         */
        CPresenceConfigurationItem* iConfItem;
        
        /**
         * Subscription status
         */
        CPresenceConfigurationItem::TConfigurationStatus iConfigurationStatus;
        
        /**
         * Don't force event.
         */
        TBool iDoNotForce;                
    };


#endif // CPSCOPERATIONGRANTPRESENCETOPRESENTITY_H
