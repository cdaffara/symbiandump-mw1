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

#include "ASSrvAlarmStore.h"

// User includes
#include "ASSrvDefs.h"
#include "ASSrvDataPool.h"
#include "ASSrvAlarmQueue.h"
#include "ASSrvStaticUtils.h"
#include "ASSrvSoundSettings.h"
#include "ASSrvServerWideData.h"
#include "ASSrvAlarmQueue.h"

// Type definitions
#define UNUSED_VAR(a) a = a

// Constants
const TInt KDelayInMicrosecondsBetweenFileOperations = 2000000; // 2 seconds
const TInt KDelayInMicrosecondsBetweenFileOperationsLongDelay = 10 * 60 * 1000000; // 10 minutes, allow user time to clear disk space?
const TInt KNumberOfAttemptsAtExternalizingFile = 5; // 5 * 2 seconds => 10 seconds
const TInt KDelayPeriodBeforeAttemptingAnExternalize = 1000; // 1 seconds

// Enumerations

// Definitions
#define __START_WITH_RESTORED_QUEUE__

// Classes referenced

//#define VERBOSE_DEBUG

#if defined(VERBOSE_DEBUG)
#define DEBUG_PRINT1(A)			{ RDebug::Print(A); }
#define DEBUG_PRINT2(A,B)		{ RDebug::Print(A,B); }
#define DEBUG_PRINT3(A,B,C)		{ RDebug::Print(A,B,C); }
#else
#define DEBUG_PRINT1(A)
#define DEBUG_PRINT2(A,B)
#define DEBUG_PRINT3(A,B,C)
#endif



//
// ----> CASSrvAlarmStore (source)
//

//*************************************************************************************
CASSrvAlarmStore::CASSrvAlarmStore(CASSrvServerWideData& aServerWideData)
:	CTimer(CActive::EPriorityIdle), iServerWideData(aServerWideData)
	{
	CActiveScheduler::Add(this);
	}


//*************************************************************************************
CASSrvAlarmStore::~CASSrvAlarmStore()
	{
#ifndef __WINC__
	delete iBackupNotification;	
#endif

	ServerWideData().Queue().NotificationPoolChangeCancel(*this);

	DEBUG_PRINT1(_L("-- -- Alarm Store destructor"));

	// is an externalize request still waiting?
	if (iFlags.IsSet(ERequestExternalize))
		{
		Cancel();
		// Flush current alarm queue
		// (if this fails now there is nothing we can do about it)
		Externalize();
		}

	// no longer interested in these events
	ServerWideData().AnyEventManager().MASAnyEventManagerObserverRemove(*this);
	}


//*************************************************************************************
void CASSrvAlarmStore::ConstructL()
	{
	CTimer::ConstructL();

	DEBUG_PRINT1(_L("++ ++ Alarm Store ConstructL"));

	TFileName name;
	RFs& fsSession = ServerWideData().FsSession();
	ASSrvStaticUtils::GetPrivateDirL(fsSession, name);

	//Check INI file directory exists
	TUint attribs;	//Not used dummy parameter
	TInt error = fsSession.Att(name,attribs);
	if (error == KErrNotFound || error == KErrPathNotFound)
		{
		error = fsSession.MkDirAll(name);
		}
	User::LeaveIfError(error);

	ASSrvStaticUtils::GetServerPathL(fsSession, name);

#ifndef __WINC__
	iBackupNotification = CBackupRestoreNotification::NewL(name, *this);
#endif

	// Internalize any previously persisted data
	error = Internalize(CASSrvAlarmQueue::EStoreInternalizeStartup);
	if	(error != KErrNone)
		{
		error = fsSession.Delete(name);
		if	(error != KErrNotFound)
			User::LeaveIfError(error);
		}

	// Ready to watch for "Alarm Added" and other Alarm Queue events
	ServerWideData().Queue().NotificationPoolChangeL(*this);

	// Ready to watch for Alarm Sound Intervals changing
	ServerWideData().AnyEventManager().MASAnyEventManagerObserverAddL(*this);
	}


