// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// aplappregfinder.h
//

#ifndef __APLAPPREGFINDER_H__
#define __APLAPPREGFINDER_H__

#include <f32file.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "apsidcheckermap.h"
#else
#include <apsidchecker.h>
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include <badesca.h>		// typedef CDesCArray

class TDriveUnitInfo;
class TApaAppEntry;

/**
@internalComponent
*/
class CApaAppRegFinder : public CBase
	{
public: // Construction / Destruction
	IMPORT_C static CApaAppRegFinder* NewL(const RFs& aFs);
	IMPORT_C static CApaAppRegFinder* NewLC(const RFs& aFs);
	~CApaAppRegFinder();
private: // Private construction
	CApaAppRegFinder(const RFs& aFs);
	void ConstructL();
public:
	enum TScanScope
		{
		EScanAllDrives,
		EScanRemovableDrives
		};	
		
	IMPORT_C void FindAllAppsL(TScanScope aScopeOfScan); // sets up a scan for all apps
	IMPORT_C TBool NextL(TApaAppEntry& aEntry, const CDesCArray& aForcedRegistrations); // increments a scan for all apps
	IMPORT_C TArray<const TDriveUnitInfo> DriveList() const;
private:		
	void RebuildDriveListL(TScanScope aScopeOfScan);
	TBool NextDriveAndFolderInNormalizedOrder(TDriveUnitInfo& aNextDrive, TPtrC& aAppFolderOnDrive) const;
	void GetCurrentDriveAndFolder(TDriveUnitInfo& aDrive, TPtrC& aAppFolderOnDrive) const;
	TInt GetAppListOfDriveL(TDriveUnit aDriveUnit, const TDesC& aPathName, CDir*& aFileList) const;
	static void LeaveIfIrregularErrorL(TInt aError);
private:
	enum TScanStage
		{
		EScanStageNonImportROM,
		EScanStageImportNonROM, 
		EScanStageImportNonNativeResourceNonROM,
		EScanStageImportROM,
		EScanStageImportNonNativeResourceROM,
		EScanStageComplete
		};
private:
	CDir* iFileList;
	TInt iFileIndex;
	const RFs& iFs;
	RSidCheckerMap iSidCheckerMap;
	RArray<const TDriveUnitInfo> iListOfDrives;
	mutable TInt iCurrentIndexInDriveList;
	mutable TInt iScanStage;
	mutable TPtrC iFolderOnCurrentDrive;
	TUid iLastChkedApp; //Uid of the last app whose sid cheking has been done.
	};
	
NONSHARABLE_CLASS(TDriveUnitInfo)
	{
public:
	TDriveUnit iUnit; 
	TUint iAttribs;
	};
	
#endif // __APLAPPREGFINDER_H__
