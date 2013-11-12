// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/
#if !defined (__TAMASTARTER_APPOBSERVER_H__)
#define __TAMASTARTER_APPOBSERVER_H__

#include <e32base.h>
#include <e32property.h>

class CTestAmastartAppObserver : public CActive
    {
    public:
    static CTestAmastartAppObserver* NewL(CActiveSchedulerWait* iActiveScheduler);
    void StartL();
    ~CTestAmastartAppObserver();
private:
	CTestAmastartAppObserver();
    void ConstructL(CActiveSchedulerWait* aActiveScheduler);
    void RunL();
    void DoCancel();
private:
	RProperty iProperty;
	CActiveSchedulerWait* iActiveScheduler;
    };

class CTestAmastartTimer : public CTimer
    {
    public:
    static CTestAmastartTimer* NewL(CActiveSchedulerWait* iActiveScheduler, TInt aDelay);
    void StartL();
    ~CTestAmastartTimer();
private:
	CTestAmastartTimer();
    void ConstructL(CActiveSchedulerWait* aActiveScheduler, TInt aDelay);
    void RunL();
    void DoCancel();
private:
	TInt iDelay;
	CActiveSchedulerWait* iActiveScheduler;
    };

#endif //__TAMASTARTER_APPOBSERVER_H__