//*************************************************************************************
CASSrvAlarmStore* CASSrvAlarmStore::NewL(CASSrvServerWideData& aServerWideData)
	{
	CASSrvAlarmStore* self = new(ELeave) CASSrvAlarmStore(aServerWideData);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


//
//
//




//*************************************************************************************
/**
 * @see MASSrvAlarmQueueObserver
 */
void CASSrvAlarmStore::MAlarmQueueObserverHandleEvent(TASSrvAlarmQueueEvent aEvent, TAlarmId /*aAlarmId*/)
	{
	switch(aEvent)
		{
	case EASSrvAlarmQueueEventHeadItemChanged:
	case EASSrvAlarmQueueEventAlarmAdded:
	case EASSrvAlarmQueueEventAlarmChanged:
	case EASSrvAlarmQueueEventAlarmDeleted:
		// RunL will Externalize all changes in 100ms
		ScheduleExternalizeWithRetry(100);
		break;

	default:
		break;
		}
	}


//
//
//


//*************************************************************************************
/**
 * @see MASSrvAnyEventObserver
 *
 * Facade interface that notifies observer about event. No real processing done
 * by this object.
 */
void CASSrvAlarmStore::MASSrvAnyEventHandleChange(TAlarmChangeEvent aEvent, TAlarmId /*aAlarmId*/)
	{
	switch(aEvent)
		{
	case EAlarmChangeEventPlayIntervalsChanged:
		// The sound intervals associated with sound timing have changed.
		// RunL will Externalize all changes in 100ms
		ScheduleExternalizeWithRetry(100);
		break;

	default:
		break;
		}
	}


//
//
//


//*************************************************************************************
/**
 * Internalize (non-leaving) the contents of the Alarm Server backup
 * from the backup file.
 */
TInt CASSrvAlarmStore::Internalize(CASSrvAlarmQueue::TStoreOperation aInternalizeOperation)
	{
	TInt error = KErrNone;

	DEBUG_PRINT1(_L("> Alarm Store Internalize ()"));

	// tell Alarm Queue what type of Internalize to perform
	error = ServerWideData().Queue().StartAlarmStoreOperation(aInternalizeOperation);

	if (!error)
		{
		// don't watch for change notifications during Internalize
		iFlags.Set(EIsInternalizing);

		TRAP(error, InternalizeL());

		// tell alarm queue that Internalize is complete
		ServerWideData().Queue().EndAlarmStoreOperation(error);

		// Finished Internalize, Look for notifications again, etc...
		iFlags.Clear(EIsInternalizing);
		}

	DEBUG_PRINT2(_L("< Alarm Store Internalize - error %i"), error);

	return error;
	}


//*************************************************************************************
/**
 * Internalize the contents of the Alarm Server backup
 * from the backup file.
 */
void CASSrvAlarmStore::InternalizeL()
	{
	RStoreReadStream stream;
	CDirectFileStore* store = OpenStoreLC(EFileRead);
	CStreamDictionary* dictionary = OpenDictionaryLC(*store);

	// Open the alarm queue stream & internalize
#ifdef __START_WITH_RESTORED_QUEUE__
	OpenStreamLC(*store, *dictionary, KAlarmStoreStreamUidQueue, stream);
	stream >> ServerWideData().Queue();
	CleanupStack::PopAndDestroy(&stream);

	// Open the alarm data stream & internalize
	OpenStreamLC(*store, *dictionary, KAlarmStoreStreamUidData, stream);
	stream >> ServerWideData().DataPool();
	CleanupStack::PopAndDestroy(&stream);
#endif

	// Open the sound settings stream & internalize
	OpenStreamLC(*store, *dictionary, KAlarmStoreStreamUidSoundSettings, stream);
	stream >> ServerWideData().SoundSettings();
	CleanupStack::PopAndDestroy(&stream);

#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS
	// Open the missed alarm data stream & internalize
	OpenStreamLC(*store, *dictionary, KAlarmStoreStreamUidEnvironmentChangeData, stream);
	ServerWideData().Queue().InternalizeLastAlarmedInstanceParamsL(stream);
	CleanupStack::PopAndDestroy(&stream);
#endif
	
	// Tidy up
	CleanupStack::PopAndDestroy(2, store);
	}
	
//*************************************************************************************
/**
 * Externalize (non-leaving) the contents of the Alarm Server backup
 * to the backup file.
 */
TInt CASSrvAlarmStore::ExternalizeIfRequested()
	{
	TInt error = KErrNone;

	DEBUG_PRINT2(_L("= Alarm Store Externalize If Requested (%u)"), iFlags.IsSet(ERequestExternalize));

	if (iFlags.IsSet(ERequestExternalize))
		{
		// Don't RunL
		Cancel();

		error = Externalize();
		}

	return error;
	}


//*************************************************************************************
/**
 * Externalize (non-leaving) the contents of the Alarm Server backup
 * to the backup file.
 */
TInt CASSrvAlarmStore::Externalize()
	{
	TInt error = KErrNone;
	
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	if (!iServerWideData.ServerIsReadOnly())
		{
#endif
		DEBUG_PRINT1(_L("> Alarm Store Externalize"));
	
		// tell Alarm Queue what type of Store operation we want to perform
		error = ServerWideData().Queue().StartAlarmStoreOperation(CASSrvAlarmQueue::EStoreExternalize);
	
		if (!error)
			{
			TRAP(error, ExternalizeL());
	
			// tell alarm queue that Externalize is complete
			ServerWideData().Queue().EndAlarmStoreOperation(error);
			}
	
		DEBUG_PRINT2(_L("< Alarm Store Externalize - error %i"), error);
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
		}
#endif
	
	return error;
	}


//*************************************************************************************
/**
 * Externalize the contents of the Alarm Server backup
 * to the backup file.
 */
void CASSrvAlarmStore::ExternalizeL()
	{
	TStreamId streamId;
	RStoreWriteStream stream;
	CStreamDictionary* dictionary = CStreamDictionary::NewLC();

	CDirectFileStore* store = OpenStoreLC(EFileWrite | EFileShareExclusive);

	// Create the alarm queue stream & externalize
	streamId = stream.CreateLC(*store);
	stream << ServerWideData().Queue();
	stream.CommitL();
	CleanupStack::PopAndDestroy(&stream);
	dictionary->AssignL(KAlarmStoreStreamUidQueue, streamId);

	// Create the alarm data queue & externalize
	streamId = stream.CreateLC(*store);
	stream << ServerWideData().DataPool();
	stream.CommitL();
	CleanupStack::PopAndDestroy(&stream);
	dictionary->AssignL(KAlarmStoreStreamUidData, streamId);

	// Create the alarm data queue & externalize
	streamId = stream.CreateLC(*store);
	stream << ServerWideData().SoundSettings();
	stream.CommitL();
	CleanupStack::PopAndDestroy(&stream);
	dictionary->AssignL(KAlarmStoreStreamUidSoundSettings, streamId);

#ifdef SYMBIAN_SKIPPED_CALENDAR_ALARMS
	// Create the missed alarm data stream & externalize
	streamId = stream.CreateLC(*store);
	ServerWideData().Queue().ExternalizeLastAlarmedInstanceParamsL(stream);
	stream.CommitL();
	CleanupStack::PopAndDestroy(&stream);
	dictionary->AssignL(KAlarmStoreStreamUidEnvironmentChangeData, streamId);	
#endif
	
	// Create root stream
	streamId = stream.CreateLC(*store);
	stream << *dictionary;
	stream.CommitL();
	CleanupStack::PopAndDestroy(&stream);
	store->SetRootL(streamId);
	store->CommitL();

	// clear Externalize request flag now, so another can be requested
	iFlags.Clear(ERequestExternalize);

	// Tidy up
	CleanupStack::PopAndDestroy(2, dictionary);
	}


//*************************************************************************************
/**
 * Open the backup store (AlarmServer.ini) using the specified
 * file mode.
 *
 * @return A file store object which encapsulates the backup store.
 */
CDirectFileStore* CASSrvAlarmStore::OpenStoreLC(TUint aMode) const
	{
	CDirectFileStore* store = OpenStoreL(aMode);

	CleanupStack::PushL(store);
	//
	return store;
	}


/**
 * Open the backup store (AlarmServer.ini) using the specified
 * file mode.
 *
 * @return A file store object which encapsulates the backup store.
 */
CDirectFileStore* CASSrvAlarmStore::OpenStoreL(TUint aMode) const
	{
	RFs& fsSession = ServerWideData().FsSession();
	TFileName fileName;
	ASSrvStaticUtils::GetServerPathL(fsSession, fileName);

	CDirectFileStore* store = NULL;
	TInt error = KErrNone;
	//
	if	(aMode == EFileRead)
		{
		// First try to open the store. If opening fails, then we
		// replace the store with a blank one.
		TRAP(error, store = CDirectFileStore::OpenL(fsSession, fileName, aMode));
		}
	//
	if	(!store || error != KErrNone)
		{
		store = CDirectFileStore::ReplaceLC(fsSession, fileName, aMode);
		//
		const TUidType type(KDirectFileStoreLayoutUid, KAlarmStoreUid, KNullUid);
		store->SetTypeL(type);
		//
		CleanupStack::Pop(store);
		}
	//
	return store;
	}


//*************************************************************************************
/**
 * Restore the root stream dictionary from the specified store
 *
 * @return A stream dictionary object on the cleanup stack
 */
CStreamDictionary* CASSrvAlarmStore::OpenDictionaryLC(CPersistentStore& aStore) const
	{
	const TStreamId rootId = aStore.Root();
	//
	CStreamDictionary* dictionary = CStreamDictionary::NewLC();
	RStoreReadStream stream;
	stream.OpenLC(aStore, rootId);
	stream >> *dictionary;
	CleanupStack::PopAndDestroy(&stream);
	//
	return dictionary;
	}


//*************************************************************************************
/**
 * Open the specified stream in the store
 */
void CASSrvAlarmStore::OpenStreamLC(CPersistentStore& aStore, CStreamDictionary& aDictionary, TUid aStreamUid, RStoreReadStream& aStream) const
	{
	const TStreamId streamId = aDictionary.At(aStreamUid);
	aStream.OpenLC(aStore, streamId);
	}


//*************************************************************************************
/**
 * Retry a previously requested backup operation
 */
void CASSrvAlarmStore::RetryStoreOperation()
	{
	if (iFlags.IsSet(ERequestExternalize) && !IsActive())
		{
		// can RunL now
		After(0);
		}
	}


//
//
//


//*************************************************************************************
/**
 * @see CActive
 */
void CASSrvAlarmStore::RunL()
	{
	TInt error = iStatus.Int();

	// operation isn't cancelled
	if	(error == KErrNone)
		{
		// task : Externalize
		if (iFlags.IsSet(ERequestExternalize))
			{
			// Try to Externalize
			error = Externalize();

			if (error < KErrNone)
				{
				++iNumberOfAttemptedRetries;

				// check retries, and reschedule
				if (iNumberOfAttemptedRetries == KNumberOfAttemptsAtExternalizingFile)
					{
					// Try one last time. Wait for 10 mins and then attempt operation again.
					// After that, give up.
					//
					// Possible reasons for entering this state:
					//
					// Low disk space, low memory, corrupt file system, etc etc. Don't think
					// its acceptable to panic - operation should be failed gracefully. 
					After(KDelayInMicrosecondsBetweenFileOperationsLongDelay);
					}
				else if (iNumberOfAttemptedRetries < KNumberOfAttemptsAtExternalizingFile)
					{
					// Try again after the delay
					After(KDelayInMicrosecondsBetweenFileOperations);
					}
				else
					{
					// Tried max number of times already. Give up.
					iFlags.Clear(ERequestExternalize);
					}
				}

			}
		}
	}


//*************************************************************************************

#ifndef __WINC__
void CASSrvAlarmStore::ScheduleExternalizeWithRetry(TUint aTimeMS)
#else
void CASSrvAlarmStore::ScheduleExternalizeWithRetry(TUint /*aTimeMS*/)
#endif
	{
	// make sure change isn't due to Internalize
	if (iFlags.IsClear(EIsInternalizing))
		{
		DEBUG_PRINT2(_L("*** Schedule Externalize - IsActive() = %u"), IsActive());

		iFlags.Set(ERequestExternalize);
		iNumberOfAttemptedRetries = 0;

#ifndef __WINC__
		// set the time to RunL iff not already set to run
		//   check that no Backup or Restore is in progress, 
		if (!iBackupNotification->BackupInProgress() && 
			!iBackupNotification->RestoreInProgress()&& !IsActive())
			{
			// schedule the actual Externalize 
			After(aTimeMS * 1000);
			}
#endif
		}
	}


//*************************************************************************************
/**
 * @see CActive
 */
TInt CASSrvAlarmStore::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}


