/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
* CRfsCustCmdTestHelper class declaration.
*
*/


#ifndef RFSCUSTCMDTESTHELPER_H
#define RFSCUSTCMDTESTHELPER_H

#include "asyncrequesthandler.h"
#include "rfscustcmd.h"

/**
 *
 * Helper class used for asynchronous wait. 
 *
 */
NONSHARABLE_CLASS( TWaitInfo )
    {
    public:
    
        CPeriodic* iPeriodic;
        CActiveSchedulerWait* iWait;
    };
   

/**
*  SSM fail policy test helper class
*
*/
NONSHARABLE_CLASS( CRfsCustCmdTestHelper ) : public CBase
    {

public:

    /**
    * Constructs a CRfsCustCmdTestHelper object.
    *
    * @return The new object
    */
    static CRfsCustCmdTestHelper* NewL( CRfsCustCmd::TRfsCustCmdType aType );

    /**
    * Destructor.
    */
    virtual ~CRfsCustCmdTestHelper();

public:

    void RfsInitialize();
    void RfsExecute();
    void RfsExecuteCancel();

private: // From CAsyncRequestHandler
    
    void IssueRequest( TRequestStatus& aStatus );
    void RunL( TInt aStatus );
    TInt RunError( TInt aError );
    void DoCancel();
    
private:

    CRfsCustCmdTestHelper( CRfsCustCmd::TRfsCustCmdType aType );
    void ConstructL();
    
private: // data

    CAsyncRequestHandler<CRfsCustCmdTestHelper>* iAsyncHandler;
    CRfsCustCmd::TRfsCustCmdType iType;
    MSsmCustomCommand* iCmd;
    };

#endif // RFSCUSTCMDTESTHELPER_H
            
