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

#include <AudioClientsListPSKeys.h>
#include "tsptriggerevents.h"
#include "RemConDebug.h"
//-----------------------------------------------------------------------
// CTspTriggerEventsWatcher
//-----------------------------------------------------------------------
CTspTriggerEventsWatcher* CTspTriggerEventsWatcher::NewL(MTspRulesTriggerObserver& aObserver)
    {
    CTspTriggerEventsWatcher* self = new(ELeave)CTspTriggerEventsWatcher(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CTspTriggerEventsWatcher::CTspTriggerEventsWatcher(MTspRulesTriggerObserver& aObserver)
    : iObserver(aObserver)
    {
    }

void CTspTriggerEventsWatcher::ConstructL()
    {
    // Create WServ event watcher
    iWsEventsObserver = CWsEventsObserver::NewL(iObserver);
    
    // Create observer for audio routing
    iAudioClientObserver = CActiveAudioClientObserver::NewL(iObserver);
    
    // Create observer for TSP client mapper
    iClientMapperObserver = CTspClientMapperObserver::NewL(iObserver);
    }

CTspTriggerEventsWatcher::~CTspTriggerEventsWatcher()
    {
    delete iAudioClientObserver;
    delete iWsEventsObserver;
    delete iClientMapperObserver;
    }

//-----------------------------------------------------------------------
// CWsEventsObserver
//-----------------------------------------------------------------------
CWsEventsObserver* CWsEventsObserver::NewL(MTspRulesTriggerObserver& aObserver)
    {
    CWsEventsObserver* self = new(ELeave)CWsEventsObserver(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CWsEventsObserver::~CWsEventsObserver()
    {
    Cancel(); 
    iWindowGroup.Close(); 
    iWsSession.Close();
    }

CWsEventsObserver::CWsEventsObserver(MTspRulesTriggerObserver& aObserver)
    : CActive(CActive::EPriorityStandard), iObserver(aObserver), iWsSession(), iWindowGroup(iWsSession)
    {
    }

void CWsEventsObserver::ConstructL()
    {
    // Register to recieve WServ events
    CActiveScheduler::Add(this);    
    User::LeaveIfError(iWsSession.Connect());
    iWindowGroup = RWindowGroup(iWsSession); 
    User::LeaveIfError(iWindowGroup.Construct(reinterpret_cast<TUint32>(this), EFalse));
    iWindowGroup.SetOrdinalPosition(-1); 
    iWindowGroup.DefaultOwningWindow(); 
    iWindowGroup.EnableReceiptOfFocus(EFalse);    
    iWindowGroup.EnableGroupListChangeEvents(); // For changes in z-order 
    iWsSession.EventReady(&iStatus);
    SetActive(); 
    }


void CWsEventsObserver::RunL()
    {
    COM_TRACE_( "[REMCONTSPCONTROLLER] CWsEventsObserver::RunL" );
    TWsEvent event;
    iWsSession.GetEvent(event);
    iWsSession.EventReady(&iStatus);
    SetActive(); 
    if(event.Type() == EEventWindowGroupListChanged)
        {
        COM_TRACE_( "[REMCONTSPCONTROLLER] CWsEventsObserver::RunL - EEventWindowGroupListChanged" );
        iObserver.MtrtoEvaluateRoutingRules();
        }
    }

void CWsEventsObserver::DoCancel()
    {
    iWsSession.EventReadyCancel();
    }

//-----------------------------------------------------------------------
// CActiveAudioClientObserver
//-----------------------------------------------------------------------
CActiveAudioClientObserver* CActiveAudioClientObserver::NewL(MTspRulesTriggerObserver& aObserver)
    {
    CActiveAudioClientObserver* self = new(ELeave)CActiveAudioClientObserver(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CActiveAudioClientObserver::~CActiveAudioClientObserver()
    {
    Cancel(); 
    iAudioApplication.Close();
    }

CActiveAudioClientObserver::CActiveAudioClientObserver(MTspRulesTriggerObserver& aObserver)
    : CActive(CActive::EPriorityStandard), iObserver(aObserver)
    {
    }

void CActiveAudioClientObserver::ConstructL()
    {
    User::LeaveIfError(iAudioApplication.Attach(KPSUidMMFAudioServer, KAudioPolicyApplicationAudioStatePlaying));
    CActiveScheduler::Add(this);    
    iAudioApplication.Subscribe(iStatus);
    SetActive(); 
    }


void CActiveAudioClientObserver::RunL()
    {
    // Doesn't matter what the value is - that'll be checked when evaluation
    // the TSP target rules.  Inform the TSP of the value change and 
    // re-subscribe.
    COM_TRACE_( "[REMCONTSPCONTROLLER] CWsEventsObserver::RunL" );
    iAudioApplication.Subscribe(iStatus);
    SetActive(); 
    iObserver.MtrtoEvaluateRoutingRules();
    }

void CActiveAudioClientObserver::DoCancel()
    {
    iAudioApplication.Cancel();
    }

//-----------------------------------------------------------------------
// CTspClientMapperObserver
//-----------------------------------------------------------------------
CTspClientMapperObserver* CTspClientMapperObserver::NewL(MTspRulesTriggerObserver& aObserver)
    {
    CTspClientMapperObserver* self = new(ELeave)CTspClientMapperObserver(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CTspClientMapperObserver::~CTspClientMapperObserver()
    {
    Cancel(); 
    }

CTspClientMapperObserver::CTspClientMapperObserver(MTspRulesTriggerObserver& aObserver)
    : CActive(CActive::EPriorityStandard), iObserver(aObserver)
    {
    }

void CTspClientMapperObserver::ConstructL()
    {
    const TUid KTspClientMapperProperty = { 0x10200C70 };
    const TUint32 KTspClientMapperKeyPlaying = 0x00000001;
    User::LeaveIfError(iPlayingClients.Attach(KTspClientMapperProperty, KTspClientMapperKeyPlaying));
    CActiveScheduler::Add(this);    
    iPlayingClients.Subscribe(iStatus);
    SetActive(); 
    }


void CTspClientMapperObserver::RunL()
    {
    // Doesn't matter what the value is - that'll be checked when evaluating
    // the TSP target rules.  Inform the TSP of the value change and 
    // re-subscribe.
    COM_TRACE_( "[REMCONTSPCONTROLLER] CWsEventsObserver::RunL" );
    iPlayingClients.Subscribe(iStatus);
    SetActive(); 
    iObserver.MtrtoEvaluateRoutingRules();
    }

void CTspClientMapperObserver::DoCancel()
    {
    iPlayingClients.Cancel();
    }


