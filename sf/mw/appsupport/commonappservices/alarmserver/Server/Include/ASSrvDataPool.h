// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __ASSRVDATAPOOL_H__
#define __ASSRVDATAPOOL_H__

// System includes
#include <s32strm.h>

// User includes
#include "ASSrvAlarmQueueObserver.h"

// Classes referenced
class CASSrvServerWideData;


//
// ----> CASSrvDataPool (header)
//
/**
 * Responsible for maintaining the assocation between AlarmIds and their arbitrary
 * data.
 */
class CASSrvDataPool : public CBase, public MASSrvAlarmQueueObserver
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASSrvDataPool*					NewL(CASSrvServerWideData& aServerWideData);
	~CASSrvDataPool();

//
private:									// INTERNAL CONSTRUCT
//
	CASSrvDataPool(CASSrvServerWideData& aServerWideData);
	void									ConstructL();

//
public:										// ACCESS
//

	TBool									DataPoolContainsEntry(TAlarmId aAlarmId) const;

	const TDesC8&							DataPoolEntry(TAlarmId aAlarmId) const;

	void									DataPoolAddDataL(TAlarmId aAlarmId, HBufC8* aData);

	void									DataPoolRemoveDataL(TAlarmId aAlarmId);

//
public:										// STORE FRAMEWORK
//

	void									InternalizeL(RReadStream& aStream);

	void									ExternalizeL(RWriteStream& aStream) const;

	void									ApplyInternalizedData(TBool aUseNewData);

//
private:									// FROM MASSrvAlarmQueueObserver
//

	void									MAlarmQueueObserverHandleEvent(TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId);

//
private:									// INTERNAL
//

	/**
	 * Access the server wide data
	 */
	inline CASSrvServerWideData&			ServerData() const { return iServerWideData; }

	TInt									FindEntry(TAlarmId aAlarmId) const;

//
private:									// INTERNAL CLASSES
//

	//
	// ----> CASSrvAlarmDataMapplet (header)
	//
	/**
	 * Implements a relationship between alarm id and alarm data
	 */
	class CASSrvAlarmDataMapplet : public CBase
		{
	//
	public:									// STATIC CONSTRUCT / DESTRUCT
	//
		static CASSrvAlarmDataMapplet*		NewLC(TAlarmId aAlarmId, HBufC8* aAlarmData);
		~CASSrvAlarmDataMapplet();

	//
	private:								// INTERNAL CONSTRUCT
	//
		CASSrvAlarmDataMapplet(TAlarmId aAlarmId, HBufC8* aAlarmData);

	//
	public:									// ACCESS
	//

		/**
		 * Return the alarm id
		 */
		inline TAlarmId						Id() const { return iAlarmId; }

		/**
		 * Return a reference to the alarm data for this entry
		 */
		inline const TDesC8&				Data() const { return *iAlarmData; }
	
	//
	private:								// INTERNAL CONSTRUCT
	//

		/**
		 * The alarm id
		 */
		TAlarmId							iAlarmId;

		/**
		 * The alarm data
		 */
		HBufC8*								iAlarmData;
		};

//
private:									// MEMBER DATA
//

	/**
	 * A handle to the server wide data.
	 */
	CASSrvServerWideData&					iServerWideData;
	
	/**
	 * The list of mappings between Alarm Id's and their associated arbitrary data.
	 */
	RPointerArray<CASSrvAlarmDataMapplet>	iDataEntries;

	/**
	 * Buffer for Internalize to read in The list of mappings between Alarm Id's
	 * and their associated arbitrary data.
	 */
	RPointerArray<CASSrvAlarmDataMapplet>	iInternalizeDataEntries;
	};

#endif
