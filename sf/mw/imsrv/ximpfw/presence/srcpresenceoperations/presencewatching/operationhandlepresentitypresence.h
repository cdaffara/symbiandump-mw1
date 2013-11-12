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
* Description:  Handle presentity group list operation
 *
*/

#ifndef CPSCOPERATIONHANDLEPRESENTITYPRESENCE_H
#define CPSCOPERATIONHANDLEPRESENTITYPRESENCE_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presenceoperationdefs.h"

class CXIMPIdentityImp;
class CPresenceInfoImp;

/**
 * Publish presence operation.
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationHandlePresentityPresence : public CXIMPOperationBase
    {

    public:

        IMPORT_C COperationHandlePresentityPresence( TInt aOperation );

        void ConstructL( const TDesC8& aParamPck );

        ~COperationHandlePresentityPresence();

    public: // from CXIMPOperationBase

        void ProcessL();

        void RequestCompletedL();

        TInt Type() const;

    private: // data
        
        /**
         * Identity
         */
        CXIMPIdentityImp* iIdentity;
        
        /**
         * Presence info
         */
        CPresenceInfoImp* iPresenceInfo;
        
        /**
         * Operation. 
         */
        TInt iOperation;
    };


#endif // CPSCOPERATIONHANDLEPRESENTITYPRESENCE_H
