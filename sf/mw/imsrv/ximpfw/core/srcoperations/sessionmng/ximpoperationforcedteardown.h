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
* Description:  Forced teardown operation 
 *
*/

#ifndef CPSCOPERATIONFORCEDTEARDOWN_H
#define CPSCOPERATIONFORCEDTEARDOWN_H


#include <e32base.h>

#include <ximpbase.h>
#include "ximpoperationbase.h"
#include "ximpoperationdefs.h"

class MXIMPHost;

/**
 * Forced tear down operation.
 * Activates all cached unbinds.
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class CXIMPOperationForcedTeardown : public CXIMPOperationBase
    {
    public:

        IMPORT_C CXIMPOperationForcedTeardown();

        void ConstructL( const TDesC8& aParamPck );

        ~CXIMPOperationForcedTeardown();

    public: // from CXIMPOperationBase

        void SetHost( MXIMPHost& aMyHost );
        void ProcessL();
        void RequestCompletedL();
        TInt Type() const;

    private: // data

    };


#endif // CPSCOPERATIONFORCEDTEARDOWN_H 
