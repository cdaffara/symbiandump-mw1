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
* Description:  Handle presentity group list operations
 *
*/

#ifndef CPSCOPERATIONHANDLEPRESENCEGRANTREQUESTS_H
#define CPSCOPERATIONHANDLEPRESENCEGRANTREQUESTS_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presencetypehelpers.h"
#include "presenceoperationdefs.h"

/**
 * Handle presentity group related operations.
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationHandlePresenceGrantRequests : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationHandlePresenceGrantRequests(
                TInt aMyType );

        void ConstructL( const TDesC8& aParamPck );

        ~COperationHandlePresenceGrantRequests();

    public: // from CXIMPOperationBase

        void ProcessL();

        void RequestCompletedL();

        TInt Type() const;

    private: // data

        /**
         * Array of group identities.
         * Owned.
         */
        RPrGrntReqInfoImpArray* iGrantReqList;

        /**
         * Obsoleted
         * Owned.
         */
        CXIMPIdentityImp* iObsoletedId;

        /**
         * Received request
         * Owned.
         */
        CPresenceGrantRequestInfoImp* iReceivedReq;

        /**
         * This is an internal and polymorphic operation.
         * The actual type will be defined by this variable,
         * set during operation creation.
         */
        TInt iMyType;
    };


#endif // CPSCOPERATIONHANDLEPRESENCEGRANTREQUESTS_H
