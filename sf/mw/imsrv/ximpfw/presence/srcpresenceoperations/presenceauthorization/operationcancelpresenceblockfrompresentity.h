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

#ifndef CPSCOPERATIONCANCELBLOCKFROMPRESENTITY_H
#define CPSCOPERATIONCANCELBLOCKFROMPRESENTITY_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presenceoperationdefs.h"

class CXIMPIdentityImp;

/**
 * Withdraw presence from presentity
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationCancelPresenceBlockFromPresentity : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationCancelPresenceBlockFromPresentity();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationCancelPresenceBlockFromPresentity();


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

    };


#endif // CPSCOPERATIONCANCELBLOCKFROMPRESENTITY_H
