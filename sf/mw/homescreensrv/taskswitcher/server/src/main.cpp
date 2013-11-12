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
*
*/

#include <QApplication>

#include "tsrunningappserver.h"
#include "tstaskmonitorglobals.h"
#include <apgwgnam.h>
#include <eikenv.h>

#include <caservice.h>

int main(int argc, char *argv[]) 
{
    RSemaphore semaphore;
    int errNo( semaphore.CreateGlobal(KTsServerReadySemaphore, 0) );
    if (KErrNone == errNo) {
        QApplication app(argc, argv);
        // maintain instance of caservice (needed for getting application icons)
        QSharedPointer<CaService> service = CaService::instance();
        TRAP(errNo,
        // hide server from TaskSwitcher
        CEikonEnv * env = CEikonEnv::Static();
        User::LeaveIfNull( env );
        env->RootWin().SetOrdinalPosition(0, ECoeWinPriorityNeverAtFront);
        CApaWindowGroupName *wgName = CApaWindowGroupName::NewLC(env->WsSession());
        wgName->SetHidden(ETrue); // hides us from FSW and protects us from OOM FW etc.
        wgName->SetSystem(ETrue); // Allow only application with PowerManagement cap to shut us down
        wgName->SetCaptionL(KRunningAppServerName);
        wgName->SetWindowGroupName(env->RootWin());
        CleanupStack::PopAndDestroy(wgName);
        CServer2* serverObject = CTsRunningAppServer::NewLC();
        semaphore.Signal();
        QT_TRYCATCH_LEAVING(errNo = app.exec());
        User::LeaveIfError(errNo);
        CleanupStack::PopAndDestroy(serverObject);
        )//TRAP
        semaphore.Close();
    }
    return errNo;
}
