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

#ifndef CPSCOPERATIONHANDLEPRESENTITYGROUPS_H
#define CPSCOPERATIONHANDLEPRESENTITYGROUPS_H


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
class COperationHandlePresentityGroups : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationHandlePresentityGroups(
                NPresenceOps::TPresenceOpTypes aMyType );

        void ConstructL( const TDesC8& aParamPck );

        ~COperationHandlePresentityGroups();

    public: // from CXIMPOperationBase

        void ProcessL();

        void RequestCompletedL();

        TInt Type() const;

    private: // data

        /**
         * Array of group identities.
         * Owned.
         */
        RPrGrpInfoImpArray* iGroupAddressList;

        /**
         * This is an internal and polymorphic operation.
         * The actual type will be defined by this variable,
         * set during operation creation.
         */
        NPresenceOps::TPresenceOpTypes iMyType;
    };


#endif // CPSCOPERATIONHandlePresentityGroups_H
