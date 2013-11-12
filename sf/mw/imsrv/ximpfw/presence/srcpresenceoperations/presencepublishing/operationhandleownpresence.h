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
* Description:  Handle own presence operation
 *
*/

#ifndef CPSCOPERATIONHANDLEOWNPRESENCE_H
#define CPSCOPERATIONHANDLEOWNPRESENCE_H


#include <e32base.h>

#include <ximpbase.h>
#include "ximpoperationbase.h"
#include "presenceoperationdefs.h"

class CPresenceInfoImp;

/**
 * Publish presence operation.
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationHandleOwnPresence : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationHandleOwnPresence();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationHandleOwnPresence();

    public: // from CXIMPOperationBase

        void ProcessL();

        void RequestCompletedL();

        TInt Type() const;

    private: // data

        /**
         * Presence document to handle. 
         * Own.
         */
        CPresenceInfoImp* iPresInfo;
    };


#endif // CPSCOPERATIONHANDLEOWNPRESENCE_H
