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
* Description:  ?Description
*
*/



#ifndef CPRFWTESTGENERALWAITER_H
#define CPRFWTESTGENERALWAITER_H 


#include <e32base.h>


class T_MWaitObserver;


/**
 * ?one_line_short_description
 *
 * ?more_complete_description
 *
 * @lib ?library
 * @since S60 v4.0
 */
class CXIMPTestGeneralWaiter : public CActive
    {
public:

    IMPORT_C static CXIMPTestGeneralWaiter* NewL( T_MWaitObserver* aObserver );


    virtual ~CXIMPTestGeneralWaiter();


private:

    CXIMPTestGeneralWaiter( T_MWaitObserver* aObserver );
    void ConstructL();


public:

    /**
     * ?description
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */
    IMPORT_C void WaitForL( TInt aSeconds );


private:

    void RunL();


    void DoCancel();


    TInt RunErrorL( TInt aError );


private: // data


    T_MWaitObserver* iObserver;

    RTimer iTimer;

    };

#endif // CPRFWTESTGENERALWAITER_H 
