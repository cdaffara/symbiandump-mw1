/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Observes changes in the default player that should be used for a bearer
*/

#include <e32base.h>
#include <e32property.h>
#include <w32std.h>


NONSHARABLE_CLASS(MTspRulesTriggerObserver)
    {
public:
    virtual void MtrtoEvaluateRoutingRules() = 0;
    };

NONSHARABLE_CLASS(CActiveAudioClientObserver) : public CActive
    {
public:
    static CActiveAudioClientObserver* NewL(MTspRulesTriggerObserver& aObserver);
    ~CActiveAudioClientObserver();

private:
    // from CActive
    void RunL();
    void DoCancel();
    
    CActiveAudioClientObserver(MTspRulesTriggerObserver& aObserver);
    void ConstructL();

private:
    MTspRulesTriggerObserver& iObserver;
    RProperty iAudioApplication;
    };

NONSHARABLE_CLASS(CTspClientMapperObserver) : public CActive
    {
public:
    static CTspClientMapperObserver* NewL(MTspRulesTriggerObserver& aObserver);
    ~CTspClientMapperObserver();

private:
    // from CActive
    void RunL();
    void DoCancel();
    
    CTspClientMapperObserver(MTspRulesTriggerObserver& aObserver);
    void ConstructL();

private:
    MTspRulesTriggerObserver& iObserver;
    RProperty iPlayingClients;
    };

NONSHARABLE_CLASS(CWsEventsObserver) : public CActive
    {
public:
    static CWsEventsObserver* NewL(MTspRulesTriggerObserver& aObserver);
    ~CWsEventsObserver();
    
private:
    // from CActive
    void RunL();
    void DoCancel();
    
    CWsEventsObserver(MTspRulesTriggerObserver& aObserver);
    void ConstructL();
    
private:
    MTspRulesTriggerObserver& iObserver;
    RWsSession iWsSession;
    RWindowGroup iWindowGroup;
    };

NONSHARABLE_CLASS(CTspTriggerEventsWatcher) : public CBase
    {
public:
    static CTspTriggerEventsWatcher* NewL(MTspRulesTriggerObserver& aObserver);
    ~CTspTriggerEventsWatcher();
    TInt AddBearer(TUid& aBearerUid);
    TInt RemoveBearer(TUid& aBearerUid);

private:
    CTspTriggerEventsWatcher(MTspRulesTriggerObserver& aObserver);
    void ConstructL();

private:
    MTspRulesTriggerObserver& iObserver;
    CActiveAudioClientObserver* iAudioClientObserver;
    CWsEventsObserver* iWsEventsObserver;
    CTspClientMapperObserver* iClientMapperObserver;
    };

