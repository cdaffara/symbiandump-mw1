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
* Description:  Block presence for presentity operation
 *
*/

#ifndef CPSCOPERATIONBLOCKPRESENCEFORPRESENTITY_H
#define CPSCOPERATIONBLOCKPRESENCEFORPRESENTITY_H


#include <e32base.h>

#include <ximpbase.h>
#include "ximpoperationbase.h"

#include "presenceoperationdefs.h"

class CXIMPIdentityImp;
class CPresenceBlockInfoImp;

/**
 * Grant presence to presentity operation
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationBlockPresenceForPresentity : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationBlockPresenceForPresentity();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationBlockPresenceForPresentity();


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
         * Own.
         */
        CXIMPIdentityImp* iIdentity;
        
        /**
         * Block info
         * Own.
         */
        CPresenceBlockInfoImp* iBlockInfo;

    };


#endif // CPSCOPERATIONBLOCKPRESENCEFORPRESENTITY_H
