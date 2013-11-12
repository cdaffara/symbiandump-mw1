// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// aplappregfinder.cpp
//

#include "aplappregfinder.h"
#include "APFDEF.H"
#include "../apfile/APFSTD.H"
#include "../apserv/apsserv.h"				// class CApaAppListServer
#include "../apgrfx/apprivate.h"
#include <apsidchecker.h>
#include "aplapplistitem.h"					// class TApaAppEntry
#include "aplappinforeader.h"				// class ApaUtils


GLDEF_C void Panic(TApfPanic aPanic)
	{
	_LIT(KApFilePanic,"APFILE");
	User::Panic(KApFilePanic,aPanic);
	}


//
// CApaAppRegFinder
//

EXPORT_C CApaAppRegFinder* CApaAppRegFinder::NewL(const RFs& aFs)
	{
	CApaAppRegFinder* self=NewLC(aFs);
	CleanupStack::Pop(self);
	return self;
	}
	
EXPORT_C CApaAppRegFinder* CApaAppRegFinder::NewLC(const RFs& aFs)
	{
	CApaAppRegFinder* self=new (ELeave) CApaAppRegFinder(aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	
CApaAppRegFinder::CApaAppRegFinder(const RFs& aFs)
	: iFs(aFs),
	iSidCheckerMap(CApaAppArcServer::Self()?CApaAppArcServer::Self()->RescanCallBack():TCallBack(NULL,NULL))
	{
	}

CApaAppRegFinder::~CApaAppRegFinder()
	{
	iListOfDrives.Close();
	iSidCheckerMap.Close();
	delete iFileList; // should already be deleted and NULL at this point
	}

void CApaAppRegFinder::ConstructL()
	{
	}


// Build a list of currently available drives
void CApaAppRegFinder::RebuildDriveListL(TScanScope aScopeOfScan)
	{
	iListOfDrives.Reset();
	TDriveList driveList;
	User::LeaveIfError(iFs.DriveList(driveList));

	// Scan through all the drives in the drive list from Y to A, querying the 
	// file server about wheather the drives are pressent or not

	TInt drive(EDriveY);
	TDriveUnitInfo driveUnitInfo;
	while(drive >= EDriveA)
		{
		// If the drive exists in the file system...
		if (driveList[drive]!=KDriveAbsent)
			{
			TDriveInfo driveInfo;
			const TInt err = iFs.Drive(driveInfo, drive);
			if (!err)
				{
				if (aScopeOfScan == EScanRemovableDrives)
					{// iListOfDrives should have list of all the removable drives irrespective of presence of the media
					if ((driveInfo.iType != EMediaRemote) && (driveInfo.iDriveAtt & KDriveAttRemovable))
						{
						driveUnitInfo.iUnit=drive;
						driveUnitInfo.iAttribs=driveInfo.iDriveAtt;
						iListOfDrives.AppendL(driveUnitInfo);
						}
					}
				else // scan all drives
					{
					if (driveInfo.iType!=EMediaNotPresent && driveInfo.iType!=EMediaRemote)
						{
						driveUnitInfo.iUnit=drive;
						driveUnitInfo.iAttribs=driveInfo.iDriveAtt;
						iListOfDrives.AppendL(driveUnitInfo);
						}
					}
				}
			}
			
		--drive;
		}

	// Add the Z-drive to the list if scanning all drives (not only removable)
	if (aScopeOfScan == EScanAllDrives)
		{
		driveUnitInfo.iUnit=EDriveZ;
		driveUnitInfo.iAttribs=KDriveAttRom;
		iListOfDrives.AppendL(driveUnitInfo);
		}
	}

EXPORT_C TArray<const TDriveUnitInfo> CApaAppRegFinder::DriveList() const
	{
	return iListOfDrives.Array();
	}

EXPORT_C void CApaAppRegFinder::FindAllAppsL(TScanScope aScopeOfScan)
	{
	delete iFileList;
	iFileList = NULL;
	RebuildDriveListL(aScopeOfScan);
	iCurrentIndexInDriveList=-1;
	iScanStage = EScanStageNonImportROM;
	iLastChkedApp = KNullUid;	
	}

/** Scans the applications in following order:
1. non-import y-a,z but only on ROM drives
2. import on all non-ROM drives in the order y-a,z
3. import on all ROM drives in the order y-a,z

Upgrades of apparc server (via Software Install) are currently disallowed. 
This means it is not possible to install registration files to \private\10003a3f\apps 
(on any drive), so there is no need to scan \private\10003a3f\apps on non ROM based drives.
*/
EXPORT_C TBool CApaAppRegFinder::NextL(TApaAppEntry& aAppRegistrationEntry, const CDesCArray& aForcedRegistrations)
	{
	__ASSERT_ALWAYS(iListOfDrives.Count(),Panic(EPanicFindAllAppsNotCalled));

	FOREVER	// Continue until we have looked at all apps on all drives
		{
		TDriveUnitInfo currentDrive;
		TPtrC appFolderOnDrive;
		
		if (!iFileList)	// If the file list has been exhausted...
			{
			iFileIndex = 0;
			
			// Look at the next drive until we find one with applications on it, or we run out of drives.
			while(NextDriveAndFolderInNormalizedOrder(currentDrive, appFolderOnDrive))	
				{
				const TInt err = GetAppListOfDriveL(currentDrive.iUnit, appFolderOnDrive, iFileList);	// ...and create a new list for the next drive.
				if(!err)	
					break;	// Found a drive with applications on it
				}
				
			if (!iFileList)		// If we've run out of drives
				return EFalse; 	// ...then signal this by returning false.
			}
		else	// Still apps in the current list. Recall the current drive and path.
			GetCurrentDriveAndFolder(currentDrive, appFolderOnDrive);

		// Scan the list of all app files for application reg-files
		while (iFileIndex < iFileList->Count())
			{
			const TEntry& entry=(*iFileList)[iFileIndex++];
			if (!ApaUtils::TypeUidIsForRegistrationFile(entry.iType))
				continue; // Only interested in app registration resource files
				
			// File is application reg-file
			
			// Build a parse-object for the full ref-file name
			TParse regFileNameParser;
			const TDriveName driveName = currentDrive.iUnit.Name();
			regFileNameParser.Set(entry.iName, &appFolderOnDrive, &driveName);

			// Apparc will call sidchecker to verify if an application is a valid registered application. 
			// Apparc will call sidchecker in the following conditions
			// 1. If the current drive is a removable drive
			// 2. If the current drive is not 
			//		a) an internal read-only drive
			// 		b) the system drive
			if(currentDrive.iAttribs&KDriveAttRemovable || ((currentDrive.iUnit != iFs.GetSystemDrive()) && (currentDrive.iAttribs&KDriveAttInternal) && !(currentDrive.iAttribs&KDriveAttRom)))
				{
				// ...then verify that there is a valid Secure ID (SID) for the appliation
				// to protect against untrusted applications.
				if (entry[2] != iLastChkedApp) //Check for validity only if it has not yet been checked
					{
					const TUid appTypeUid = 
					(entry[0].iUid == KUidPrefixedNonNativeRegistrationResourceFile ? entry[1] : KNullUid);
				
					// Get the CAppSidChecker for this type of executable
					TBool validRegistration = ETrue;
					TRAPD(err, validRegistration = iSidCheckerMap.FindSidCheckerL(appTypeUid).AppRegisteredAt(entry[2], currentDrive.iUnit));
					iLastChkedApp = entry[2];
					if(!err)
						{
						if(!validRegistration)	// If not trusted...
							{
							// Check if this registration file should be included despite 
							// not being officially reported as a valid registration
							const TPtrC fullName = regFileNameParser.FullName();
							TInt ignorePos = KErrNotFound;
							if (aForcedRegistrations.FindIsq(fullName, ignorePos, ECmpFolded) != 0)	// If not found
								continue;	// App is not trusted. Move on to the next!
							}
						// App is trusted! Fall through.
						}
					else if(err != KErrNotFound)
						User::LeaveIfError(err);
					else
						continue; // Can't tell. Move on to next.
					}
				else
					{
					continue;
					}
				}
	
			// App was found to be trustworthy	
			aAppRegistrationEntry.iUidType = entry.iType;
			aAppRegistrationEntry.iFullName = regFileNameParser.FullName();
			return ETrue;	// Break out of the loop!
			}
			
		// Current drive has been exhausted...
		delete iFileList;
		iFileList = NULL;
		} // ...loop and check the next drive for apps.
	}
	
void CApaAppRegFinder::GetCurrentDriveAndFolder(TDriveUnitInfo& aDrive, TPtrC& aAppFolderOnDrive) const
	{
	ASSERT(iCurrentIndexInDriveList != KErrNotFound && iCurrentIndexInDriveList < iListOfDrives.Count());
	aDrive = iListOfDrives[iCurrentIndexInDriveList];
	aAppFolderOnDrive.Set(iFolderOnCurrentDrive);
	}
	
TBool CApaAppRegFinder::NextDriveAndFolderInNormalizedOrder(TDriveUnitInfo& aNextDrive, TPtrC& aAppFolderOnDrive) const
	{
_LIT(KAppRegRscSearchPath,"\\private\\10003a3f\\apps\\");
_LIT(KAppRegRscImportSearchPath,"\\private\\10003a3f\\import\\apps\\");
_LIT(KAppRegRscImportNonNativeSearchPath,"\\private\\10003a3f\\import\\apps\\NonNative\\Resource\\");

	// Updates iCurrentIndexInDriveList
	// 
	TBool foundAnotherDrive = ETrue;
	FOREVER
		{
		iCurrentIndexInDriveList++;	// move to next folder
		if (iCurrentIndexInDriveList >= iListOfDrives.Count())	// If we reached the end if the drive list...
			{
			ASSERT(iScanStage < EScanStageComplete);
			iCurrentIndexInDriveList = 0;		// ...start over from the beginning of the drive list
			iScanStage++;						// ...and start looking for drives of the next type
			}
		
		// Get next drive's info
		const TDriveUnitInfo driveUnitInfo = iListOfDrives[iCurrentIndexInDriveList];
		
		// Check if this drive is of the correct type for this next stage
		switch (iScanStage)
			{
			case EScanStageNonImportROM:
			// Stage 1: Look in the non-import folder on ROM drives
				if (driveUnitInfo.iAttribs==KDriveAttRom)
					{
					aAppFolderOnDrive.Set(KAppRegRscSearchPath);
					goto done;	// Break out of the loop to return the drive found
					}
				break;	// Break and move on to the next drive
			case EScanStageImportNonROM:	
			// Stage 2: Look in the import folder for native apps on non-ROM drives
				if (driveUnitInfo.iAttribs!=KDriveAttRom)
					{
					aAppFolderOnDrive.Set(KAppRegRscImportSearchPath);
					goto done;	// Break out of the loop to return the drive found
					}
				break; // Break and move on to the next drive
			case EScanStageImportNonNativeResourceNonROM:
			// Stage 3: Look in the import folder for non-native apps on non-ROM drives
				if (driveUnitInfo.iAttribs!=KDriveAttRom)
					{
					aAppFolderOnDrive.Set(KAppRegRscImportNonNativeSearchPath);
					goto done;	// Break out of the loop to return the drive found
					}
				break; // Break and move on to the next drive
			case EScanStageImportROM:
			// Stage 4: Look in import folder for native apps on ROM drives
				if (driveUnitInfo.iAttribs==KDriveAttRom)
					{
					aAppFolderOnDrive.Set(KAppRegRscImportSearchPath);
					goto done;	// Break out of the loop to return the drive found
					}
				break; // Break and move on to the next drive
			case EScanStageImportNonNativeResourceROM:
			// Stage 5: Look in the import folder for non-native apps on ROM drives
				if (driveUnitInfo.iAttribs==KDriveAttRom)
					{
					aAppFolderOnDrive.Set(KAppRegRscImportNonNativeSearchPath);
					goto done;	// Break out of the loop to return the drive found
					}
				break; // Break and move on to the next drive
			case EScanStageComplete:
			// Stage 6: All done!
				aAppFolderOnDrive.Set(KNullDesC);
				foundAnotherDrive = EFalse;
				goto done;
			default:
				ASSERT(0);
				break;	
			}
		}

done:		
	aNextDrive = (foundAnotherDrive) ? iListOfDrives[iCurrentIndexInDriveList] : TDriveUnitInfo();
	iFolderOnCurrentDrive.Set(aAppFolderOnDrive);	// Remember the folder path for GetCurrentDriveAndFolder()
	return foundAnotherDrive;
	}	
	
_LIT(KAppRegRscSearchAnyFile,"*");

TInt CApaAppRegFinder::GetAppListOfDriveL(TDriveUnit aDriveUnit, const TDesC& aPath, CDir*& aFileList) const
	{
	const TDriveName driveName = aDriveUnit.Name();
	TParse regFileNameParser;
	TInt error = regFileNameParser.Set(KAppRegRscSearchAnyFile, &aPath, &driveName);
	User::LeaveIfError(error);
	
	ASSERT(!aFileList);
	error = iFs.GetDir(regFileNameParser.FullName(), KEntryAttAllowUid, ESortNone, aFileList);
	LeaveIfIrregularErrorL(error);
	return error;
	}

void CApaAppRegFinder::LeaveIfIrregularErrorL(TInt aError)	// e.g. KErrNoMemory
// static
	{
	if (aError!=KErrNone && aError!=KErrNotFound && aError!=KErrPathNotFound && aError!=KErrNotReady
		&& aError!=KErrDisMounted && aError!=KErrCorrupt && aError!=KErrNotSupported && 
		aError!=KErrBadName && aError!=KErrLocked)
		{
		User::Leave(aError);
		}
	}
