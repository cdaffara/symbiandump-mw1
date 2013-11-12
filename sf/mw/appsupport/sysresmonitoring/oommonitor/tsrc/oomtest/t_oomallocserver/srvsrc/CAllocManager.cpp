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
* Description:  
*
*/




#include "CAllocManager.h"
#include <e32property.h>
#include "t_oomdummyplugin_properties.h"

const TInt KOneSecond = 1000000;

CAllocManager::CAllocManager()
	{
	// No implementation required
	}

CAllocManager::~CAllocManager()
	{
	delete iAllocationTimer;
	iSimulations.ResetAndDestroy();
	}

CAllocManager* CAllocManager::NewLC()
	{
	CAllocManager* self = new (ELeave) CAllocManager();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CAllocManager* CAllocManager::NewL()
	{
	CAllocManager* self = CAllocManager::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CAllocManager::ConstructL()
	{
	iAllocationTimer = CPeriodic::NewL(CActive::EPriorityStandard);
	}

TInt CAllocManager::SimulationTickStatic(TAny *aPtr)
	{
	((CAllocManager*)aPtr)->SimulationTick();
	return KErrNone;
	}

void CAllocManager::SimulationTick()
	{
	for(TInt i=iSimulations.Count()-1; i>=0; i--)
		{
		iSimulations[i]->SimulationTick();
		}
	}

TInt CAllocManager::Reset()
	{
	StopAllocating();
	iSimulations.ResetAndDestroy();
	return KErrNone;
	}

TInt CAllocManager::StartAllocating()
	{
	iAllocationTimer->Cancel();
	iAllocationTimer->Start(KOneSecond, KOneSecond, TCallBack(SimulationTickStatic, this));
	return KErrNone;
	}

TInt CAllocManager::StopAllocating()
	{
	iAllocationTimer->Cancel();
	return KErrNone;
	}

void CAllocManager::ConfigureL(TUint aPlugin, TUint aAllocRate, TUint aAllocInitial, TUint aAllocLimit)
	{
	TInt sim = FindSimulation(aPlugin);
	if(sim>=0)
		{
		delete iSimulations[sim];
		iSimulations.Remove(sim);
		}
	CAllocSimulation* newsim = CAllocSimulation::NewLC(aAllocLimit, aAllocInitial, aAllocRate, aPlugin);
	iSimulations.AppendL(newsim);
	CleanupStack::Pop(newsim);
	}

TInt CAllocManager::MemoryLow(TUint aPlugin)
	{
	TInt sim = FindSimulation(aPlugin);
	if(sim>=0)
		{
		iSimulations[sim]->SetPaused(ETrue);
		iSimulations[sim]->FreeMemory();
		return KErrNone;
		}
	else return KErrNotFound;
	}

TInt CAllocManager::MemoryGood(TUint aPlugin)
	{
	TInt sim = FindSimulation(aPlugin);
	if(sim>=0)
		{
		iSimulations[sim]->SetPaused(EFalse);
		return KErrNone;
		}
	else return KErrNotFound;
	}

TInt CAllocManager::FindSimulation(TUint aUid)
	{
	return iSimulations.Find<TUint>(aUid, CAllocSimulation::CompareTo);
	}

CAllocSimulation::CAllocSimulation(TInt aAllocLimit, TInt aAllocInitial,
		TInt aAllocRate, TUint aUid) : 
		iAllocLimit(aAllocLimit),
		iAllocInitial(aAllocInitial),
		iAllocRate(aAllocRate),
		iUid(aUid)
	{
	// No implementation required
	}

CAllocSimulation::~CAllocSimulation()
	{
	iChunk.Close();
	}

CAllocSimulation* CAllocSimulation::NewLC(TInt aAllocLimit, TInt aAllocInitial,
		TInt aAllocRate, TUint aUid)
	{
	CAllocSimulation* self = new (ELeave) CAllocSimulation(aAllocLimit, aAllocInitial,
			aAllocRate, aUid);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CAllocSimulation* CAllocSimulation::NewL(TInt aAllocLimit, TInt aAllocInitial,
		TInt aAllocRate, TUint aUid)
	{
	CAllocSimulation* self = CAllocSimulation::NewLC(aAllocLimit, aAllocInitial,
			aAllocRate, aUid);
	CleanupStack::Pop(); // self;
	return self;
	}

void CAllocSimulation::ConstructL()
	{
	User::LeaveIfError(iChunk.CreateLocal(iAllocInitial, iAllocLimit));
	}

void CAllocSimulation::SimulationTick()
	{
	if(!iPaused)
		{
		TUint allocnext = iAllocCurrent + iAllocRate;
		if(allocnext < iAllocInitial) allocnext = iAllocInitial;
		else if(allocnext > iAllocLimit) allocnext = iAllocLimit;
		if(iAllocCurrent != allocnext && KErrNone == iChunk.Adjust(iAllocCurrent))
			{
			iAllocCurrent = allocnext;
			RProperty::Set(KUidOomPropertyCategory, iUid + KOOMDummyPluginCurrentAllocationBytes, iAllocCurrent);
			}
		}
	}

void CAllocSimulation::SetPaused(TBool aPaused)
	{
	iPaused = aPaused;
	}

void CAllocSimulation::FreeMemory()
	{
	iChunk.Adjust(0);
	iAllocCurrent = 0;
	RProperty::Set(KUidOomPropertyCategory, iUid + KOOMDummyPluginCurrentAllocationBytes, iAllocCurrent);
	}

TBool CAllocSimulation::CompareTo(const TUint* aUid, const CAllocSimulation& aSelf)
	{
	return *aUid==aSelf.iUid;
	}

