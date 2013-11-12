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
* Description:  Internal operation for launching cleaner
 *
*/

#ifndef CPSCOPERATIONLAUNCHCLEANER_H
#define CPSCOPERATIONLAUNCHCLEANER_H


#include <e32base.h>
#include <ximpbase.h>
#include "ximpoperationbase.h"
#include "presenceoperationdefs.h"


/**
 * Internal synthesise own subscription event operation
 *
 * @lib ximpoperations.lib
 * @since S60 v4.0
 */
class COperationLaunchCleaner : public CXIMPOperationBase
    {
    public:

        IMPORT_C COperationLaunchCleaner();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationLaunchCleaner();


    public: // from CXIMPOperationBase

        void ProcessL();

        void RequestCompletedL();

        TInt Type() const;
        
        void EndOperation();

    private: // data
    };


#endif // CPSCOPERATIONLAUNCHCLEANER_H
