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
* Description:  Remove Presentity Group Member operation
 *
*/

#ifndef PSCOPERATIONREMOVEPRESENTITYGROUPMEMBER_H
#define PSCOPERATIONREMOVEPRESENTITYGROUPMEMBER_H

#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presenceoperationdefs.h"

class CXIMPIdentityImp;

/**
 * Remove presentity group member operation
 *
 * @lib presenceoperation.lib
 * @since S60 v4.0
 */
class COperationRemovePresentityGroupMember : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationRemovePresentityGroupMember();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationRemovePresentityGroupMember();

    public: // from CXIMPOperationBase

        void ProcessL();

        void RequestCompletedL();

        TInt Type() const;

    private: // data

        /**
         * Group id
         * Own.
         */
        CXIMPIdentityImp* iGroupId;
        CXIMPIdentityImp* iMemberId;
    };


#endif // PSCOPERATIONREMOVEPRESENTITYGROUPMEMBER_H
