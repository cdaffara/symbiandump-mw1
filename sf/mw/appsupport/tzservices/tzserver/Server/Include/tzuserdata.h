// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TZUSERDATA_H__
#define __TZUSERDATA_H__

#include <sqldb.h>
#include <vtzrules.h>

class CTzUserNames;
class RProperty;

enum TTzUserDataChangeOp
    {
    ETzUserDataCreated,  // User defined TZ data has been created.
    ETzUserDataUpdated,  // User defined TZ data has been updated.
    ETzUserDataDeleted,  // User defined TZ data has been deleted.
    ETzUserDataRestored, // User defined TZ data has been restored.
    };

struct TTzUserDataChange
    {
    // Operation that has changed user defined TZ data.
    TTzUserDataChangeOp iOperation;

    // Numeric TZID of the changed user defined TZ data.
    TInt iTzId;
    };

class MTzUserDataChangeObserver
    {
public:
    virtual void NotifyUserTzRulesChange(TTzUserDataChange aChange) = 0;
    virtual void NotifyUserTzNamesChange(TTzUserDataChange aChange) = 0;
    };

class CTzUserDataDb : public CBase
    {
public:
    static CTzUserDataDb* NewL();
 	~CTzUserDataDb();
 	
 	void AddObserverL(MTzUserDataChangeObserver* aChangeObs);
 	void RemoveObserver(MTzUserDataChangeObserver* aChangeObs);
 	
// CRUD operations.
    TInt CreateTzL(const CTzRules& aTzRules, const CTzUserNames& aTzNames);
    void ReadTzRulesL(CTzRules& aRules, TUint aTzId);
    CTzUserNames* ReadTzNamesL(TUint aTzId);
 	void ReadTzIdsL(RArray<TUint32>& aTzIds);
	void UpdateTzL(TUint aTzId, const CTzRules& aTzRules, const CTzUserNames& aTzNames);
    void DeleteTzL(TUint aTzId);
    
// Check if a given user ID exists in the database    
	TBool IsIdInDbL(TUint aTzId);
	
// Backup and restore operations.
    void BackupBeginningL();
    void BackupCompletedL();
    void RestoreBeginningL();
    void RestoreCompletedL();
 private:
    CTzUserDataDb();
    void ConstructL();
	
// Database operations.
    void OpenL();	
    void CreateL();
    void CreateSchemaL();
    void Close();
	
// Transaction operations.
    void BeginTransactionL();
    void CommitTransactionL();
    void RollbackTransaction();
    static void CleanupDatabaseRollback(TAny* aDatabase);

// Notification operations.
    void NotifyTzRulesChange(TUint aTzId, TTzUserDataChangeOp aChangeOp);
    void NotifyTzNamesChange(TUint aTzId, TTzUserDataChangeOp aChangeOp);
    
// Reuse TZ IDs
    TUint32 FindUnusedTzIdL();
     
 private:
 	RSqlDatabase iDatabase;
    TBool iBackupInProgress;
    TBool iRestoreInProgress;
    TUint32 iNextCandidateTzId;  
    RPointerArray<MTzUserDataChangeObserver> iChangeObservers;
    };

#endif // __TZUSERDATA_H__
