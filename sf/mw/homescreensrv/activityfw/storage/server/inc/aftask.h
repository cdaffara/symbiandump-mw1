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
#ifndef AFTASK_H
#define AFTASK_H

#include <e32base.h>

class CAfTask: public CBase
{
public:
    /**
     * Return task data 
     */
    virtual const TDesC8& Data() const =0;
    
    /**
     * Notify instance about incoming broadcast message.
     * @param msg - broadcast message
     */
    virtual void BroadcastReceivedL(const RMessage2& msg, TBool applicationUninstalled = EFalse) =0;
    
    /**
     * Returns ETrue if task is related with session argument
     */
    virtual TBool IsSessionTask(const CSession2* session) =0;
};

#endif  //AFTASK_H

