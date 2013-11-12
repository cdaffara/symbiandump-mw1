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

#ifndef __ASSRVALARMSTORE_H__
#define __ASSRVALARMSTORE_H__

// System includes
#include <e32base.h>
#include <s32file.h>
#include "CBackupRestoreNotification.h"

// User includes
#include "ASSrvAlarmQueue.h"
#include "ASSrvAlarmQueueObserver.h"
#include "ASSrvAnyEventObserver.h"
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
#include "assrvsystemstateobserver.h"
#endif

// Type definitions

// Constants

// Enumerations

// Classes referenced
class CASSrvServerWideData;


//
// ----> CASSrvAlarmStore (header)
//
/**
 * This object implements the logic for keeping a persistent backup of the
 * Alarm Server queue in the file system. It also monitors for Backup/restore operations.
 * When the AlarmServer.ini is to be backed up or replaced.
 */
class CASSrvAlarmStore : public CTimer, 
#ifndef __WINC__
						 public MBackupRestoreNotificationObserver,
				
#endif
						 public MASSrvAlarmQueueObserver, public MASSrvAnyEventObserver
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
, public MASSrvSystemStateObserver
#endif
	{
//
public:										// STATIC CONSTRUCT / DESTRUCT
//
	static CASSrvAlarmStore*				NewL(CASSrvServerWideData& aServerWideData);
	~CASSrvAlarmStore();

//
private:									// INTERNAL CONSTRUCT
//
	CASSrvAlarmStore(CASSrvServerWideData& aServerWideData);
	void									ConstructL();

//
private:									// FROM MASSrvAlarmQueueObserver
//

	/**
	 * @see MASSrvAlarmQueueObserver
	 */
	void									MAlarmQueueObserverHandleEvent(TASSrvAlarmQueueEvent aEvent, TAlarmId aAlarmId);

//
public:										// FROM MASSrvAnyEventObserver
//

	/**
	 * @see MASSrvAnyEventObserver
	 */

	void									MASSrvAnyEventHandleChange(TAlarmChangeEvent aEvent, TAlarmId aAlarmId);

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
public: // From MASSrvSystemStateObserver
	void									MHandleSystemStateChange(TState aState);
#endif
	
//
private:									// INTERNAL
//

	/**
	 * Access the server wide data
	 */
	inline CASSrvServerWideData&			ServerWideData() const { return iServerWideData; }

	/**
	 * Internalize (non-leaving) the contents of the Alarm Server queue 
	 * and some settings from the backup file.
	 * @return KErrNone or other standard error code.
	 */
	TInt									Internalize(CASSrvAlarmQueue::TStoreOperation aInternalizeOperation);

	/**
	 * Internalize the contents of the Alarm Server queue & some settings
	 * from the backup file.
	 */
	void									InternalizeL();

	/**
	 * If Alarm changes were seen previously Externalize the contents of 
	 * the Alarm Server backup to the backup file.
	 * @return KErrNone or other standard error code.
	 */
	TInt									ExternalizeIfRequested();

	/**
	 * Externalize (non-leaving) the Alarm Server queue & some settings 
	 * to the backup file.
	 * @return KErrNone or other standard error code.
	 */
	TInt									Externalize();

	/**
	 * Externalize the Alarm Server queue & some settings to the backup file.
	 */
	void									ExternalizeL();

	/**
	 * Open the backup store (AlarmServer.ini) using the specified
	 * file mode.
	 *
	 * @return A file store object which encapsulates the backup store.
	 */
	CDirectFileStore*						OpenStoreLC(TUint aMode) const;

	/**
	 * Open the backup store (AlarmServer.ini) using the specified
	 * file mode.
	 *
	 * @return A file store object which encapsulates the backup store.
	 */
	CDirectFileStore*						OpenStoreL(TUint aMode) const;

	/**
	 * Restore the root stream dictionary from the specified store
	 *
	 * @return A stream dictionary object on the cleanup stack
	 */
	CStreamDictionary*						OpenDictionaryLC(CPersistentStore& aStore) const;

	/**
	 * Open the specified stream in the store
	 */
	void									OpenStreamLC(CPersistentStore& aStore, CStreamDictionary& aDictionary, TUid aStreamUid, RStoreReadStream& aStream) const;

	/**
	 * Retry a previously requested backup operation
	 */
	void									RetryStoreOperation();

	void StartOfBackupOrRestoreL(CASSrvAlarmQueue::TStoreOperation aStoreOperation);

//
private:									// FROM CActive
//

	/**
	 * @see CActive
	 */
	void									RunL();

	/**
	 * @see CActive
	 */
	TInt									RunError(TInt aError);

#ifndef __WINC__
//
private:									// FROM MBackupRestoreNotificationObserver
//
	void BackupBeginningL();
	void BackupCompletedL();

	void RestoreBeginningL();
	void RestoreCompletedL(TInt aRestoreResult);

#endif
//
private:									// INTERNAL ENUMERATIONS
//

	/**
	 *  Internal flags
	 */
	enum TBackupFlags
		{
		/**
		 * Indicates that the Alarm Server performing an Internalize, so 
		 * the AlarmServer.ini file is in use.  We are also not interested 
		 * in changes to the alarm queue - they come from the Internalize
		 * and do not need to cause an Externalize.
		 */
		EIsInternalizing = 1,

		/**
		 * Indicates that some change to Alarm data has been observed and
		 * that an Externalize is required.
		 * If RunL sees this flag it will try to perform an Externalize,
		 * and retry a number of times if an error is encountered.
		 */
		ERequestExternalize = 2
		};

//
private:									// INTERNAL METHODS
//

	void									ScheduleExternalizeWithRetry(TUint aTimeMS);

//
private:									// MEMBER DATA
//

	/**
	 * The server wide data
	 */
	CASSrvServerWideData&					iServerWideData;

	/**
	 * The number of retry attempts that have already taken
	 * place without success
	 */
	TInt									iNumberOfAttemptedRetries;

	/**
	 * Internal flags
	 */
	TBitFlags								iFlags;

#ifndef __WINC__
	CBackupRestoreNotification*				iBackupNotification;
#endif
	};

#endif
