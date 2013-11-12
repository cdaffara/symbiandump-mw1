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
#ifndef AFASYNCREQUESTPRIVATE_H
#define AFASYNCREQUESTPRIVATE_H

#include <e32base.h>
#include <afcmd.h>

#include "afasyncrequestobserver.h"
#include "afasyncsession.h"

class CAfAsyncRequestPrivate : public CActive

{
public:
    static void NewLD(MAfAsyncRequestObserver &observer, 
                      MAfAsyncSession & session,
                      ActivityCmd cmd);

    ~CAfAsyncRequestPrivate();

protected:
    void DoCancel();

    void RunL();

    TInt RunError(TInt error);

private:
    CAfAsyncRequestPrivate(MAfAsyncRequestObserver &, 
                           MAfAsyncSession &,
                           ActivityCmd);
    void ConstructL();

private:
    MAfAsyncRequestObserver &mObserver;
    MAfAsyncSession &mSession;
    const ActivityCmd mCmd;
    TPckgBuf<TInt> mIds[4];
};
#endif // AFASYNCREQUESTPRIVATE_H
