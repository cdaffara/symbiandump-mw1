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
* Description:  Grant presence to everyone
 *
*/

#ifndef CPSCOPERATIONGRANTPRESENCETOEVERYONE_H
#define CPSCOPERATIONGRANTPRESENCETOEVERYONE_H


#include <e32base.h>

#include <ximpbase.h>
#include "ximpoperationbase.h"
#include "presencetoeveryoneconfigurationitem.h"

#include "presenceoperationdefs.h"

class CXIMPIdentityImp;

/**
 * Grant presence to everyone
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationGrantPresenceToEveryone : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationGrantPresenceToEveryone();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationGrantPresenceToEveryone();


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
         * Authorization item for configuration
         */
        CPresenceToEveryoneConfigurationItem* iConfItem;
        
        /**
         * Subscription status
         */
        CPresenceToEveryoneConfigurationItem::TConfigurationStatus iConfigurationStatus;
        
        /**
         * Don't force event.
         */
        TBool iDoNotForce;        
    };


#endif // CPSCOPERATIONGRANTPRESENCETOEVERYONE_H
