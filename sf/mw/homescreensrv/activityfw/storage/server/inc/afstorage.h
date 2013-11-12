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
#ifndef AFSTORAGE_H
#define AFSTORAGE_H

#define __E32SVR_H__

#include <e32std.h>
#include <e32base.h>
#include <d32dbms.h>    // RDbStoreDatabase
#include <s32file.h>    // CFileStore & CPermanentFileStore
#include "afentry.h"

class CAfDatabaseCleaner;

class CAfStorage : public CBase
    {
public:
    ~CAfStorage();

    static CAfStorage* NewL(RFs& aSession);

    void SaveActivityL(CAfEntry &aEntry);

    void DeleteActivityL(CAfEntry &aEntry);

    void DeleteActivitiesL(CAfEntry &aEntry);

    void AllActivitiesL(RPointerArray<CAfEntry> &aDst, TInt aLimit);

    void ActivitiesL(RPointerArray<CAfEntry> &aDst, TInt aAppId);
    
    void ActivityL(RPointerArray<CAfEntry> &aDst, CAfEntry &aSrc);

    RFs& Fs();

    static void ThumbnailPathL(RBuf &aDst, 
                               RFs& aFileSystem, 
                               TInt aUid, 
                               const TDesC &aActivityName,
                               TBool aPersistent);

    static void StoragePathL(RBuf &aDst, 
                             RFs& aFileSystem, 
                             TBool aPersistent);

    static void AppStoragePathL(RBuf &aDst, 
                                RFs& aFileSystem,
                                TInt aUid,
                                TBool aPersistent);
private:
    static HBufC8* Md5HexDigestL(const TDesC8 &aString);
    
private:
    CAfStorage(RFs& aSession);

    void ConstructL();

    void CreateDbL(const TDesC& aDatabaseFile);

    void OpenDbL(const TDesC& aDatabaseFile);

    void VerifyDbL();

    CDbColSet* ExpectedTableLC();

    void CreateTableL();

    void VerifyTableL();

    void DeleteNonPersistentActivitiesL();

    void GetActivitiesL(const TDesC& aDst);

    HBufC* SelectRowLC(TInt aAppId, const TDesC& aActId) const;

    HBufC* SelectRowsLC(TInt aAppId) const;

    HBufC* DeleteRowLC(TInt aAppId, const TDesC& aActId) const;
    
    HBufC* DeleteRowsLC(TInt aAppId) const;

    HBufC* BuildQueryLC(const TDesC& aFormat, TInt aAppId, const TDesC& aActId) const;
    
    void ActivitiesL(RPointerArray<CAfEntry>& aDst, 
                     const TDesC& aQuery, 
                     CAfEntry::AccessRights aRights,
                     TInt aLimit = 0,
                     TBool deserializeAllData = EFalse);

    void ActivitiesL(RPointerArray<CAfEntry>& aDst, 
                     RDbView& aQuery, 
                     CAfEntry::AccessRights aRights,
                     TInt aLimit = 0,
                     TBool deserializeAllData = EFalse);

    void GetActivityForUpdateL(RDbView& aQuery, TInt aAppId, const TDesC& aActId);

    void ReadDataL(RBuf& aDst, RDbRowSet& aSrc, TInt aOffset) const;

    void ExternalizeDataL(RDbRowSet &aDst,const CAfEntry & aSrc, TInt aOffset) const;

    void InternalizeDataL(CAfEntry &aDst, RDbRowSet& aSrc, TInt aOffset) const;

public:
    TBool InterruptCleanup();
    void RequestCleanup();

private:
    RFs& iFsSession;
    RDbStoreDatabase iActDb;/** For database operations*/
    CFileStore* iFileStore;  /** For creating and opening database files */
    CAfDatabaseCleaner *iDatabaseCleaner;
};

#endif //AFSTORAGE_H
