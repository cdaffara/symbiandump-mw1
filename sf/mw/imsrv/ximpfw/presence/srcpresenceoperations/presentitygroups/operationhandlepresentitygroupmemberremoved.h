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
* Description:  Handle presentity group content operations
 *
*/

#ifndef CPSCOPERATIONHANDLEPRESENTITYGROUPMEMBERREMOVED_H
#define CPSCOPERATIONHANDLEPRESENTITYGROUPMEMBERREMOVED_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presencetypehelpers.h"
#include "presenceoperationdefs.h"

class CXIMPIdentityImp;

/**
 * Handle presentity group related operations.
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationHandlePresentityGroupMemberRemoved : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationHandlePresentityGroupMemberRemoved();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationHandlePresentityGroupMemberRemoved();

    public: // from CXIMPOperationBase

        void ProcessL();

        void RequestCompletedL();
        TInt Type() const;

    private: // data

        /**
         * Array of group identities.
         * Owned.
         */
        CXIMPIdentityImp* iGroupId;

        /**
         * Array of group identities.
         * Owned.
         */
        CPresentityGroupMemberInfoImp* iMemberInfo;

    };


#endif // CPSCOPERATIONHANDLEPRESENTITYGROUPMEMBERREMOVED_H
