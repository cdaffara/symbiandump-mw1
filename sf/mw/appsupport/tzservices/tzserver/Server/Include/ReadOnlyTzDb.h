// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __READONLYTZDB_H__
#define __READONLYTZDB_H__

#include <f32file.h>
#include "MBackupRestoreNotificationObserver.h"
#include "mtimezonedataaccess.h"
#include "tzdbentities.h"
#include "tzswiobserver.h"

class CTzDbChangeNotifier;
class CVTzRules;

//
// CReadOnlyTzDb: class to manage the Read-Only TimeZone Database
//
class CReadOnlyTzDb : public CBase, public MTzDataObserver
	{
public:
	static CReadOnlyTzDb* NewL(RFs& aFs, CTzSwiObserver& aSwiObserver, MTzDataObserver& aTzDataObserver);
	~CReadOnlyTzDb();
	void ReleaseData();	// releases the data without destroying the object 
						// (so that it can keep watching for changes to the database)

	//
	// Public methods to extract information from the database
	// 
	void GetTzRulesL(CTzRules& aRules, CTzId& aTzId);
	CBufFlat* GetTzRuleDesignationL(CTzId& aTzId, const TDateTime& aDateTime, TTzTimeReference aTimeRef);
	CTzId* GetDefaultTimeZoneIdL();
	TInt GetOffsetForTimeZoneIdL(const TTime& aTime, TInt aTzId);
	TBool IsIdInDbL(TUint aTzId);
	
public:
	// methods to handle backup-restore
	void BackupBeginningL();
	void BackupCompletedL();
	void RestoreBeginningL();
	void RestoreCompletedL();
	TInt StartAddress() const;

	CTzDbZone* GetZoneL(TUint aReference) const;
	CTzDbRuleSet* GetRuleSetL(TUint aReference) const;

	const TTzZone& GetTZone(TUint aReference) const;
	const TTzRegionalZoneIndex& GetTRegionalZoneIndex(TUint aReference) const;
	const TTzStdTimeAlignment& GetTStdTimeAlignment(TUint aReference) const;
	const TTzRuleUse& GetTRuleUse(TUint aReference) const;

	TPtrC8 GetStringL(TUint aOffset) const;
	const TTzRuleDefinition& GetTRuleDefinition(TUint aOffset) const;

	// inherited from MTzDataObserver
	void NotifyTZDataStatusChangeL(RTz::TTzChanges);
	
private:
	CReadOnlyTzDb(RFs& aFs, CTzSwiObserver& aSwiObserver, MTzDataObserver& aTzDataObserver);
	void ConstructL();
	void InitialiseL();
	
	TInt CopyDatabaseToRam(const TDesC &aTzDbFileName);
	TInt InvalidateFlashDatabaseL();
	void DoGetTzRulesL(CTzRules& aRules, CTzId& aTzId);
	
	CTzDbZone* FindZoneL(CTzId& aTzId, TBool aFillInLocationIDs);
	CTzDbZone* FindZoneByNameL(const TDesC8& aName);
	CTzDbZone* FindZoneByNumericIdL(TUint aNumericId);
	CTzDbStdTimeAlignment* FindStdTimeAlignmentL(const TDesC8& aZone, const TDateTime& aDateTime);

	void HandleDatabaseChangeL(RTz::TTzChanges aChange);

	inline void SetRestoreInProcess();
	inline void ResetRestoreInProcess();

private:
	RFs& iFs;
	CTzSwiObserver& iSwiObserver;

	// detect and notify changes in the flash-based TZ Database
	MTzDataObserver& iTzDataObserver;
	
	// DB entity tables (will be created when the object is constructed)
	CTzDbStringsTable*				iStringsTable;
	CTzDbRegionsTable*				iRegionsTable;
	CTzDbRegionalZonesTable*		iRegionalZonesTable;
	CTzDbZonesTable*				iZonesTable;
	CTzDbLinksTable*				iLinksTable;
	CTzDbStdTimeAlignmentsTable*	iStdTimeAlignmentsTable;
	CTzDbRuleSetsTable*				iRuleSetsTable;
	CTzDbRuleUsesTable*				iRuleUsesTable;
	CTzDbRuleDefinitionsTable*		iRuleDefinitionsTable;

	// Start address of RAM copy of TZ Database file
	TInt	iStartAddress;
	// RAM copy of TZ Database file
	TUint8*	iTzDbData;
	RChunk	iChunk;
	// If true, RAM copy of TZ Database file was copied from Flash (C: drive)
	// otherwise was copied from ROM (Z: drive).
	TBool iTzDbDataFromFlash;

	TBool	iRestoreInProcess;
	};

inline void CReadOnlyTzDb::SetRestoreInProcess()	
	{ 
	iRestoreInProcess = ETrue;	
	}
 
inline void CReadOnlyTzDb::ResetRestoreInProcess()	
	{ 
	iRestoreInProcess = EFalse;	
	} 

#endif // __READONLYTZDB_H__
