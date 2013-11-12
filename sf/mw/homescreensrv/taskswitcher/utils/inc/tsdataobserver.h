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
#ifndef DATAOBSERVER_H
#define DATAOBSERVER_H

#include <e32base.h>

class MTsDataObserver {
public:
    virtual void DataChanged() =0;
};

class CTsDataObserver: public CBase,
                       public MTsDataObserver
{
};

class MTsDataObserverStorage
{
public:
    virtual void PushL(CTsDataObserver*) =0;
    virtual void Pop(CTsDataObserver*) =0;
};

#endif //DATAOBSERVER_H
