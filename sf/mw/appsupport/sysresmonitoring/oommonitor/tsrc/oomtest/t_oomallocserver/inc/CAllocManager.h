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





#ifndef CALLOCMANAGER_H
#define CALLOCMANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

class CAllocSimulation : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAllocSimulation();

	/**
	 * Two-phased constructor.
	 */
	static CAllocSimulation* NewL(TInt aAllocLimit, TInt aAllocInitial,
			TInt aAllocRate, TUint aUid);

	/**
	 * Two-phased constructor.
	 */
	static CAllocSimulation* NewLC(TInt aAllocLimit, TInt aAllocInitial,
			TInt aAllocRate, TUint aUid);

	void SimulationTick();
	void SetPaused(TBool aPaused);
	void FreeMemory();
	
	static TBool CompareTo(const TUint *aUid, const CAllocSimulation& aSelf);
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CAllocSimulation(TInt aAllocLimit, TInt aAllocInitial,
			TInt aAllocRate, TUint aUid);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	TInt iAllocLimit;
	TInt iAllocInitial;
	TInt iAllocRate;
	TUint iUid;
	RChunk iChunk;
	TBool iPaused;
	TUint iAllocCurrent;
	};

/**
 *  CCAllocManager
 * 
 */
class CAllocManager : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAllocManager();

	/**
	 * Two-phased constructor.
	 */
	static CAllocManager* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CAllocManager* NewLC();

	TInt Reset();
	TInt StartAllocating();
	TInt StopAllocating();
	void ConfigureL(TUint aPlugin, TUint aAllocRate, TUint aAllocInitial, TUint aAllocLimit);
	TInt MemoryLow(TUint aPlugin);
	TInt MemoryGood(TUint aPlugin);
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CAllocManager();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	static TInt SimulationTickStatic(TAny *aPtr);
	void SimulationTick();

	TInt FindSimulation(TUint aUid);
	
	CPeriodic *iAllocationTimer;
	RPointerArray<CAllocSimulation> iSimulations;
	};

#endif // CALLOCMANAGER_H
