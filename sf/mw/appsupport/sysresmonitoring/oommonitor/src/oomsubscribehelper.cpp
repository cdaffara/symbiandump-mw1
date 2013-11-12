/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Main classes for Out of Memory Monitor.
*
*/

#include <e32property.h>
#include "oomsubscribehelper.h"
#include "OomTraces.h"

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CSubscribeHelper::CSubscribeHelper(TCallBack aCallBack, RProperty& aProperty)
    : CActive(EPriorityNormal), iCallBack(aCallBack), iProperty(aProperty)
    {
    FUNC_LOG;

    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CSubscribeHelper::~CSubscribeHelper()
    {
    FUNC_LOG;

    Cancel();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CSubscribeHelper::Subscribe()
    {
    FUNC_LOG;

    if (!IsActive())
        {
        iProperty.Subscribe(iStatus);
        SetActive();
        }
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CSubscribeHelper::StopSubscribe()
    {
    FUNC_LOG;

    Cancel();
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CSubscribeHelper::RunL()
    {
    FUNC_LOG;

    if (iStatus.Int() == KErrNone)
        {
        iCallBack.CallBack();
        Subscribe();
        }
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CSubscribeHelper::DoCancel()
    {
    FUNC_LOG;

    iProperty.Cancel();
    }

