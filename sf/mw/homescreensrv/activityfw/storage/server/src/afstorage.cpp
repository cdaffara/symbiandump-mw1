/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "afstorage.h"
#include <bautils.h>
#include <hash.h>
#include <s32mem.h>

#include "afdatabasecleaner.h"
#include "afqueries.h"
#include "afentry.h"

_LIT(KDbName, "activity.db");
_LIT(KDbDrive, "c:");
const TInt KMaxPathLength = 256;

_LIT(KNonPersistent, "non_persistent\\");
_LIT(KPersistent, "persistent\\");
_LIT(KUidFormat, "%+08x\\");
_LIT(KExtFormat, ".mbm");

// -----------------------------------------------------------------------------
/**
 * Constructor for performing 1st stage construction
 * @param session - initialized session to file system
 */
CAfStorage::CAfStorage(RFs& session)
:
iFsSession(session)
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
/**
 * Destructor.
 */
CAfStorage::~CAfStorage()
    {
    delete iDatabaseCleaner;
    iActDb.Close();
    delete iFileStore;
    }

// -----------------------------------------------------------------------------
/**
 * Two-phased constructor.
 * @param session - initialized session to file system
 */
CAfStorage* CAfStorage::NewL(RFs& session)
    {
    CAfStorage* self = new (ELeave) CAfStorage(session);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
/**
 * EPOC default constructor for performing 2nd stage construction
 */
void CAfStorage::ConstructL()
    {
    RBuf path;
    CleanupClosePushL( path );
    path.CreateL(KMaxPathLength);
    User::LeaveIfError(iFsSession.PrivatePath(path ));
    path.Append(KDbName);
    path.Insert(0, KDbDrive);
    BaflUtils::EnsurePathExistsL(iFsSession, path);
    BaflUtils::FileExists(iFsSession, path) ? OpenDbL(path) : CreateDbL(path);
    CleanupStack::PopAndDestroy(&path);
    
    iDatabaseCleaner = new (ELeave) CAfDatabaseCleaner(iActDb);
    
    DeleteNonPersistentActivitiesL();
    RequestCleanup();
    }

// -----------------------------------------------------------------------------
/**
 * Create database and its structure
 * @param databaseFile - database file path
 */
void CAfStorage::CreateDbL(const TDesC& databaseFile)
    {
    iFileStore = CPermanentFileStore::ReplaceL(iFsSession,
                                               databaseFile,
                                               EFileRead|EFileWrite);
    iFileStore->SetTypeL(iFileStore->Layout());// Set file store type
    TStreamId id = iActDb.CreateL(iFileStore);// Create stream object
    iFileStore->SetRootL(id);// Keep database id as root of store
    iFileStore->CommitL();// Complete creation by commiting
    CreateTableL();
    }

// -----------------------------------------------------------------------------
/**
 * Open database
 * @param databaseFile - database file path
 */
void CAfStorage::OpenDbL(const TDesC& databaseFile)
    {
    TRAPD( errNo,
           iFileStore = CPermanentFileStore::OpenL( iFsSession,
                                                    databaseFile,
                                                    EFileRead|EFileWrite );
           iFileStore->SetTypeL( iFileStore->Layout() ); /* Set file store type*/
           iActDb.OpenL( iFileStore, iFileStore->Root() );
           VerifyTableL(); )
    if( KErrNone != errNo )
        {
        //database is corrupted. recreate
        iActDb.Close();
        delete iFileStore;
        iFileStore = 0;
        CreateDbL( databaseFile );
        }
    }

// -----------------------------------------------------------------------------
/**
 * Verify database structure
 */
void CAfStorage::VerifyDbL()
{
    TInt errNo(KErrCorrupt);
    CDbTableNames* tables = iActDb.TableNamesL();
    CleanupStack::PushL( tables );
    for( TInt iter(0); iter < tables->Count() && KErrNone != errNo; ++iter )
        {
        if( 0 == (*tables)[iter].Compare( KActivityTableName() ) )
            {
            VerifyTableL();
            errNo = KErrNone;
            }
        }
    CleanupStack::PopAndDestroy( tables );
    User::LeaveIfError( errNo );
}

// -----------------------------------------------------------------------------
CDbColSet* CAfStorage::ExpectedTableLC()
    {
    CDbColSet* actColSet = CDbColSet::NewLC();
    
    TDbCol appName(KApplicationColumnName, EDbColInt64);
    appName.iAttributes = TDbCol::ENotNull;
    actColSet->AddL( appName );
    
    TDbCol actName( KActivityColumnName, EDbColText16 );// Using default length
    actName.iAttributes = TDbCol::ENotNull;
    actColSet->AddL( actName );
    
    // custom name
    actColSet->AddL( TDbCol( KCustomNameColumnName, EDbColText16 ) );
    
    TDbCol actFlags( KFlagsColumnName, EDbColInt32 );
    actFlags.iAttributes = TDbCol::ENotNull;
    actColSet->AddL(actFlags);
    
    TDbCol actTimestamp( KTimestampColumnName, EDbColDateTime );
    actTimestamp.iAttributes = TDbCol::ENotNull;
    actColSet->AddL(actTimestamp);
    
    actColSet->AddL( TDbCol( KDataColumnName, EDbColLongBinary ) );// Stream Data
    
    return actColSet;
    }

// -----------------------------------------------------------------------------
/**
 * Create database structure
 */
void CAfStorage::CreateTableL()
    {
    CDbColSet* actColSet(ExpectedTableLC());
    // Create the table
    User::LeaveIfError(iActDb.CreateTable(KActivityTableName,
                                         *actColSet));
    CleanupStack::PopAndDestroy(actColSet);
    }

// -----------------------------------------------------------------------------
/**
 * Verify table structure
 */
void CAfStorage::VerifyTableL()
    {
    CDbColSet *currentTable(iActDb.ColSetL(KActivityTableName));
    CleanupStack::PushL(currentTable);
    CDbColSet *expectedTable(ExpectedTableLC());
    for( TInt iter(1); iter <= expectedTable->Count(); ++iter )
        {
        const TDbCol& expectedColumn((*expectedTable)[iter]);
        const TDbCol* currentColumn(currentTable->Col(expectedColumn.iName));
        if( 0 == currentColumn ||
            expectedColumn.iAttributes != currentColumn->iAttributes || 
            expectedColumn.iMaxLength != currentColumn->iMaxLength || 
            expectedColumn.iType != currentColumn->iType )
            {
            User::Leave(KErrCorrupt);
            }
        }
    CleanupStack::PopAndDestroy( expectedTable );
    CleanupStack::PopAndDestroy( currentTable );
    }
// -----------------------------------------------------------------------------
/**
 * Delete non-persistent activities
 */
void CAfStorage::DeleteNonPersistentActivitiesL()
    {
    HBufC *query(BuildQueryLC(KDeleteNonPersistentActivities(), 
                              CAfEntry::Persistent, 
                              KNullDesC));
    User::LeaveIfError(iActDb.Execute(*query));
    RBuf privatePath;
    CleanupClosePushL(privatePath);
    privatePath.CreateL(KMaxPathLength);
    StoragePathL(privatePath, Fs(), FALSE);
    CFileMan *fileMan = CFileMan::NewL(Fs());
    TInt i = fileMan->RmDir(privatePath);
    delete fileMan;
    CleanupStack::PopAndDestroy(&privatePath);
    CleanupStack::PopAndDestroy(query);
    }

// -----------------------------------------------------------------------------
/**
 * Save activity
 * @param entry - activity data
 */
void CAfStorage::SaveActivityL(CAfEntry &entry)
{
    // @todo check if this can be tidied up
    //verify if row already exists
    TInt errNo(KErrNone);
    RDbView view;
    CleanupClosePushL(view);
    TRAP(errNo, GetActivityForUpdateL(view, entry.ApplicationId(), entry.ActivityId()));
    if( KErrNone == errNo )
        {
        // update
        view.UpdateL();
        TRAPD(errNo,
        CDbColSet* colSet = view.ColSetL();
        CleanupStack::PushL(colSet);

        view.SetColL(colSet->ColNo(KFlagsColumnName), entry.Flags());
        view.SetColL(colSet->ColNo(KTimestampColumnName), entry.Timestamp());
        view.SetColL(colSet->ColNo(KCustomNameColumnName), entry.CustomActivityName());
        ExternalizeDataL(view, entry, colSet->ColNo(KDataColumnName));

        view.PutL();
        CleanupStack::PopAndDestroy(colSet);)

        if (KErrNone != errNo) 
            {
            view.Cancel();
            User::Leave(errNo);
            }
        }
    else
        {
        // insert
        
        //write table
        RDbTable table;
        CleanupClosePushL(table);
        User::LeaveIfError(table.Open(iActDb, KActivityTableName, table.EUpdatable));
        CDbColSet *row = table.ColSetL();
        CleanupStack::PushL(row);

        table.InsertL();
        TRAP(errNo,
        table.SetColL(row->ColNo(KApplicationColumnName), TInt64(entry.ApplicationId()));
        table.SetColL(row->ColNo(KActivityColumnName), entry.ActivityId());
        table.SetColL(row->ColNo(KCustomNameColumnName), entry.CustomActivityName());
        table.SetColL(row->ColNo(KFlagsColumnName), entry.Flags());
        table.SetColL(row->ColNo(KTimestampColumnName), entry.Timestamp());
        ExternalizeDataL(table, entry, row->ColNo(KDataColumnName));
        table.PutL();)
        if (KErrNone != errNo) {
            table.Cancel();
            User::Leave(errNo);
        }
        CleanupStack::PopAndDestroy(row);
        CleanupStack::PopAndDestroy(&table);
    }
    CleanupStack::PopAndDestroy(&view);
}

// -----------------------------------------------------------------------------
/**
 * Delete activity
 * @param appId - application id
 * @param actId - activity id
 */
void CAfStorage::DeleteActivityL(CAfEntry& entry)
    {
    HBufC *query(DeleteRowLC(entry.ApplicationId(), entry.ActivityId()));
    User::LeaveIfError(iActDb.Execute(*query));
    CleanupStack::PopAndDestroy(query);
    }

// -----------------------------------------------------------------------------
//
void CAfStorage::DeleteActivitiesL(CAfEntry& entry)
    {
    HBufC *query(DeleteRowsLC(entry.ApplicationId()));
    User::LeaveIfError(iActDb.Execute(*query));
    RBuf privatePath;
    CleanupClosePushL(privatePath);
    privatePath.CreateL(KMaxPathLength);
    AppStoragePathL(privatePath, Fs(), entry.ApplicationId(), FALSE);
    CFileMan *fileMan = CFileMan::NewL(Fs());
    CleanupStack::PushL(fileMan);
    fileMan->RmDir(privatePath);
    privatePath.Zero();
    
    AppStoragePathL(privatePath, Fs(), entry.ApplicationId(), TRUE);
    fileMan->RmDir(privatePath);
    
    CleanupStack::PopAndDestroy(fileMan);
    CleanupStack::PopAndDestroy(&privatePath);
    CleanupStack::PopAndDestroy(query);
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CAfStorage::AllActivitiesL(RPointerArray<CAfEntry>& dst, TInt aLimit)
    {
    ActivitiesL(dst, KSelectRows(), CAfEntry::Public, aLimit);
    }

// -----------------------------------------------------------------------------
/**
 * Serialize application activity into the buffer
 * @param dst - destination buffer
 * @param appId - application id
 */
void CAfStorage::ActivitiesL(RPointerArray<CAfEntry>& dst,TInt appId)
    {
    HBufC *query(SelectRowsLC(appId));
    ActivitiesL(dst, *query, CAfEntry::Private);
    CleanupStack::PopAndDestroy(query);
    }

// -----------------------------------------------------------------------------
/**
 * Serialize application activity into the buffer
 * @param dst - destination entry
 * @param src - condition pattern
 */
void CAfStorage::ActivityL(RPointerArray<CAfEntry> &dst, CAfEntry& src)
    {
    HBufC *query = SelectRowLC(src.ApplicationId(), src.ActivityId());
    ActivitiesL(dst, *query, CAfEntry::Private, 1, ETrue);
    if( 0 >= dst.Count() )
        {
        User::Leave(KErrNotFound);
        }
    CleanupStack::PopAndDestroy(query);
    }

// -----------------------------------------------------------------------------
/**
 * Provide initialized file system session
 * @return file system session
 */
RFs& CAfStorage::Fs()
    {
    return iFsSession;
    }

// -----------------------------------------------------------------------------
/**
 * Format query to select activity row
 * @param appId - application id
 * @param actId - activity id
 * @return formated sql query
 */
HBufC* CAfStorage::SelectRowLC(TInt appId, const TDesC& actId) const
    {
    return BuildQueryLC(KSelectRow(),appId, actId);
    }

// -----------------------------------------------------------------------------
/**
 * Format query to select activities for application
 * @param appId - application id
 * @return formated sql query
 */
HBufC* CAfStorage::SelectRowsLC(TInt appId) const
    {
    return BuildQueryLC(KSelectAppRows(), appId, KNullDesC);
    }

// -----------------------------------------------------------------------------
/**
 * Format query to delete activity
 * @param appId - application id
 * @param actId - activity id
 * @return formated sql query
 */
HBufC* CAfStorage::DeleteRowLC(TInt appId, const TDesC& actId) const
    {
    return BuildQueryLC(KDeleteRow(),appId, actId);
    }

// -----------------------------------------------------------------------------
/**
 * Format query to delete activities for application
 * @param appId - application id
 * @return formated sql query
 */
HBufC* CAfStorage::DeleteRowsLC(TInt appId) const
    {
    return BuildQueryLC(KDeleteRows(),appId, KNullDesC);
    }

// -----------------------------------------------------------------------------
/**
 * Format sql query
 * @format - sql format string
 * @param appId - application id
 * @param actId - activity id
 * @return formated sql query
 */
HBufC* CAfStorage::BuildQueryLC(const TDesC& format,
                                      TInt appId,
                                      const TDesC& actId) const
    {
    TBuf<16> appName;
    appName.AppendNum(appId);
    RBuf actName;
    CleanupClosePushL(actName);
    actName.CreateL(actId.Length());
    actName.Copy(actId);
    HBufC* query = HBufC::NewL(format.Length() +
                               appName.Length() +
                               actName.Length() );
    query->Des().AppendFormat(format, &appName, &actName);
    CleanupStack::PopAndDestroy(&actName);
    CleanupStack::PushL(query);
    return query;
    }

// -----------------------------------------------------------------------------
/**
 * Execute sql query and result serialize into buffer
 * @param dst - destination result buffer
 * @param query - sql activity query
 */
void CAfStorage::ActivitiesL(RPointerArray<CAfEntry>& dst, const TDesC& query, CAfEntry::AccessRights rights, TInt limit, TBool deserializeAllData)
    {
    RDbView view;// Create a view on the database
    CleanupClosePushL(view);
    User::LeaveIfError(view.Prepare(iActDb, TDbQuery(query), view.EReadOnly));
    User::LeaveIfError(view.EvaluateAll());
    ActivitiesL(dst, view, rights, limit, deserializeAllData);
    CleanupStack::PopAndDestroy(&view);
    }

// -----------------------------------------------------------------------------
/**
 * Return view deserialisd into entries array
 * @param dst - destination result
 * @param query - view
 * @param rights - acess rights
 */
void CAfStorage::ActivitiesL(RPointerArray<CAfEntry>& dst, 
                             RDbView& src, 
                             CAfEntry::AccessRights rights, 
                             TInt limit,
                             TBool deserializeAllData)
    {
    CDbColSet* row = src.ColSetL();
    CleanupStack::PushL(row);

    const TInt flagsOffset(row->ColNo(KFlagsColumnName)),
               applicationOffset(row->ColNo(KApplicationColumnName)),
               activityOffset(row->ColNo(KActivityColumnName)),
               customNameOffset(row->ColNo(KCustomNameColumnName)),
               dataOffset(row->ColNo(KDataColumnName)),
               timestampOffset(row->ColNo(KTimestampColumnName));

    RBuf activityName;
    CleanupClosePushL(activityName);

    RBuf customName;
    CleanupClosePushL(customName);
    
    
    for (src.FirstL(); src.AtRow(); src.NextL()) 
        {
        if ( 0 < limit && dst.Count() >= limit )
            {
            break;
            }
        src.GetL();
        ReadDataL(activityName, src, activityOffset);
        ReadDataL(customName, src, customNameOffset);

        CAfEntry *entry = CAfEntry::NewLC(src.ColInt32(flagsOffset),
                                          src.ColInt64(applicationOffset),
                                          activityName,
                                          customName,
                                          KNullDesC,
                                          KNullDesC8,
                                          KNullDesC8,
                                          src.ColTime(timestampOffset));
        if( CAfEntry::Public == rights && 
            (entry->Flags() & CAfEntry::Invisible) )
            {
            CleanupStack::PopAndDestroy(entry);
            continue;
            }
        InternalizeDataL(*entry, src, dataOffset);
        
        if (!deserializeAllData) {
            entry->SetDataL(KNullDesC8(), CAfEntry::Public);
            entry->SetDataL(KNullDesC8(), CAfEntry::Private);
        } else {
            if (CAfEntry::Public == rights) {
                entry->SetDataL(KNullDesC8(), CAfEntry::Private);
            }
        }

        dst.AppendL(entry);
        CleanupStack::Pop(entry);
        }
    CleanupStack::PopAndDestroy(&customName);
    CleanupStack::PopAndDestroy(&activityName);
    CleanupStack::PopAndDestroy(row);
    }

// -----------------------------------------------------------------------------
/**
 * Get activity for update
 * @param query - destination query result
 * @param appId - application id
 * @param actId - activity id
 */
void CAfStorage::GetActivityForUpdateL(RDbView& view, TInt appId, const TDesC& actId)
    {
    HBufC* query(SelectRowLC(appId, actId));
    User::LeaveIfError(view.Prepare(iActDb, TDbQuery(*query), view.EUpdatable));
    CleanupStack::PopAndDestroy(query);
    User::LeaveIfError(view.EvaluateAll());
    if( !view.FirstL() )
        {
        User::Leave(KErrNotFound);
        }
}

// -----------------------------------------------------------------------------
void CAfStorage::ReadDataL(RBuf& dst, RDbRowSet& src, TInt offset) const
    {
    const TInt length(src.ColLength(offset));
    CAfEntry::ReallocL(dst, length);
    RDbColReadStream srcStream;
    srcStream.OpenLC(src,offset);
    srcStream.ReadL(dst, src.ColLength(offset));
    CleanupStack::PopAndDestroy(&srcStream);
    }

// -----------------------------------------------------------------------------
void CAfStorage::ExternalizeDataL(RDbRowSet& dst,const CAfEntry &src, TInt offset) const
    {
    RDbColWriteStream dbStream;
    CleanupClosePushL(dbStream);
    dbStream.OpenL(dst, offset);
    src.ExternalizeDataOnlyL(dbStream);
    CleanupStack::PopAndDestroy(&dbStream);
    }

// -----------------------------------------------------------------------------
void CAfStorage::InternalizeDataL(CAfEntry & dst, RDbRowSet& src, TInt offset) const
    {
    RDbColReadStream dbStream;
    CleanupClosePushL(dbStream);
    dbStream.OpenL(src, offset);
    dst.InternalizeDataOnlyL(dbStream);
    CleanupStack::PopAndDestroy(&dbStream);
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CAfStorage::StoragePathL(RBuf &dst, 
                  RFs& fileSystem, 
                  TBool persistent)
    {
    if (dst.MaxLength() < KMaxPathLength) 
        {
        dst.ReAllocL(KMaxPathLength);
        }
    dst.Zero();
    User::LeaveIfError(fileSystem.PrivatePath(dst));
    if(persistent)
        {
        dst.Append(KPersistent);
        }
    else
        {
        dst.Append(KNonPersistent);
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CAfStorage::AppStoragePathL(RBuf &dst, 
                     RFs& fileSystem,
                     TInt uid,
                     TBool persistent)
    {
    StoragePathL(dst, fileSystem, persistent);
    
    //Format activity path
    dst.AppendFormat( KUidFormat, uid); 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CAfStorage::ThumbnailPathL(RBuf &dst, 
                                RFs& fileSystem, 
                                TInt uid, 
                                const TDesC &activityName,
                                TBool persistent)
{
    RBuf8 buff8;
    CleanupClosePushL(buff8);
    buff8.CreateL(activityName.Length());
    buff8.Copy(activityName);
    HBufC8 *activityHash = Md5HexDigestL(buff8);
    CleanupStack::PopAndDestroy(&buff8);
    CleanupStack::PushL(activityHash);
    
    AppStoragePathL(dst, fileSystem, uid, persistent);
    
    RBuf hash16;
    CleanupClosePushL(hash16);
    hash16.CreateL(KMaxPathLength);
    hash16.Copy(*activityHash);
    dst.Append(hash16);//reuse already allocated buffer to convert 8 -> 16
    CleanupStack::PopAndDestroy(&hash16);
    dst.Append(KExtFormat());
    CleanupStack::PopAndDestroy(activityHash);
       
    BaflUtils::EnsurePathExistsL(fileSystem, dst);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC8* CAfStorage::Md5HexDigestL(const TDesC8 &string)
    {
    _LIT8(KMd5HexFormat, "%+02x");
    CMD5* md5 = CMD5::NewL();
    CleanupStack::PushL(md5);
    
    TPtrC8 hashedSig(md5->Hash(string));
    
    HBufC8* buf = HBufC8::NewL(hashedSig.Length() * 2);
    TPtr8 bufPtr = buf->Des();
    
    for(TInt i(0); i< hashedSig.Length(); ++i) {
        bufPtr.AppendFormat(KMd5HexFormat,hashedSig[i]);
    }
    CleanupStack::PopAndDestroy(md5);
    return buf;
    }

// -----------------------------------------------------------------------------
/**
 * Cancel ongoing cleanup if one is in progress.
 * @return ETrue if the database cleanup was in progress, EFalse otherwise
 */
TBool CAfStorage::InterruptCleanup()
    {
    if( iDatabaseCleaner->IsActive() )
        {
        iDatabaseCleaner->Cancel();
        return ETrue;
        } 
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
/**
 * Start database cleanup
 */
void CAfStorage::RequestCleanup()
    {
    iDatabaseCleaner->StartCleanup();
    }
