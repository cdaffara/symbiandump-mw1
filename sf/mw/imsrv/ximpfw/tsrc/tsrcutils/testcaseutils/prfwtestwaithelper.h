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
* Description:  XIMP Framework TestGeneralWaiter 
*
*/



#ifndef CPRFWTESTWAITHELPER_H
#define CPRFWTESTWAITHELPER_H


#include <e32base.h>

class CXIMPTestGeneralWaiter;

class CXIMPTestWaitHelper : public CBase
    {
    public:

        IMPORT_C static CXIMPTestWaitHelper* NewL();
        
        IMPORT_C void WaitForL( TInt aSeconds );
        
        ~CXIMPTestWaitHelper();

    private:
    
        /**
         *
         */
        void ConstructL();

    private: // data
    };

#endif // CPRFWTESTWAITHELPER_H
