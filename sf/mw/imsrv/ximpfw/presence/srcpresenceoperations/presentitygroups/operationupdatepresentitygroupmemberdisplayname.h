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
* Description:  Update Presentity Group Member Display Name operation
 *
*/

#ifndef PSCOPERATIONUPDATEPRESENTITYGROUPMEMBERDISPLAYNAME_H
#define PSCOPERATIONUPDATEPRESENTITYGROUPMEMBERDISPLAYNAME_H

#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presenceoperationdefs.h"

class CXIMPIdentityImp;
class CPresentityGroupMemberInfoImp;

/**
 * Update presentity group member display name operation
 *
 * @lib presenceoperation.lib
 * @since S60 v4.0
 */
class COperationUpdatePresentityGroupMemberDisplayName : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationUpdatePresentityGroupMemberDisplayName();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationUpdatePresentityGroupMemberDisplayName();

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
        
        /**
         * Group member info
         * Own.
         */
        CPresentityGroupMemberInfoImp* iMemberInfo;
    };


#endif // PSCOPERATIONUPDATEPRESENTITYGROUPMEMBERDISPLAYNAME_H
