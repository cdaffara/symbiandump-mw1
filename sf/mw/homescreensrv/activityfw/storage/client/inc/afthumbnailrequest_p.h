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
#ifndef AFTHUMBNAILREQUESTPRIVATE_H
#define AFTHUMBNAILREQUESTPRIVATE_H

#include <e32base.h>

#include "afasyncrequestobserver.h"
#include "afasyncsession.h"

class CAfThumbnailRequestPrivate : public CActive

{
public:
    static void NewLD(MAfAsyncRequestObserver &observer, 
                      MAfAsyncSession & session,
                      const TDesC& source,
                      TAny* userData);

    ~CAfThumbnailRequestPrivate();

protected:
    void DoCancel();

    void RunL();

    TInt RunError(TInt error);

private:
    CAfThumbnailRequestPrivate(MAfAsyncRequestObserver &, 
                               MAfAsyncSession &, 
                               TAny*);

    void ConstructL(const TDesC& source);

private:
    MAfAsyncRequestObserver &mObserver;
    MAfAsyncSession &mSession;
    TPckgBuf<TInt> mIds[4]; 
    RBuf mImgSrc;
    TAny* mUserData;
};
#endif // AFTHUMBNAILREQUESTPRIVATE_H
