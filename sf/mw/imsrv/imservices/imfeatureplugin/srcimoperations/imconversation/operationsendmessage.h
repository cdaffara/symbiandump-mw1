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
* Description:  Im send message operation
 *
*/

#ifndef C_OPERATIONSENDMESSAGE_H
#define C_OPERATIONSENDMESSAGE_H

#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "imoperationdefs.h"

class CImConversationInfoImp;

/**
 * Im send message operation
 *
 * @lib imoperation.lib
 * @since S60
 */
class COperationSendMessage : public CXIMPOperationBase
    {

    public:
        
        IMPORT_C COperationSendMessage();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationSendMessage();

    public: // from CXIMPOperationBase
        
        void ProcessL();

        void RequestCompletedL();
        
        TInt Type() const;
        
    private: // data
    
        /**
         * convinfo
         */
        CImConversationInfoImp* iConvInfo;
    };


#endif // C_OPERATIONSENDMESSAGE_H
