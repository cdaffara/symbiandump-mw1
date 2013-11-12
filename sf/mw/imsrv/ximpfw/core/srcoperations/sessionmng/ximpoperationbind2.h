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
* Description:  Bind operation
 *
*/

#ifndef C_XIMPOPERATIONBIND2_H
#define C_XIMPOPERATIONBIND2_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "ximpoperationdefs.h"

class CXIMPContextStateEventImp;

/**
 * Bind operation
 *
 * @lib prfwoperation.lib
 * @since S60 v4.0
 */
class CXIMPOperationBind2 : public CXIMPOperationBase
    {

    public:
        
        IMPORT_C CXIMPOperationBind2();

        void ConstructL( const TDesC8& aParamPck );

        ~CXIMPOperationBind2();

    public: // from CXIMPOperationBase
        
        void ProcessL();

        void RequestCompletedL();
        
        TInt Type() const;
        
        void BeginOperation();
        void EndOperation();
        
    private: // data
    
        // State event for client
        CXIMPContextStateEventImp* iTempEvent;
    };


#endif // C_XIMPOPERATIONBIND2_H
