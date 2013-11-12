/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFPersistentData class implementation.
*
*/



// INCLUDES
#include <s32file.h>
#include <bautils.h>
#include <connect/sbdefs.h>

#include "cfpersistentdata.h"
#include "cfoperationnode.h"
#include "cftrace.h"
#include "cfpendingpersistencytask.h"
#include "cfcommon.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCFPersistentData::CCFPersistentData
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CCFPersistentData::CCFPersistentData( RFs& aFs,
    const TUid& aOwner,
    const TDesC& aName ):
    CActive( EPriorityStandard ),
	iFs( aFs ),
	iOwner( aOwner ),
    iName( aName )
    {
    FUNC_LOG;

    CActiveScheduler::Add( this ); // Add to scheduler
    }

// ---------------------------------------------------------------------------
// CCFPersistentData::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CCFPersistentData::ConstructL()
    {
    FUNC_LOG;
    
    StartListeningL();
    }

// ---------------------------------------------------------------------------
// CCFPersistentData::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFPersistentData* CCFPersistentData::NewL( RFs& aFs,
    const TUid& aOwner,
    const TDesC& aName )
    {
    FUNC_LOG;

    CCFPersistentData* self = NewLC( aFs, aOwner, aName );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CCFPersistentData::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCFPersistentData* CCFPersistentData::NewLC( RFs& aFs,
    const TUid& aOwner,
    const TDesC& aName )
    {
    FUNC_LOG;

    CCFPersistentData* self = new( ELeave ) CCFPersistentData( aFs,
        aOwner, aName );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// Destructor
CCFPersistentData::~CCFPersistentData()
    {
    FUNC_LOG;
    
    Cancel(); // Cancel any request, if outstanding
    iProperty.Close();
    iPendingTasks.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// CCFPersistentData::DoCancel
// ---------------------------------------------------------------------------
//
void CCFPersistentData::DoCancel()
	{
    iProperty.Cancel();
	}

// ---------------------------------------------------------------------------
// CCFPersistentData::StartListeningL
// ---------------------------------------------------------------------------
//
void CCFPersistentData::StartListeningL()
	{
	Cancel();							// Cancel any request, just to be sure

    // Check the key current value
	iProperty.Attach(KUidSystemCategory, conn::KUidBackupRestoreKey);
	TInt backupStateValue = 0;
	iProperty.Get(backupStateValue);
	TUint backupState = backupStateValue & conn::KBURPartTypeMask;
	if( backupState == conn::EBURNormal || backupState == conn::EBURUnset )
		{
		iStoreRestoreDenied = EFalse;
		}
	else
		{
		iStoreRestoreDenied = ETrue;
		}
	
	iProperty.Subscribe(iStatus);
	SetActive();						// Tell scheduler a request is active
	}

// ---------------------------------------------------------------------------
// CCFPersistentData::RunL
// ---------------------------------------------------------------------------
//
void CCFPersistentData::RunL()
	{
	TInt backupStateValue = 0;
	iProperty.Get(backupStateValue);
	TUint backupState = backupStateValue & conn::KBURPartTypeMask;
	if( backupState == conn::EBURNormal || backupState == conn::EBURUnset )
		{
		if( iStoreRestoreDenied )
			{
			ProcessPendingTasksL();
			}
		iStoreRestoreDenied = EFalse;
		}
	else
		{
		iStoreRestoreDenied = ETrue;
		}
	
	iProperty.Subscribe(iStatus);
	SetActive();						// Tell scheduler a request is active
	}

// ---------------------------------------------------------------------------
// CCFPersistentData::RunError
// ---------------------------------------------------------------------------
//
TInt CCFPersistentData::RunError( TInt aError )
    {
    return aError;
    }

// ---------------------------------------------------------------------------
// CCFPersistentData::ProcessPendingTasksL
// ---------------------------------------------------------------------------
//
void CCFPersistentData::ProcessPendingTasksL()
    {
    FUNC_LOG;
    
    for (TInt i = iPendingTasks.Count()-1; i >= 0; i--)
    	{
    	CCFPendingPersistencyTask* iTask = iPendingTasks[i];
    	if ( iTask->Mode() == CCFPendingPersistencyTask::EStore )
    		{
    		DoStoreL( iTask->FileName(), iTask->Operation()  );
    		}
    	else if ( iTask->Mode() == CCFPendingPersistencyTask::ERestore )
    		{
    		DoRestoreL( iTask->FileName(), iTask->Operation()  );
    		}
    	}
    iPendingTasks.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// CCFPersistentData::RestoreL
// ---------------------------------------------------------------------------
//
void CCFPersistentData::RestoreL( 
    const TDesC& aFile,
    CCFOperationNode& aOperation )
    {
    FUNC_LOG;

    if ( !iStoreRestoreDenied )
    	{
    	DoRestoreL( aFile, aOperation );
    	}
    else
    	{
    	// make store task pending
    	CCFPendingPersistencyTask* task = CCFPendingPersistencyTask::NewLC(
    			CCFPendingPersistencyTask::ERestore, aFile, aOperation );
    	User::LeaveIfError( iPendingTasks.Append( task ) );
    	CleanupStack::Pop( task );
    	}
    }
    
// ---------------------------------------------------------------------------
// CCFPersistentData::DoRestoreL
// ---------------------------------------------------------------------------
//
void CCFPersistentData::DoRestoreL( 
    const TDesC& aFile,
    CCFOperationNode& aOperation )
    {
    FUNC_LOG;

    HBufC* streamName = CreateStreamName( aFile );
    User::LeaveIfNull( streamName );
    CleanupStack::PushL( streamName );
    
    RFileReadStream stream;
    TInt err = stream.Open( iFs, *streamName, EFileShareReadersOnly | EFileRead );
    if ( err == KErrNone )
        {
        stream.PushL();                             // CLEANUP<< stream
        aOperation.InternalizeL( stream );
        stream.Pop();                               // CLEANUP>> stream
        }
    else if ( err != KErrNotFound )
        {
        ERROR(err, "CCFPersistentData::RestoreL() error");
        }
    stream.Close();
    CleanupStack::PopAndDestroy( streamName );      // CLEANUP>> streamName
    }

// ---------------------------------------------------------------------------
// CCFPersistentData::StoreL
// ---------------------------------------------------------------------------
//
void CCFPersistentData::StoreL( 
    const TDesC& aFile,
    CCFOperationNode& aOperation )
    {
    FUNC_LOG;
    
    if ( !iStoreRestoreDenied )
    	{
    	DoStoreL( aFile, aOperation );
    	}
    else
    	{
    	// make store task pending
    	CCFPendingPersistencyTask* task = CCFPendingPersistencyTask::NewLC(
    			CCFPendingPersistencyTask::EStore, aFile, aOperation );
    	User::LeaveIfError( iPendingTasks.Append( task ) );
    	CleanupStack::Pop( task );
    	}
    }

// ---------------------------------------------------------------------------
// CCFPersistentData::StoreL
// ---------------------------------------------------------------------------
//
void CCFPersistentData::DoStoreL( 
    const TDesC& aFile,
    CCFOperationNode& aOperation )
    {
    FUNC_LOG;

    HBufC* streamName = CreateStreamName( aFile );
    User::LeaveIfNull( streamName );
    CleanupStack::PushL( streamName );
    
    BaflUtils::EnsurePathExistsL( iFs, *streamName );
    RFileWriteStream stream;
    TInt err = stream.Replace( iFs,
            *streamName,
            EFileShareExclusive | EFileWrite );
    if ( err == KErrNone )
        {
        stream.PushL();
        aOperation.ExternalizeL( stream );
        stream.Pop();
        }
    else
        {
        ERROR(err, "CCFPersistentData::StoreL() error");
        }
    stream.Close();
    CleanupStack::PopAndDestroy( streamName );
    }

// ---------------------------------------------------------------------------
// CCFPersistentData::Delete
// ---------------------------------------------------------------------------
//
void CCFPersistentData::Delete( const TDesC& aFile )
    {
    FUNC_LOG;
    
    if ( !iStoreRestoreDenied )
    	{
    	DoDelete( aFile );
    	}
    else
    	{
    	ERROR( KErrInUse, "Cannot delete file because of backup or restore!")
    	}
    }

// ---------------------------------------------------------------------------
// CCFPersistentData::DoDelete
// ---------------------------------------------------------------------------
//
void CCFPersistentData::DoDelete( const TDesC& aFile )
    {
    FUNC_LOG;

    HBufC* streamName = CreateStreamName( aFile );
    if( streamName )
        {
        TInt err = BaflUtils::DeleteFile( iFs, *streamName );
        if( err != KErrNone )
            {
            ERROR_1( err, "Failed to delete persistent data [%S]", &aFile );
            }
        delete streamName;
        streamName = NULL;
        }
    else
        {
        ERROR_1( KErrNoMemory, "Failed to delete persistent data [%S]", &aFile );
        }    
    }

// ---------------------------------------------------------------------------
// CCFPersistentData::CreateStreamName
// ---------------------------------------------------------------------------
//
HBufC* CCFPersistentData::CreateStreamName( const TDesC& aFile )
    {
    HBufC* name = HBufC::New( KMaxFileName );
    if( name )
        {
        // Double check that we do not exceed KMaxFileName
        if( aFile.Length() + KPersistenDataFormat().Length() < KMaxFileName )
            {
            // Format name
            TParsePtrC parse( aFile );
            TPtrC persistencyFileName( parse.Name() );
            TPtr namePtr( name->Des() );
            namePtr.Format( KPersistenDataFormat,
                iOwner.iUid, &iName, &persistencyFileName );
            }
        }
    return name;
    }
