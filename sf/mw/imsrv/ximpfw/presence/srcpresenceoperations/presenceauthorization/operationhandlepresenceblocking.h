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
* Description:  Handle presence blocking operations
 *
*/

#ifndef CPSCOPERATIONHANDLEPRESENCEBLOCKING_H
#define CPSCOPERATIONHANDLEPRESENCEBLOCKING_H


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
class COperationHandlePresenceBlocking : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationHandlePresenceBlocking(
                TInt aMyType );

        void ConstructL( const TDesC8& aParamPck );

        ~COperationHandlePresenceBlocking();

    public: // from CXIMPOperationBase

        void ProcessL();

        void RequestCompletedL();

        TInt Type() const;

    private: // data

        /**
         * Array blocked presentities.
         * Owned.
         */
        RPrBlockInfoImpArray* iBlockList;

        /**
         * Blocked presentity
         * Owned.
         */
        CPresenceBlockInfoImp* iBlocked;

        /**
         * Canceled block presentity
         * Owned.
         */
        CXIMPIdentityImp* iCanceledBlockId;

        /**
         * This is an internal and polymorphic operation.
         * The actual type will be defined by this variable,
         * set during operation creation.
         */
        TInt iMyType;
    };


#endif // CPSCOPERATIONHANDLEPRESENCEBLOCKING_H