//*************************************************************************************
#ifndef __WINC__

/**
 * @see MBackupRestoreNotificationObserver
 */

void CASSrvAlarmStore::BackupBeginningL()
	{
	DEBUG_PRINT1(_L("-> Alarm Store => Backup Beginning"));
	StartOfBackupOrRestoreL(CASSrvAlarmQueue::EStoreBackup);
	}

void CASSrvAlarmStore::BackupCompletedL()
	{
	// End of Backup
	DEBUG_PRINT1(_L("-> Alarm Store => Backup Ended"));

	// Backup server was reading from the alarm file so
	// tell alarm queue that Backup is complete
	ServerWideData().Queue().EndAlarmStoreOperation(KErrNone);

	// In case Store has been waiting for backup to finish. Now
	// attempt to perform deferred operation.
	RetryStoreOperation();
	}

void CASSrvAlarmStore::RestoreBeginningL()
	{
	// Restore is starting
	DEBUG_PRINT1(_L("-> Alarm Store => Restore Beginning"));
	StartOfBackupOrRestoreL(CASSrvAlarmQueue::EStoreRestore);
}

void CASSrvAlarmStore::RestoreCompletedL(TInt aRestoreResult)
	{
	DEBUG_PRINT1(_L("-> Alarm Store => Restore Ended"));

	// If the Backup server successfully wrote to the alarm file, then we should 
	// read its contents and restore all alarm data based upon it.
	if (aRestoreResult == KErrNone)
		{
		DEBUG_PRINT1(_L("-- End of Restore ... EEnd"));
		// Any error during Internalize must be pretty severe so catch it and leave.
		// NB Internalize calls ServerWideData().Queue().EndAlarmStoreOperation();
		User::LeaveIfError(Internalize(CASSrvAlarmQueue::EStoreInternalizeRestore));
		}
	else
		{
		DEBUG_PRINT1(_L("-- end of Restore ... EAbort"));
	
        ServerWideData().Queue().EndAlarmStoreOperation(KErrGeneral);
        
		// we don't know what state the AlarmServer.Ini file is in
		// so want an Externalize() to happen soon
		ScheduleExternalizeWithRetry(KDelayPeriodBeforeAttemptingAnExternalize);
		}
	}

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
void CASSrvAlarmStore::MHandleSystemStateChange(TState aState)
	{
	if (aState == EShutdown)
		{
		ExternalizeIfRequested();
		}
	}
#endif

void CASSrvAlarmStore::StartOfBackupOrRestoreL(CASSrvAlarmQueue::TStoreOperation aStoreOperation)
	{
	// Flush any 'scheduled' Externalize before Restore
	ExternalizeIfRequested();

	// tell Alarm Queue what type of Store is wanted
	User::LeaveIfError(ServerWideData().Queue().StartAlarmStoreOperation(aStoreOperation));
	}
#endif // #ifndef __WINC__
