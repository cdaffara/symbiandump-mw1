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

#ifndef C_PSCOPERATIONBIND_H
#define C_PSCOPERATIONBIND_H


#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "ximpoperationdefs.h"

class CXIMPContextStateEventImp;

/**
 * Bind operation
 *
 * @lib ximpoperation.lib
 * @since S60 v4.0
 */
class CXIMPOperationBind : public CXIMPOperationBase
    {

    public:
        
        IMPORT_C CXIMPOperationBind();

        void ConstructL( const TDesC8& aParamPck );

        ~CXIMPOperationBind();

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


#endif // C_PSCOPERATIONBIND_H
