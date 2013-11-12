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
#ifndef AFASYNCSESSION_H
#define AFASYNCSESSION_H
#include <e32base.h>
class MAfAsyncSession
{
public:
    virtual void PushL(CBase* asyncTask) = 0;
    virtual void Pop(CBase* asyncTask) =0;
    virtual void executeL(int function) =0;
    virtual void sendAsync(int function, const TIpcArgs &args, TRequestStatus& status) =0;
    virtual void getDataL(int id, TDes8& buffer) =0;
};

#endif //AFASYNCSESSION_H
