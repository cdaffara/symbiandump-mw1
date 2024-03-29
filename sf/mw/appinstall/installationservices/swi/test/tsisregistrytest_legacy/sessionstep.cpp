/*
* Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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


/**
 @file 
 @test
 @internalTechnology
*/

#include <e32svr.h>
#include <test/testexecutelog.h>
#include <test/testexecutestepbase.h>
#include "sessionstep.h"
#include "sisregistrysession.h"
#include "sisregistryentry.h"
#include "sisregistrypackage.h"
#include "installtypes.h"
#include "cleanuputils.h"
#include "hashcontainer.h"
#include "testutilclientswi.h"
#include "arrayutils.h"
#include "sisregistrytoken.h"
#include "sisregistrylog.h"
#include <s32file.h>

#include <centralrepository.h>

/////////////////////////////////////////////////////////////////////
// defs, Constants used by test steps
/////////////////////////////////////////////////////////////////////

_LIT(KNoUid, "nouid");
_LIT(KUid, "uid");
_LIT(KCenRepKey, "cenrepkey");
_LIT(KCenRepValue, "cenrepvalue");
_LIT(KDrivebitmapcount, "drivebitmapcout");
_LIT(KSid, "sid");
_LIT(KPackage, "package");
_LIT(KVendor, "vendor");
_LIT(KController, "ctl");
_LIT(KKeyFormat, "-%02d");
_LIT(KSidFileNameSec, "file");
_LIT(KModFileName, "file");
_LIT(KModTag, "modtag");
_LIT(KHashFileName, "file");
_LIT(KHashFileValue, "hashvalue");
_LIT(KRootCertTag, "rootcert");
_LIT(KResultTag, "expectedresult");
_LIT(KDateTime, "datetime"); //YYYYMMDD:HHMMSS.MMMMMM
_LIT(KPkgCount, "embeddingpkgcount");
_LIT(KLanguage, "language");
_LIT(KRegFileName, "regfilename");
_LIT(KCtlCount, "ctlcount");
_LIT(KHashCount, "hashcount");
_LIT(KIndex, "index");
_LIT(KSisRegistryPath, "\\sys\\install\\sisregistry\\");
_LIT(KCacheBackupFile, "\\sys\\install\\sisregistry\\backup.lst");
_LIT(KCorruptCacheBackupFile, "z:\\tswi\\tsisregistrytest\\data\\hashforlang_txt.dat");
_LIT(KSysBackupFileBackup, "\\sys\\install\\sisregistry\\backup.bak");

/////////////////////////////////////////////////////////////////////
// Tags used by test steps
/////////////////////////////////////////////////////////////////////
using namespace Swi;

/////////////////////////////////////////////////////////////////////
// CSisRegistryStep
/////////////////////////////////////////////////////////////////////
TVerdict CSisRegistrySessionStep::doTestStepPreambleL()
	{
	__UHEAP_MARK;

	// Install an active scheduler
	CActiveScheduler* s = new (ELeave) CActiveScheduler;
	s->Install(s);
	
	INFO_PRINTF1(_L("Connecting to SisRegistry.."));
	User::LeaveIfError(iSisRegistry.Connect());
	INFO_PRINTF1(_L("Connected!"));
	
	return TestStepResult();
	}

TVerdict CSisRegistrySessionStep::doTestStepPostambleL()
	{
	// Remove the installed active scheduler
	CActiveScheduler* s = CActiveScheduler::Current();
	s->Install(NULL);
	delete s;

	iSisRegistry.Close();
	
	__UHEAP_MARKEND;

	return TestStepResult();
	}
/////////////////////////////////////////////////////////////////////
// CEmbeddingPackageStep - Checks the embedding packages for a given package
/////////////////////////////////////////////////////////////////////
CEmbeddingPackageStep::CEmbeddingPackageStep()
	{
	SetTestStepName(KEmbeddingPackageStep);
	}

TVerdict CEmbeddingPackageStep::doTestStepL()
	{
	RSisRegistryEntry entry;
	RPointerArray<Swi::CSisRegistryPackage> packages;
	SetTestStepResult(EPass);
	
	TUid uidOpen;
	TInt embeddingPkgCount;
	if(!GetUidFromConfig(ConfigSection(), KUid, uidOpen) || !GetIntFromConfig(ConfigSection(), KPkgCount, embeddingPkgCount))
		{
		ERR_PRINTF1(_L("Missing package Uid or expected package count"));
		SetTestStepResult(EFail);
		return TestStepResult();
		}
		
	if (KErrNone != entry.Open(iSisRegistry, uidOpen))
		{
		SetTestStepResult(EFail);
	    return TestStepResult();
	    }
	CleanupClosePushL(entry);

	CleanupResetAndDestroyPushL(packages);
	entry.EmbeddingPackagesL(packages);
	
	if(packages.Count() != embeddingPkgCount)
		{
		ERR_PRINTF3(_L("The actual number of embedding packages does not meet expected. Expected: %d Actual: %d"), embeddingPkgCount, packages.Count());
		SetTestStepResult(EFail);
		}
	CleanupStack::PopAndDestroy(2, &entry);
	return TestStepResult();
	}

/////////////////////////////////////////////////////////////////////
// CSidFileStep - Checks the filename to sid mapping
/////////////////////////////////////////////////////////////////////
CSidFileStep::CSidFileStep()
	{
	SetTestStepName(KSidFileName);
	}

TVerdict CSidFileStep::doTestStepL()
	{
	RArray<TUid> theSids;
	CleanupClosePushL(theSids);
	GetUidArrayFromConfigL(ConfigSection(), KSid, theSids);
	
	//RArray<TUid> thePackageUids;
	//CleanupClosePushL(thePackageUids);
	//GetUidArrayFromConfigL(ConfigSection(), KUid, thePackageUids);
	
	RArray<TPtrC> expectedSidNames;
	CleanupClosePushL(expectedSidNames);
	GetStringArrayFromConfigL(ConfigSection(), KSidFileNameSec, expectedSidNames);
	
	SetTestStepResult(EPass);
	
	if (theSids.Count() != expectedSidNames.Count())
		{
		User::LeaveIfError(KErrCorrupt);
		}
	
	TBuf<KMaxFileName> fileName;
	
	for (TInt sidCount(0); sidCount<theSids.Count(); sidCount++)
		{
		iSisRegistry.SidToFileNameL(theSids[sidCount],fileName);
		if (expectedSidNames[sidCount].Compare(fileName) != 0)
			{
			SetTestStepResult(EFail);
			break;
			}
		}
		
	CleanupStack::PopAndDestroy(&expectedSidNames);
	//CleanupStack::PopAndDestroy(&thePackageUids);
	CleanupStack::PopAndDestroy(&theSids);
	return TestStepResult();
	}

////////////////////////////////////////////////////////////////////////
// CModifiableFileStep - Checks whether the file is tagged as modifiable
////////////////////////////////////////////////////////////////////////
CModifiableFileStep::CModifiableFileStep()
	{
	SetTestStepName(KModifiableFile);
	}

TVerdict CModifiableFileStep::doTestStepL()
	{
	RArray<TPtrC> theFileNames;
	CleanupClosePushL(theFileNames);
	GetStringArrayFromConfigL(ConfigSection(), KModFileName, theFileNames);
		
	RArray<TInt> expectedModifiableTags;
	CleanupClosePushL(expectedModifiableTags);
	GetIntArrayFromConfigL(ConfigSection(), KModTag, expectedModifiableTags);
	
	SetTestStepResult(EPass);
	
	if (theFileNames.Count() != expectedModifiableTags.Count())
		{
		User::LeaveIfError(KErrCorrupt);
		}
	
	TInt numModFiles = theFileNames.Count();
	
	for (TInt i=0; i<numModFiles; i++)
		{
		TBool isFileModifiable = iSisRegistry.ModifiableL(theFileNames[i]);
		
		INFO_PRINTF2(_L("File '%d':"), i);
		INFO_PRINTF2(_L(" expected modifiable tag   = %d"), expectedModifiableTags[i]);
		INFO_PRINTF2(_L(" calculated modifiable tag = %d"), isFileModifiable);

		if (expectedModifiableTags[i] != isFileModifiable)
			{
			SetTestStepResult(EFail);
			break;
			}
		}
	
	// Items popped and cleaned up from the stack (in this order):
	// &expectedModifiableTags and &theFileNames.
	CleanupStack::PopAndDestroy(2, &theFileNames);
	
	return TestStepResult();
	}

////////////////////////////////////////////////////////////////////////
// CFileHashStep - Checks the hash of a file
////////////////////////////////////////////////////////////////////////
CFileHashStep::CFileHashStep()
	{
	SetTestStepName(KFileHash);
	}

TVerdict CFileHashStep::doTestStepL()
	{
	// Get the filenames (for which the hash will be calculated)
	// from the test section.
	RArray<TPtrC> theFileNames;
	CleanupClosePushL(theFileNames);
	GetStringArrayFromConfigL(ConfigSection(), KHashFileName, theFileNames);

	// Get the filenames that contain the expected hash values,
	// from the test section.
	RArray<TPtrC> expectedHashFiles;
	CleanupClosePushL(expectedHashFiles);
	GetStringArrayFromConfigL(ConfigSection(), KHashFileValue, expectedHashFiles);
	
	// Initialise the test step result to pass.
	SetTestStepResult(EPass);
	
	// Ensure that there are as many filenames whose hash will be calculated,
	// as there are files with expected hash values.
	TInt numModFiles = theFileNames.Count();
	if (numModFiles != expectedHashFiles.Count())
		{
		User::LeaveIfError(KErrCorrupt);
		}
	
	// For each file in config section KHashFileName, calculate its hash
	// value and compare it with the corresponding expected hash value.
	// Fail the test case if they don't match.
	for (TInt i=0; i<numModFiles; i++)
		{
		INFO_PRINTF2(_L("Using SisRegistry to calculate hash for: %S"), &theFileNames[i]);
		// Use SisRegistry to calculate the hash value for this file.
		CHashContainer* hashContainer = iSisRegistry.HashL(theFileNames[i]);
		CleanupStack::PushL(hashContainer);
		
		const TDesC8& calcHashValue = hashContainer->Data();
		
			// NOTE: The following block of code is temporary,
			//       so we can get the hash values first.
			// -----------------------------------------------
			// Name the file that will contain the expected hash value;
			// on the 2nd iteration, this would have to be substituted
			// by "c:\\system\\data\\hashForLang_txt.dat".
			//_LIT(KHashFile, "c:\\system\\data\\hashForTestsidcheck_A_exe.dat");
								
			// Create and open the (binary) file for writing later.
			//RFs fs_util;
			//User::LeaveIfError(fs_util.Connect());
			//CleanupClosePushL(fs_util);

			//RFile fileToWrite;
			
			//User::LeaveIfError(fileToWrite.Create(fs_util, KHashFile,
			//										EFileWrite | EFileStream));
			//CleanupClosePushL(fileToWrite);
	
			//User::LeaveIfError(fileToWrite.Write(calcHashValue));
			//CleanupStack::PopAndDestroy(&fileToWrite);
			//CleanupStack::PopAndDestroy(&fs_util);
			// -----------------------------------------------

		// Open for reading the file that has the expected hash value.
		INFO_PRINTF2(_L("Reading Expected hash value from: %S"), &expectedHashFiles[i]);
		RFs fs;
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);
		
		RFile fileToRead;
		User::LeaveIfError(fileToRead.Open(fs, expectedHashFiles[i],
										   EFileRead | EFileShareReadersOnly | EFileStream));
		CleanupClosePushL(fileToRead);

		// Create a block of memory to store the expected hash value.
		TInt fileSize = 0;
		User::LeaveIfError(fileToRead.Size(fileSize));
		HBufC8* memForExpectedHashValue = HBufC8::NewLC(fileSize);
	
		// Read the expected hash value into the previously created memory block.
		TPtr8 expectedHashValue(memForExpectedHashValue->Des());
		User::LeaveIfError(fileToRead.Read(0, expectedHashValue, fileSize));

		INFO_PRINTF1(_L("Comparing hash values"));
		
		// Compare the calculated has value against its expected value.	
		if (calcHashValue.Compare(expectedHashValue) != KErrNone)
			{
			SetTestStepResult(EFail);
			
			// Items popped and cleaned up from the stack (in this order):
			// memForExpectedHashValue, &fileToRead, &fs and hashContainer.
			CleanupStack::PopAndDestroy(4, hashContainer);
		
			break;
			}

		// Items popped and cleaned up from the stack (in this order):
		// memForExpectedHashValue, &fileToRead, &fs and hashContainer.
		CleanupStack::PopAndDestroy(4, hashContainer);
		}

	// Items popped and cleaned up from the stack (in this order):
	// &expectedHashFiles and &theFileNames.
	CleanupStack::PopAndDestroy(2, &theFileNames);
	
	return TestStepResult();
	}

/////////////////////////////////////////////////////////////////////
// CInstalledUidsStep - checks installed packages Uids list
/////////////////////////////////////////////////////////////////////
CInstalledUidsStep::CInstalledUidsStep()
	{
	SetTestStepName(KInstalledUids);
	}

TVerdict CInstalledUidsStep::doTestStepL()
	{
	RArray<TUid> expectedUids;
	CleanupClosePushL(expectedUids);
	GetUidArrayFromConfigL(ConfigSection(), KUid, expectedUids);
		
	RArray<TUid> actualUids;
	CleanupClosePushL(actualUids);
	iSisRegistry.InstalledUidsL(actualUids);
	
	SetTestStepResult(EPass);
	
	for (TInt index=0; index < expectedUids.Count(); index++)
		{
		if(actualUids.Find(expectedUids[index]) != KErrNotFound)
			{
			INFO_PRINTF2(_L("Uid: 0x%08x was found!"), expectedUids[index]);
			}
		else
			{
			SetTestStepResult(EFail);
			ERR_PRINTF2(_L("Uid: 0x%08x not found!"), expectedUids[index]);
			}
		}
		
	CleanupStack::PopAndDestroy(2, &expectedUids); //actualUids
	
	return TestStepResult();
	}
	
/////////////////////////////////////////////////////////////////////
// CInstalledPackagesStep - checks installed packages names and vendor list
/////////////////////////////////////////////////////////////////////
CInstalledPackagesStep::CInstalledPackagesStep()
	{
	SetTestStepName(KInstalledPackages);
	}

TVerdict CInstalledPackagesStep::doTestStepL()
	{
	RArray<TUid> expectedUids;
	CleanupClosePushL(expectedUids);
	GetUidArrayFromConfigL(ConfigSection(), KUid, expectedUids);
	
	RArray<TPtrC> expectedPackageNames;
	CleanupClosePushL(expectedPackageNames);
	GetStringArrayFromConfigL(ConfigSection(), KPackage, expectedPackageNames);
	
	RArray<TPtrC> expectedVendorNames;
	CleanupClosePushL(expectedVendorNames);
	GetStringArrayFromConfigL(ConfigSection(), KVendor, expectedVendorNames);
	
	RPointerArray<CSisRegistryPackage> packages;
	CleanupResetAndDestroy<RPointerArray<CSisRegistryPackage> >::PushL(packages);
	
	iSisRegistry.InstalledPackagesL(packages);
	
	SetTestStepResult(EPass);
	
	for (TInt expected = 0; expected < expectedPackageNames.Count(); expected++)
		{
		TBool result = EFalse;
		for (TInt actual = 0; actual < packages.Count(); ++actual)
			{
			if (packages[actual]->Uid() == expectedUids[expected] &&
				!packages[actual]->Name().CompareF(expectedPackageNames[expected]) &&
				!packages[actual]->Vendor().CompareF(expectedVendorNames[expected]))
				{
				result = ETrue;
				break;
				}
			}
			
		if(result)
			{
			INFO_PRINTF3(_L("Package %S by Vendor %S was found!"), &expectedPackageNames[expected], &expectedVendorNames[expected]);
			}
		else
			{
			SetTestStepResult(EFail);
			ERR_PRINTF3(_L("Package %S by Vendor %S was not found!"), &expectedPackageNames[expected], &expectedVendorNames[expected]);
			}
		}
	
	CleanupStack::PopAndDestroy(4, &expectedUids); //packages, expectedVendorNames, expectedPackageNames
	
	return TestStepResult();
	}

/////////////////////////////////////////////////////////////////////
// CRemovablePackagesStep - checks removable packages names and vendor list
/////////////////////////////////////////////////////////////////////
CRemovablePackagesStep::CRemovablePackagesStep()
	{
	SetTestStepName(KRemovablePackages);
	}

TVerdict CRemovablePackagesStep::doTestStepL()
	{
	RArray<TUid> expectedNoUids;
	CleanupClosePushL(expectedNoUids);
	GetUidArrayFromConfigL(ConfigSection(), KNoUid, expectedNoUids);
	
	RArray<TUid> expectedUids;
	CleanupClosePushL(expectedUids);
	GetUidArrayFromConfigL(ConfigSection(), KUid, expectedUids);
	
	RArray<TPtrC> expectedPackageNames;
	CleanupClosePushL(expectedPackageNames);
	GetStringArrayFromConfigL(ConfigSection(), KPackage, expectedPackageNames);
	
	RArray<TPtrC> expectedVendorNames;
	CleanupClosePushL(expectedVendorNames);
	GetStringArrayFromConfigL(ConfigSection(), KVendor, expectedVendorNames);
	
	RPointerArray<CSisRegistryPackage> packages;
	CleanupResetAndDestroy<RPointerArray<CSisRegistryPackage> >::PushL(packages);
	
	iSisRegistry.RemovablePackagesL(packages);
	
	TInt expected = 0;
	for (expected = 0; expected < expectedPackageNames.Count(); expected++)
		{
		TBool result = EFalse;
		for (TInt actual = 0; actual < packages.Count(); ++actual)
			{
			if (packages[actual]->Uid() == expectedUids[expected] &&
				!packages[actual]->Name().CompareF(expectedPackageNames[expected]) &&
				!packages[actual]->Vendor().CompareF(expectedVendorNames[expected]))
				{
				result = ETrue;
				break;
				}
			}

		if(result)
			{
			INFO_PRINTF3(_L("Package %S by Vendor %S was found!"), &expectedPackageNames[expected], &expectedVendorNames[expected]);
			}
		else
			{
			SetTestStepResult(EFail);
			ERR_PRINTF3(_L("Package %S by Vendor %S was not found!"), &expectedPackageNames[expected], &expectedVendorNames[expected]);
			}
		}
		
	for (expected = 0; expected < expectedNoUids.Count(); expected++)
		{
		TBool result = ETrue;
		for (TInt actual = 0; actual < packages.Count(); ++actual)
			{
			if (packages[actual]->Uid() == expectedNoUids[expected])
				{
				result = EFalse;
				break;
				}
			}

		if(result)
			{
			INFO_PRINTF2(_L("Non-Removable Package %d was not found!"), &expectedNoUids[expected]);
			}
		else
			{
			SetTestStepResult(EFail);
			ERR_PRINTF2(_L("Non-Removable Package %d was found!"), &expectedNoUids[expected]);
			}
		}
	
	CleanupStack::PopAndDestroy(5, &expectedNoUids); //packages, expectedVendorNames, expectedPackageNames
	
	return TestStepResult();
	}
	
/////////////////////////////////////////////////////////////////////
// CNonRemovablePackagesStep - checks Non-removable packages in rom
/////////////////////////////////////////////////////////////////////
CInRomNonRemovablePackage::CInRomNonRemovablePackage()
 	{
 	SetTestStepName(KInRomNonRemovablePackages);
 	}
 
TVerdict CInRomNonRemovablePackage::doTestStepL()
 	{
  	RPointerArray<CSisRegistryPackage> packages;
 	CleanupResetAndDestroy<RPointerArray<CSisRegistryPackage> >::PushL(packages);
 	
 	iSisRegistry.RemovablePackagesL(packages);
 	
 	RSisRegistryEntry entry;
	TBool inrom = EFalse;
	for (TInt i = 0; i < packages.Count(); ++i)
		{
		User::LeaveIfError(entry.OpenL(iSisRegistry, *packages[i]));
		inrom = entry.IsInRomL();
 	  	
 	  	if(inrom)
	  		{
	  		SetTestStepResult(EFail);
	 		}
		}
	INFO_PRINTF1(_L("Packages which are removable are not in ROM"));	
 	CleanupStack::PopAndDestroy(&packages); 
 	
 	return TestStepResult();
 	}
 
//////////////////////////////////////////////////////////////////////////////
// CInstalledDrives - check all the drives where files are installed from SIS
//////////////////////////////////////////////////////////////////////////////
CInstalledDrives::CInstalledDrives()
 	{
 	SetTestStepName(KInstalledDrives);
 	}
 	
TVerdict CInstalledDrives::doTestStepL()
 	{
 	TInt drivebitmapcount=0;
	TUid expectedUid;
	GetUidFromConfig(ConfigSection(), KUid, expectedUid);
	GetIntFromConfig(ConfigSection(), KDrivebitmapcount, drivebitmapcount);

  	RPointerArray<CSisRegistryPackage> packages;
 	CleanupResetAndDestroy<RPointerArray<CSisRegistryPackage> >::PushL(packages);
 	
 	iSisRegistry.RemovablePackagesL(packages);
 	
 	RSisRegistryEntry entry;
	CleanupClosePushL(entry);
 	
 	for (TInt i=0; i<packages.Count(); i++)
 		{
	 	if(expectedUid == packages[i]->Uid())
			{
			User::LeaveIfError(entry.OpenL(iSisRegistry, *packages[i]));
			TUint drives = entry.InstalledDrivesL();
			if(drives != drivebitmapcount)
			   	{
			    SetTestStepResult(EFail);
			    INFO_PRINTF1(_L("Drive bitmask count is not Correct..."));					
			   	}
			 else
				{
				INFO_PRINTF1(_L("Drive bitmask count is OK...")); 	
				}  
 			}
 		}
		
	CleanupStack::PopAndDestroy(2,&packages);  //entry

 	return TestStepResult();
 	}


/////////////////////////////////////////////////////////////////////
// CIsUidInstalledStep - checks whether an Uid is registered
/////////////////////////////////////////////////////////////////////
CIsUidInstalledStep::CIsUidInstalledStep()
	{
	SetTestStepName(KIsUidInstalled);
	}

TVerdict CIsUidInstalledStep::doTestStepL()
	{
	RArray<TUid> expectedUids;
	CleanupClosePushL(expectedUids);
	GetUidArrayFromConfigL(ConfigSection(), KUid, expectedUids);
	
	SetTestStepResult(EPass);
	for (TInt index = 0; index < expectedUids.Count(); index++)
		{
		if(iSisRegistry.IsInstalledL(expectedUids[index]))
			{
			INFO_PRINTF2(_L("Uid: 0x%08x was found!"), expectedUids[index]);
			}
		else
			{
			SetTestStepResult(EFail);
			ERR_PRINTF2(_L("Uid: 0x%08x not found!"), expectedUids[index]);
			}
		}
	CleanupStack::PopAndDestroy(&expectedUids);
	
	return TestStepResult();
	}
	

/////////////////////////////////////////////////////////////////////
// CSetCenRepSettingStep - Set Central Reposatory Value
/////////////////////////////////////////////////////////////////////
CSetCenRepSettingStep::CSetCenRepSettingStep()
    {
    SetTestStepName(KSetCenRepSetting);
    }

TVerdict CSetCenRepSettingStep::doTestStepL()
    {        
    SetTestStepResult(EPass);
    TUid repUid;
    if(!GetUidFromConfig(ConfigSection(), KUid, repUid))
        {
        ERR_PRINTF1(_L("Package UID is missing in the configuration file!"));
        SetTestStepResult(EFail);
        return TestStepResult();
        }
    
    TInt cenRepKey;
    if(!GetIntFromConfig(ConfigSection(), KCenRepKey, cenRepKey))
        {
        ERR_PRINTF1(_L("Central Reposatory Key is missing in the configuration file!"));
        SetTestStepResult(EFail);
        return TestStepResult();
        }
    
    TInt cenRepValue;
    if(!GetIntFromConfig(ConfigSection(), KCenRepValue, cenRepValue))
        {
        ERR_PRINTF1(_L("Central Reposatory Value is missing in the configuration file!"));
        SetTestStepResult(EFail);
        return TestStepResult();
        }
    
    CRepository* rep = CRepository::NewLC(repUid);
    TInt err = rep->Set(cenRepKey, cenRepValue);
        
    if( err == KErrNone )
    {
        INFO_PRINTF4(_L("Setting Central Reposatory  0x%08x  , key = %d, value = %d"),repUid,cenRepKey,cenRepValue);
    }
    else
    {
        SetTestStepResult(EFail);
        ERR_PRINTF4(_L("Failed Setting Central Reposatory  0x%08x  , key = %d, value = %d"),repUid,cenRepKey,cenRepValue);
    }
    
    CleanupStack::PopAndDestroy(rep); 
    return TestStepResult();
    }


/////////////////////////////////////////////////////////////////////
// CIsPackageInstalledStep - checks if packages are registered
/////////////////////////////////////////////////////////////////////

CIsPackageInstalledStep::CIsPackageInstalledStep()
	{
	SetTestStepName(KIsPackageInstalled);
	}

TVerdict CIsPackageInstalledStep::doTestStepL()
	{
	RArray<TUid> expectedUids;
	CleanupClosePushL(expectedUids);
	GetUidArrayFromConfigL(ConfigSection(), KUid, expectedUids);
	
	RArray<TPtrC> expectedPackageNames;
	CleanupClosePushL(expectedPackageNames);
	GetStringArrayFromConfigL(ConfigSection(), KPackage, expectedPackageNames);
	
	RArray<TPtrC> expectedVendorNames;
	CleanupClosePushL(expectedVendorNames);
	GetStringArrayFromConfigL(ConfigSection(), KVendor, expectedVendorNames);
	
	RPointerArray<CSisRegistryPackage> packages;
	CleanupResetAndDestroy<RPointerArray<CSisRegistryPackage> >::PushL(packages);
	
	SetTestStepResult(EPass);
	for (TInt i = 0; i < expectedUids.Count(); ++i)
		{
		CSisRegistryPackage* package = CSisRegistryPackage::NewLC(expectedUids[i],
			expectedPackageNames[i], expectedVendorNames[i]);
		package->SetIndex(0);
		
		RSisRegistryEntry entry;
		CleanupClosePushL(entry);

		TInt err = entry.Open(iSisRegistry, package->Name(), package->Vendor());
		if (err != KErrNone)
			{
			SetTestStepResult(EFail);
			ERR_PRINTF2(_L("Uid: 0x%08x not found!"), expectedUids[i]);
			}
		
		CleanupStack::PopAndDestroy(2, package); // entry
		}
	
	CleanupStack::PopAndDestroy(4, &expectedUids); // expectedVendorNames, expectedPackageNames
	
	return TestStepResult();
	}

/////////////////////////////////////////////////////////////////////
// CIsControllerInstalledStep - checks whether a Controller is present
/////////////////////////////////////////////////////////////////////
CIsControllerInstalledStep::CIsControllerInstalledStep()
	{
	SetTestStepName(KIsControllerInstalled);
	}

TVerdict CIsControllerInstalledStep::doTestStepL()
	{
	RArray<TPtrC> expectedControllers;
	CleanupClosePushL(expectedControllers);
	GetStringArrayFromConfigL(ConfigSection(), KController, expectedControllers);
	
	SetTestStepResult(EPass);
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	for (TInt index = 0; index < expectedControllers.Count(); index++)
		{
		RFile file;
		CleanupClosePushL(file);
		// open file and read the data
		User::LeaveIfError(file.Open(fs, expectedControllers[index], EFileRead | EFileShareReadersOnly));
		// read the data from the file
		TInt fileSize;
		file.Size(fileSize);
		HBufC8* buffer = HBufC8::NewLC(fileSize);
		TPtr8 ptr(buffer->Des());
		file.Read(0, ptr, fileSize);

		if(iSisRegistry.IsInstalledL(*buffer))
			{
			INFO_PRINTF2(_L("%S matching controller was found!"), &expectedControllers[index]);
			}
		else
			{
			SetTestStepResult(EFail);
			INFO_PRINTF2(_L("%S controller did not match expected!"), &expectedControllers[index]);
			}
		CleanupStack::PopAndDestroy(2, &file); //buffer
		}
	CleanupStack::PopAndDestroy(2, &expectedControllers); //fs
	
	return TestStepResult();
	}

/////////////////////////////////////////////////////////////////////
// CGetControllersStep - checks whether one can get the binary version 
// of entry controllers present
/////////////////////////////////////////////////////////////////////
CGetControllersStep::CGetControllersStep()
	{
	SetTestStepName(KGetControllers);	
	}

TVerdict CGetControllersStep::doTestStepL()
	{
	SetTestStepResult(EPass);
	
	RArray<TUid> expectedUids;
	CleanupClosePushL(expectedUids);
	GetUidArrayFromConfigL(ConfigSection(), KUid, expectedUids);
	
	RFs fs;
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.Connect());
	
	for (TInt index = 0; index < expectedUids.Count(); index++)
		{
		TInt res;		
		RSisRegistryEntry entry;
		CleanupClosePushL(entry);
		INFO_PRINTF3(_L("Open: %08x %d entry!"), expectedUids[index],index);
		res = entry.Open(iSisRegistry, expectedUids[index]);
		if (res == KErrNone)
			{
			RPointerArray<HBufC8> actualControllers;
			CleanupResetAndDestroy<RPointerArray<HBufC8> >::PushL(actualControllers);
			TRAP(res, entry.ControllersL(actualControllers));
		  	if (res != KErrNone)
				{
				INFO_PRINTF3(_L("Accessing controllers for uid: %08x left with %d!"), expectedUids[index], res);
				SetTestStepResult(EFail);
				User::Leave(res);
				}
		    
		    HBufC* keyName = HBufC::NewLC(KUid().Length() + KKeyFormat().Length()
							+ KController().Length() + KKeyFormat().Length());
			TPtr ptr(keyName->Des());
			ptr = KUid();
			ptr.AppendFormat(KKeyFormat(), index + 1);
			ptr.Append(KController);
			
			RArray<TPtrC> controllerFilenames;
			CleanupClosePushL(controllerFilenames);
			GetStringArrayFromConfigL(ConfigSection(), ptr, controllerFilenames);
		    
		    if (controllerFilenames.Count() != actualControllers.Count())
				{
				ERR_PRINTF3(_L("Actual Controllers = %d, Expected Controllers = %d"), actualControllers.Count(), controllerFilenames.Count());
				SetTestStepResult(EFail);
				}
		    		    
		    for (TInt filename = 0; filename < controllerFilenames.Count(); filename++)
		    	{
		    	RFile file;
				CleanupClosePushL(file);
				// open file and read the data
				User::LeaveIfError(file.Open(fs, controllerFilenames[filename], EFileRead | EFileShareReadersOnly));
				// read the data from the file
				TInt fileSize;
				file.Size(fileSize);
				// create a buffer according to file size and load it with data
				HBufC8* buffer = HBufC8::NewLC(fileSize);
				TPtr8 ptr(buffer->Des());
				file.Read(0, ptr, fileSize);
				
				TBool found = EFalse;
				for (TInt controller = 0; controller < actualControllers.Count(); controller++)
					{
					if(*actualControllers[controller] == *buffer)
						{
						found = ETrue;
						break;
						}
					}
		
				if(found)
		   			{
		   			INFO_PRINTF3(_L("Matching controller #%d found for Uid: %08x!"), filename, expectedUids[index]);
					}
				else
					{			
		   			SetTestStepResult(EFail);
		   			ERR_PRINTF3(_L("Matching controller #%d not found for Uid: %08x!"), filename, expectedUids[index]);
					}
					
				// cleanup
				CleanupStack::PopAndDestroy(2, &file); //buffer
		    	}
		    
			CleanupStack::PopAndDestroy(3, &actualControllers); // controllerFilenames, keyNames
			}
		else
			{
			SetTestStepResult(EFail);
			INFO_PRINTF2(_L("Failed Open Uid: %08x entry!"), expectedUids[index]);
			}
		
		CleanupStack::PopAndDestroy(&entry);
		}
		
	CleanupStack::PopAndDestroy(2, &expectedUids);
	
	return TestStepResult();
	}

/////////////////////////////////////////////////////////////////////
// CEntryUidOpenCloseStep
/////////////////////////////////////////////////////////////////////
CEntryUidOpenCloseStep::CEntryUidOpenCloseStep()
	{
	SetTestStepName(KEntryUidOpenClose);
	}

TVerdict CEntryUidOpenCloseStep::doTestStepL()
	{
	RArray<TUid> uids;
	CleanupClosePushL(uids);
	iSisRegistry.InstalledUidsL(uids);
    
    SetTestStepResult(EPass);
    for(TInt index = 0; index < uids.Count(); index++)
    	{
    	Swi::RSisRegistryEntry entry;
    	CleanupClosePushL(entry);
    	TInt result = entry.Open(iSisRegistry, uids[index]);
    	if (result == KErrNone)
    		{
			INFO_PRINTF2(_L("Uid: 0x%08x was opened successfully!"), uids[index]);
			entry.Close();
			INFO_PRINTF2(_L("Uid: 0x%08x was closed successfully!"), uids[index]);
			}
		else
			{
			SetTestStepResult(EFail);
			ERR_PRINTF3(_L("Uid: 0x%08x not opened, error %d!"), uids[index], result);
			}
		CleanupStack::PopAndDestroy(&entry);
    	}
    
    CleanupStack::PopAndDestroy(&uids);
    
	return TestStepResult();
	}
	
/////////////////////////////////////////////////////////////////////
// CEntryPackageOpenCloseStep
/////////////////////////////////////////////////////////////////////
CEntryPackageOpenCloseStep::CEntryPackageOpenCloseStep()
	{
	SetTestStepName(KEntryPackageOpenClose);
	}

TVerdict CEntryPackageOpenCloseStep::doTestStepL()
	{
	RPointerArray<CSisRegistryPackage> packages;
	CleanupResetAndDestroy<RPointerArray<CSisRegistryPackage> >::PushL(packages);
	
	iSisRegistry.InstalledPackagesL(packages);
	
	SetTestStepResult(EPass);
    for(TInt index = 0; index < packages.Count(); index++)
    	{
    	TPtrC package = packages[index]->Name();
    	TPtrC vendor = packages[index]->Vendor();
    	
    	Swi::RSisRegistryEntry entry;
    	
    	TInt result = entry.OpenL(iSisRegistry, *packages[index]);
    	if (result == KErrNone)
    		{
    		CleanupClosePushL(entry);
			INFO_PRINTF3(_L("Package %S by Vendor %S was opened successfully!"), &package, &vendor);
			CleanupStack::PopAndDestroy(&entry);
			INFO_PRINTF3(_L("Package %S by Vendor %S was closed successfully!"), &package, &vendor);
			}
		else
			{
			SetTestStepResult(EFail);
			ERR_PRINTF4(_L("Package %S by Vendor %S not opened, error %d!"), &package, &vendor, result);
			}
    	}
    
    CleanupStack::PopAndDestroy(&packages);
    
	return TestStepResult();
	}

/////////////////////////////////////////////////////////////////////
// CEntryNameOpenCloseStep
/////////////////////////////////////////////////////////////////////
CEntryNameOpenCloseStep::CEntryNameOpenCloseStep()
	{
	SetTestStepName(KEntryNameOpenClose);
	}

TVerdict CEntryNameOpenCloseStep::doTestStepL()
	{
	RPointerArray<CSisRegistryPackage> packages;
	CleanupResetAndDestroy<RPointerArray<CSisRegistryPackage> >::PushL(packages);
	
	iSisRegistry.InstalledPackagesL(packages);
	
	SetTestStepResult(EPass);
    for(TInt index = 0; index < packages.Count(); index++)
    	{
    	TPtrC package = packages[index]->Name();
    	TPtrC vendor = packages[index]->Vendor();
    	
    	Swi::RSisRegistryEntry entry;
    	
    	TInt result = entry.Open(iSisRegistry, package, vendor);
    	if (result == KErrNone)
    		{
    		CleanupClosePushL(entry);
			INFO_PRINTF3(_L("Package %S by Vendor %S was opened successfully!"), &package, &vendor);
			CleanupStack::PopAndDestroy(&entry);
			INFO_PRINTF3(_L("Package %S by Vendor %S was closed successfully!"), &package, &vendor);
			}
		else
			{
			SetTestStepResult(EFail);
			ERR_PRINTF4(_L("Package %S by Vendor %S not opened, error %d!"), &package, &vendor, result);
			}
    	}
    
    CleanupStack::PopAndDestroy(&packages);
    
	return TestStepResult();
	}

/////////////////////////////////////////////////////////////////////
// CEntryMethodsTestStep - tests specific to entry methods
/////////////////////////////////////////////////////////////////////
CEntryMethodsStep::CEntryMethodsStep()
	{
	SetTestStepName(KEntryMethods);
	}

TVerdict CEntryMethodsStep::doTestStepL()
	{
	RPointerArray<CSisRegistryPackage> packages;
	CleanupResetAndDestroy<RPointerArray<CSisRegistryPackage> >::PushL(packages);
	
	iSisRegistry.InstalledPackagesL(packages);
		
	SetTestStepResult(EPass);
    for(TInt index = 0; index < packages.Count(); index++)
    	{
    	TPtrC package = packages[index]->Name();
    	TPtrC vendor = packages[index]->Vendor();
    	
    	Swi::RSisRegistryEntry entry;
    	
    	INFO_PRINTF3(_L("Opening Package %S by Vendor %S..."), &package, &vendor);
    	User::LeaveIfError(entry.OpenL(iSisRegistry, *packages[index]));
    	CleanupClosePushL(entry);
		// get the version
		TVersion packageVersion = entry.VersionL(); 
		INFO_PRINTF4(_L("Version  %d %d %d"), packageVersion.iMajor, packageVersion.iMinor, packageVersion.iBuild );
	
		// get sids	
		RArray<TUid> sids;
		CleanupClosePushL(sids);
		entry.SidsL(sids);
		if (sids.Count() <= 0)
			{
			INFO_PRINTF1(_L("No sids installed!"));
			}
		else 
			{
			for (TInt sid = 0; sid < sids.Count(); sid++)
				{
				INFO_PRINTF2(_L("Sid: 0x%08x was found!"), sids[sid]);
				}
			}
		
		CleanupStack::PopAndDestroy(&sids);

		// get the vendor; package name 
		HBufC* reportedVendor = entry.UniqueVendorNameL();
		CleanupStack::PushL(reportedVendor);
		HBufC* reportedPackage = entry.PackageNameL();
		CleanupStack::PushL(reportedPackage);
		
		INFO_PRINTF3(_L("Package: %S from Vendor: %S"), reportedPackage, reportedVendor); 
	
		CleanupStack::PopAndDestroy(reportedPackage);
		CleanupStack::PopAndDestroy(reportedVendor);

		// get the localised vendor name 
		HBufC* vendorLocal = entry.LocalizedVendorNameL();
		CleanupStack::PushL(vendorLocal);
		INFO_PRINTF2(_L("Localised Vendor Name: %S"), vendorLocal); 
	
		CleanupStack::PopAndDestroy(vendorLocal);

		// get uid
		TUid uid = entry.UidL();
		INFO_PRINTF3(_L("Package Uid: %d 0x%08x"), uid.iUid, uid.iUid);
	
		// get language
		TLanguage language = entry.LanguageL();
		INFO_PRINTF3(_L("Package Language: %d 0x%08x"), language, language);

		// get property
		TInt32 propertyKey = 10;
		TInt32 propertyValue = entry.PropertyL(propertyKey);
		INFO_PRINTF3(_L("Package property: key %d  value %d"), propertyKey, propertyValue);

		// is present - currenty relies on default value
		TBool isPresent = entry.IsPresentL();
		INFO_PRINTF2(_L("Package present is  %d"), isPresent);
  	
  		// is signed
		TBool isSigned = entry.IsSignedL();
		INFO_PRINTF2(_L("Package signed is  %d"), isSigned);
  

		// check Trust
		switch(entry.TrustL())
			{
			case ESisPackageUnsignedOrSelfSigned:
				INFO_PRINTF1(_L("Packaged Trust: Unsigned or self signed"));
				break;
			case ESisPackageCertificateChainNoTrustAnchor:
				INFO_PRINTF1(_L("Packaged Trust: Certificate Chain has no trust anchor in Cert store"));
				break;
			case ESisPackageCertificateChainValidatedToTrustAnchor:
				INFO_PRINTF1(_L("Packaged Trust: Certificate Chain Validated to Trust Anchor in Cert store"));
				break;
			case ESisPackageChainValidatedToTrustAnchorOCSPTransientError:
				INFO_PRINTF1(_L("Packaged Trust: Certificate Chain Validated and OCSP Transient Error"));
				break;
			case ESisPackageChainValidatedToTrustAnchorAndOCSPValid:
				INFO_PRINTF1(_L("Packaged Trust: Certificate Chain Validated and OCSP Valid"));
				break;
			case ESisPackageBuiltIntoRom:
				INFO_PRINTF1(_L("Packaged Trust: Built into ROM"));
				break;
			case ESisPackageValidationFailed:
				INFO_PRINTF1(_L("Packaged Trust: Certificate Chain Validation Failed"));
				break;
			default:
				INFO_PRINTF1(_L("Packaged Trust: Unknown"));
				break;
			};

		// Time at which the trust was established
	  	TTime timeStamp = entry.TrustTimeStampL();
  		TBuf <32> dateTimeText;
		TRAP_IGNORE(timeStamp.FormatL(dateTimeText, _L("%H%:1%T%:2%S on %1%/1%2%/2%3")));
		INFO_PRINTF2(_L("Package Trust Established: %S"), &dateTimeText);

    	// is in rom
		TBool inRom = entry.IsInRomL();
		INFO_PRINTF2(_L("Package inRom is  %d"), inRom);

    	// is an augmentation
		TBool isAug = entry.IsAugmentationL();
		INFO_PRINTF2(_L("Package isAug is  %d"), isAug);
  	
  		// installation size
		TInt64 size = entry.SizeL();
		INFO_PRINTF3(_L("Package size is %d %d"), I64HIGH(size), I64LOW(size));
  	
  		// get list of files
  		RPointerArray<HBufC> files;
  		CleanupResetAndDestroy<RPointerArray<HBufC> >::PushL(files);
  		entry.FilesL(files);
 		INFO_PRINTF2(_L("Package has %d files"), files.Count());
 		for (TInt file = 0; file < files.Count(); file++)
 			{
		//	INFO_PRINTF3(_L("File %d  file path %S"),file + 1, files[file]);	  	
 			}
  		CleanupStack::PopAndDestroy(&files);
  
		CleanupStack::PopAndDestroy(&entry);
    	}
    
    CleanupStack::PopAndDestroy(&packages);
	
	return TestStepResult();
	}	
	
				
/////////////////////////////////////////////////////////////////////
// CEntrySidsStep - obtain a list of Sids from an entry
/////////////////////////////////////////////////////////////////////
CEntrySidsStep::CEntrySidsStep()
	{
	SetTestStepName(KEntrySids);
	}

TVerdict CEntrySidsStep::doTestStepL()
	{
	SetTestStepResult(EFail);
	
	RArray<TUid> expectedSids;
	CleanupClosePushL(expectedSids);
	GetUidArrayFromConfigL(ConfigSection(), KSid, expectedSids);
	
	TPtrC package;
	TPtrC vendor;
	if(GetStringFromConfig(ConfigSection(), KPackage, package)
	&& GetStringFromConfig(ConfigSection(), KVendor, vendor))
		{
		Swi::RSisRegistryEntry entry;
		User::LeaveIfError(entry.Open(iSisRegistry, package, vendor));
		CleanupClosePushL(entry);
		
		RArray<TUid> sids;
		CleanupClosePushL(sids);
		entry.SidsL(sids);
		
		if (sids.Count() != expectedSids.Count())
			{
			ERR_PRINTF3(_L("Actual Sids = %d, Expected Sids = %d"), sids.Count(), expectedSids.Count());
			}
		else 
			{
			SetTestStepResult(EPass);
			for (TInt index=0; index < expectedSids.Count(); index++)
				{
				if(sids.Find(expectedSids[index]) != KErrNotFound)
					{
					INFO_PRINTF2(_L("Sid: 0x%08x was found!"), expectedSids[index]);
					}
				else
					{
					SetTestStepResult(EFail);
					ERR_PRINTF2(_L("Sid: 0x%08x not found!"), expectedSids[index]);
					}
				}
			}
		
		CleanupStack::PopAndDestroy(&sids);
		CleanupStack::PopAndDestroy(&entry);
		}
	
	CleanupStack::PopAndDestroy(&expectedSids);
	return TestStepResult();
	}

/////////////////////////////////////////////////////////////////////
// CIsSidPresentStep - test package SIDs are reported as present.
/////////////////////////////////////////////////////////////////////

CIsSidPresentStep::CIsSidPresentStep()
	{
	SetTestStepName(KIsSidPresentStep);
	}

TVerdict CIsSidPresentStep::doTestStepL()
	{
	// iterate through every sid which is installed in a package
	RPointerArray<CSisRegistryPackage> packages;
	CleanupResetAndDestroy<RPointerArray<CSisRegistryPackage> >::PushL(packages);
	
	iSisRegistry.InstalledPackagesL(packages);

	TInt32 minSid = KMaxTInt32;
	TInt32 maxSid = KMinTInt32;

	for (TInt i = packages.Count() - 1; i >= 0; --i)
		{
		const CSisRegistryPackage& pkg = *packages[i];

		Swi::RSisRegistryEntry e;
		TInt r = e.Open(iSisRegistry, pkg.Uid());
		User::LeaveIfError(r);
		CleanupClosePushL(e);

		RArray<TUid> sids;
		e.SidsL(sids);
		CleanupClosePushL(sids);

		for (TInt j = sids.Count() - 1; j >= 0; --j)
			{
			TUid sid = sids[j];
			minSid = Min(minSid, sid.iUid);
			maxSid = Max(maxSid, sid.iUid);
			
			TBool sidPresent = iSisRegistry.IsSidPresentL(sid);
			TESTL(sidPresent);
			}

		CleanupStack::PopAndDestroy(2, &e);
		}

	TUid minSidM1(TUid::Uid(minSid - 1));
	TESTL(! iSisRegistry.IsSidPresentL(minSidM1));
	TUid maxSidP1(TUid::Uid(minSid + 1));
	TESTL(! iSisRegistry.IsSidPresentL(maxSidP1));	
	
	CleanupStack::PopAndDestroy(&packages);
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}
	
/////////////////////////////////////////////////////////////////////
// CIsPackagePresentStep - Test the IsPresent method
/////////////////////////////////////////////////////////////////////

CIsPackagePresentStep::CIsPackagePresentStep()
	{
	SetTestStepName(KIsPackagePresentStep);
	}
	
TVerdict CIsPackagePresentStep::doTestStepL()
	{
	SetTestStepResult(EPass);
	
	RArray<TUid> pUids;
	CleanupClosePushL(pUids);
	GetUidArrayFromConfigL(ConfigSection(), KUid, pUids);
	
	TInt count(pUids.Count());
	for (TInt i = 0; i < count; ++i)
		{
		// before each step, reset the registry state
	
		RTestUtilSessionSwi util;
		User::LeaveIfError(util.Connect());
		CleanupClosePushL(util);
		util.RegenerateRegistryCacheL();
		CleanupStack::PopAndDestroy(&util);
		
		// query the registry to see if this entry is present
		RSisRegistryEntry entry;
 		//Since the SisRegistry is inconsistent from hardware and emulator, we won't check
 		//whether a package is presented on device if it's not installed at all thus no registry entry (e.g. MIDP)
 		#ifdef __WINS__
 			User::LeaveIfError(entry.Open(iSisRegistry, pUids[i]));
 		#else
 			TInt err = entry.Open(iSisRegistry, pUids[i]);
 			if(KErrNotFound == err)
 				{
 				INFO_PRINTF2(_L("Package with UID 0x%08x is not installed on HW. IsPresent test skipped"),
 						pUids[i]);
 					continue;
 				}
 			User::LeaveIfError(err);
 		#endif
		CleanupClosePushL(entry);
		
		if (!entry.IsPresentL())
			{
			INFO_PRINTF2(_L("Package with UID 0x%08x is marked as not present!"),
				pUids[i]);
			SetTestStepResult(EFail);
			}
		
		CleanupStack::PopAndDestroy(&entry);
		}
	CleanupStack::PopAndDestroy(&pUids);
	return TestStepResult();
	}
CIntegrityCheckStep::CIntegrityCheckStep()
	{
	SetTestStepName(KIntegrityCheckStep);
	}
CIntegrityCheckStep::~CIntegrityCheckStep()
	{
	}

TVerdict CIntegrityCheckStep::doTestStepL()
	{
	TInt result=0;

	
	CleanupResetAndDestroy<RPointerArray<CX509Certificate> >::PushL(iCertArray);
	
	InitializeL();
	TUid pUid;
	GetUidFromConfig(ConfigSection(), KUid, pUid);
	GetIntFromConfig(ConfigSection(),KResultTag,result);

	RSisRegistryEntry entry;
	
	SetTestStepResult(EFail);

	
	// query the registry to see if this entry is present	
	User::LeaveIfError (entry.Open(iSisRegistry, pUid));
	
	//Call the controller
	TBool check = EFalse;

	check = entry.VerifyControllerSignatureL(iCertArray);
		
	if(check!=result)
		{
		SetTestStepResult(EFail);
		}
	else
		{
		SetTestStepResult(EPass);
		}
	entry.Close();
	CleanupStack::PopAndDestroy(&iCertArray);
	return TestStepResult();
	}
	
void CIntegrityCheckStep::InitializeL()
	{
	CX509Certificate* cert = NULL;
	RArray<TPtrC> rootCertificates;
	CleanupClosePushL(rootCertificates);
	GetStringArrayFromConfigL(ConfigSection(), KRootCertTag, rootCertificates);
	
	User::LeaveIfError (fs.Connect());
	//opens the data file
	CleanupClosePushL(fs);

	//Loop through all the X509 root certificates and load them
	for (TInt i=0; i<rootCertificates.Count(); i++)
		{
		HBufC8* rawData = ReadFileLC(rootCertificates[i]);
		cert = CX509Certificate::NewL(*rawData);
		CleanupStack::PopAndDestroy(rawData);
		CleanupStack::PushL(cert);
		iCertArray.AppendL(cert);
		CleanupStack::Pop();
		}
	
	//closes the file session
	CleanupStack::PopAndDestroy(&fs);
	CleanupStack::PopAndDestroy(&rootCertificates);
	}

HBufC8* CIntegrityCheckStep::ReadFileLC(const TDesC& aDatapath)
	{ 	
	RFile file;
	User::LeaveIfError(file.Open(fs, aDatapath, EFileRead)) ; 
	
	TInt fileSize = 0;
	User::LeaveIfError(file.Size(fileSize));	
	
	HBufC8* rawData = HBufC8::NewLC(fileSize);
	TPtr8 rawDataPtr(rawData->Des());
	rawDataPtr.SetLength(fileSize);	
	file.Read (rawDataPtr);
	
	file.Close();
	
	return rawData;
	}

/////////////////////////////////////////////////////////////////////
// CIntegrityDateAndTimeCheckStep - 
/////////////////////////////////////////////////////////////////////	


CIntegrityDateAndTimeCheckStep::CIntegrityDateAndTimeCheckStep()
	{
	SetTestStepName(KDateAndTimeIntegrityCheckStep);
	}


TVerdict CIntegrityDateAndTimeCheckStep::doTestStepPreambleL()
	{
	CIntegrityCheckStep::doTestStepPreambleL();
	
	GetUidFromConfig(ConfigSection(), KUid, iPackageUid);
	GetIntFromConfig(ConfigSection(),KResultTag,iExpectedResult);
	
	TPtrC dateAndTime;
	GetStringFromConfig(ConfigSection(), KDateTime, dateAndTime);
	iTime = TTime(dateAndTime);
	
	iOriginalTime.UniversalTime();
	User::SetUTCTime(iTime);
	
	return EPass;
	}

TVerdict CIntegrityDateAndTimeCheckStep::doTestStepPostambleL()
	{
	TTime timeNow;
	timeNow.UniversalTime();
	
	TTimeIntervalMicroSeconds timeInterval = timeNow.MicroSecondsFrom(iTime);
	timeNow = iOriginalTime + timeInterval;
	
	User::SetUTCTime(timeNow);
	
	return TestStepResult();
	}

TVerdict CIntegrityDateAndTimeCheckStep::doTestStepL()
	{
	CleanupResetAndDestroy<RPointerArray<CX509Certificate> >::PushL(iCertArray);
	
	InitializeL();

	RSisRegistryEntry entry;
	
	SetTestStepResult(EPass);
	// query the registry to see if this entry is present	
	User::LeaveIfError(entry.Open(iSisRegistry, iPackageUid));
	CleanupClosePushL(entry);
	
	//Call the controller
	TBool check = EFalse;
	
	check = entry.VerifyControllerSignatureL(iCertArray, ETrue);
	if(check!=iExpectedResult)
		{
		ERR_PRINTF3(_L("ERROR: First call to verify failed.  Result: %d.  Expected Result: %d"), check, iExpectedResult);
		SetTestStepResult(EFail);
		}

	check = entry.VerifyControllerSignatureL(iCertArray, EFalse);
	if(check==EFalse)
		{
		ERR_PRINTF2(_L("ERROR: Second call to verify failed.  Result: %d.  Expected Result: ETrue"), check);
		SetTestStepResult(EFail);
		}
		
	check = entry.VerifyControllerSignatureL(iCertArray, ETrue);
	if(check!=iExpectedResult)
		{
		ERR_PRINTF3(_L("ERROR: Third call to verify failed.  Result: %d.  Expected Result: %d"), check, iExpectedResult);
		SetTestStepResult(EFail);
		}		
			
	CleanupStack::PopAndDestroy(2, &iCertArray);
	
	return TestStepResult();
	}


/////////////////////////////////////////////////////////////////////
// CLoggingFileInfoStep - 
/////////////////////////////////////////////////////////////////////

CLoggingFileInfoStep::CLoggingFileInfoStep()
 	{
 	SetTestStepName(KLoggingFileInfo);
 	}
 
TVerdict CLoggingFileInfoStep::doTestStepL()
 	{
 	SetTestStepResult(EPass);
    __UHEAP_MARK;
 	RPointerArray<CLogEntry> fileInfo;
 	CleanupResetAndDestroy<RPointerArray<CLogEntry> >::PushL(fileInfo);
 	iSisRegistry.RetrieveLogFileL(fileInfo);
 
 	if	(fileInfo.Count()>0)
	 	{
	  	for (TInt i = 0; i < 5; i++)
	 		{ 
	 		TInt var = 1;
	 		TBuf<16> entry;
	 		entry.Format(_L("LOGENTRY%d.%d"),i,var);	 		
		  	TPtrC text0;	
			GetStringFromConfig(ConfigSection(), entry, text0);
			entry.Format(_L("LOGENTRY%d.%d"),i,++var);
		 TPtrC text1;	
		 	GetStringFromConfig(ConfigSection(), entry, text1);
		 	entry.Format(_L("LOGENTRY%d.%d"),i,++var);
		 TPtrC text2;	
		 	GetStringFromConfig(ConfigSection(), entry, text2);
		 	entry.Format(_L("LOGENTRY%d.%d"),i,++var);
		 TPtrC text3;	
		 	GetStringFromConfig(ConfigSection(), entry, text3);
		 	
		 	TBuf<16> pkgName = fileInfo[0]->GetPkgName();
		 	TBuf<20> versiontext;
			versiontext.Format(_L("%d.%d.%d"),fileInfo[i]->GetMajorVersion(),fileInfo[i]->GetMinorVersion(),fileInfo[i]->GetBuildVersion());
			
			enum Swi::TSwiLogTypes s = fileInfo[i]->GetInstallType();
			TBuf<16> installType ;
			switch (s)
					{
					case 0: // Swi::TSwiLogTypes::ESwiLogInstall :
						installType.Format(_L("Install"));
						break;
					case 1: 
						installType.Format(_L("UnInstall"));
						
						break;
					case 2: // Swi::TSwiLogTypes::ESwiLogUpgrade :
					installType.Format(_L("Upgrade"));
					
						break;
					default:
					installType.Format(_L("Error unknown value"));
					
						break;
					}
				
			TBuf<16> uidOfPkg;
			uidOfPkg.Format(_L("%x"),fileInfo[i]->GetUid());
			
			if(pkgName == text0 && versiontext == text1
			&& installType ==text2 && uidOfPkg ==text3)
					{
					SetTestStepResult(EPass);
					}
				else
					{
					SetTestStepResult(EFail);
					}
		  	}
	 	}
 	else
 		{
 		SetTestStepResult(EPass);
 		}
	CleanupStack::PopAndDestroy(&fileInfo);
 	__UHEAP_MARKEND;
	return TestStepResult();
 	}

CRomStubPerformanceStep::CRomStubPerformanceStep()
	{
	SetTestStepName(KRomStubPerformanceStep);
	}

TVerdict CRomStubPerformanceStep::doTestStepL()
	{
	// Simple test step to determine how long it takes to
	// regenerate the cache.
	
	// Make sure the sisregistry is deleted before regenerating the cache. This
	// will ensure that the registry entries are created from the ROM stub, instead
	// of being read from the harddisk
	RTestUtilSessionSwi util;
	User::LeaveIfError(util.Connect());
	CleanupClosePushL(util);
	
	TDriveUnit sysDrive (RFs::GetSystemDrive());
	TBuf<128> sisRegistryPath (sysDrive.Name());
	sisRegistryPath.Append(KSisRegistryPath);

	TInt err = util.RmDir(sisRegistryPath);
	if(err != KErrNone && err != KErrPathNotFound && err != KErrNotFound)
		{
		User::Leave(err);
		}
	else
		{
		INFO_PRINTF1(_L("Sis Registry Deleted!"));
		}
	
	INFO_PRINTF1(_L("Regenerate the cache. Starting the timer..."));
	TTime startTime;
	startTime.UniversalTime();
	
	RTestUtilSessionSwi utilswi;
	User::LeaveIfError(utilswi.Connect());
	CleanupClosePushL(utilswi);

	utilswi.RegenerateRegistryCacheL();
	
	// Stop the timer
	TTime endTime;
	endTime.UniversalTime();
	INFO_PRINTF1(_L("Cache regenerated. Timer stopped."));

	TInt64 durationMicro=endTime.MicroSecondsFrom(startTime).Int64();
	TInt durationMilli=durationMicro/1000;
	
	INFO_PRINTF2(_L("Duration (milliseconds): %d"),durationMilli);
	
	CleanupStack::PopAndDestroy(&utilswi);
	CleanupStack::PopAndDestroy(&util);
	
	SetTestStepResult(EPass);

	return TestStepResult();
	}

//////////////////////////////////////////////////////////////
// CCorruptCacheStep - The Sis Registry will leave backup.bak 
// around when it encounters a corrupt cache file
//////////////////////////////////////////////////////////////

CCorruptCacheStep::CCorruptCacheStep()
	{
	SetTestStepName(KCorruptCacheStep);
	}

TVerdict CCorruptCacheStep::doTestStepL()
	{
	SetTestStepResult(EFail);
	
	__UHEAP_MARK;
	
	RTestUtilSessionSwi util;
	User::LeaveIfError(util.Connect());
	CleanupClosePushL(util);
	TDriveUnit sysDrive (RFs::GetSystemDrive());
	TDriveName sysDriveName (sysDrive.Name());

	// Close the Sis Registry and wait for 3 secs for
	// server to shut down
	iSisRegistry.Close();
	
	INFO_PRINTF1(_L("Sis Registry shut down"));
	User::After(3000000);
	
	// Delete the "good" backup.lst (if present)
	TInt err = util.Delete(KCacheBackupFile);

	if (err == KErrNone)
		{
		INFO_PRINTF2(_L("Deleted '%S'"), &KCacheBackupFile);	
		}
	
	// Use hashforlang_txt.dat as a corrupt backup.lst and connect to the 
	// Sis Registry.
	util.Copy(KCorruptCacheBackupFile, KCacheBackupFile);
	
	INFO_PRINTF3(_L("Copied '%S' to '%S'"), &KCorruptCacheBackupFile, &KCacheBackupFile);
	
	INFO_PRINTF1(_L("Connecting to SisRegistry..."));
	
	RProcess sisRegistryServer;
	TInt retVal = sisRegistryServer.Create(_L("SisRegistryServer"),KNullDesC);
	
	if (retVal == KErrNone)
		{
		TRequestStatus status;
		sisRegistryServer.Logon(status);
		sisRegistryServer.Resume();
		User::WaitForRequest(status);

		TExitCategoryName panicName = sisRegistryServer.ExitCategory();
		TPtrC name(panicName);
		TInt reasonNumber = sisRegistryServer.ExitReason();
		
		if (reasonNumber == 0 && name.Compare(_L("Kill")) == KErrNone)
			{
			if (util.FileExistsL(KSysBackupFileBackup))
				{
				if (util.FileExistsL(KCacheBackupFile))
					{
					INFO_PRINTF4(_L("'%S' and '%S' exist in System Drive: %S\\"),
							&KSysBackupFileBackup, &KCacheBackupFile, &sysDriveName);
					SetTestStepResult(EPass);
					}
				else
					{
					INFO_PRINTF3(_L("Expecting '%S' exist in System Drive: %S\\ but not found"),
								&KCacheBackupFile, &sysDriveName);
					}
				}
			else
				{
				INFO_PRINTF3(_L("Expecting '%S' exist in System Drive: %S\\ but not found"),
							&KSysBackupFileBackup, &sysDriveName);
				}
			}

		INFO_PRINTF3(_L("CCorruptCacheStep - Exit Reason Number: %d Exit Reason Code: %S"),
						reasonNumber, &name);
		}
		
	sisRegistryServer.Terminate(0);
	sisRegistryServer.Close();
		
	CleanupStack::PopAndDestroy(&util);

	__UHEAP_MARKEND;
	
	return TestStepResult();
	}

////////////////////////////////////////////////////////////////
// CCorruptCacheRecoverStep - Checks to see if the Sis Registry 
// recovers from a corrupt token cache file
////////////////////////////////////////////////////////////////

CCorruptCacheRecoverStep::CCorruptCacheRecoverStep()
	{
	SetTestStepName(KCorruptCacheRecoverStep);
	}

TVerdict CCorruptCacheRecoverStep::doTestStepL()
	{
	SetTestStepResult(EFail);	
	
	RTestUtilSessionSwi util;
	User::LeaveIfError(util.Connect());
	CleanupClosePushL(util);
	TDriveUnit sysDrive (RFs::GetSystemDrive());
	TDriveName sysDriveName (sysDrive.Name());
	
	// Close the Sis Registry and wait for 3 secs for
	// the server to shut down
	iSisRegistry.Close();
	INFO_PRINTF1(_L("Sis Registry shut down"));
	User::After(3000000);
	
	// Attempt to read from the token cache file
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RFileReadStream stream;
	User::LeaveIfError(stream.Open(fs, KCacheBackupFile, EFileRead | EFileShareReadersOnly));
	CleanupClosePushL(stream);
	
	INFO_PRINTF2(_L("'%S' regenerated successfully. Check by reading and internalizing..."), &KCacheBackupFile); 
	
	RPointerArray<CSisRegistryToken> tokens;
	CleanupResetAndDestroy<RPointerArray<CSisRegistryToken> >::PushL(tokens);
	
	// Will not catch a panic, but still a worthwhile check to make
	TRAPD(err, stream.ReadUint32L());
	TRAP(err, InternalizePointerArrayL(tokens, stream));
	
	if (err == KErrNone)
		{
		INFO_PRINTF2(_L("'%S' successfully read!"), &KCacheBackupFile);
		
		if (!util.FileExistsL(KSysBackupFileBackup))
			{
			if (util.FileExistsL(KCacheBackupFile))
				{
				INFO_PRINTF3(_L("'%S' exist in System Drive: %S\\"),
							&KCacheBackupFile, &sysDriveName);
							
				SetTestStepResult(EPass);
				}
			else
				{
				INFO_PRINTF3(_L("Expecting '%S' exist in System Drive: %S\\ but not found"),
							&KCacheBackupFile, &sysDriveName);
				}
			}
		else
			{
			INFO_PRINTF3(_L("'%S' exist in System Drive: %S\\ but shouldn't"),
						&KSysBackupFileBackup, &sysDriveName);
			}
		}
		
	CleanupStack::PopAndDestroy(4, &util);
	
	return TestStepResult();		
	}

/////////////////////////////////////////////////////////////////////
// CChangeLocaleStep - Changes the locale
/////////////////////////////////////////////////////////////////////

CChangeLocaleStep::CChangeLocaleStep()
	{
	SetTestStepName(KChangeLocale);
	}

TVerdict CChangeLocaleStep::doTestStepL()
    {
    SetTestStepResult(EFail);

    TInt language=0;
    if (!GetIntFromConfig(ConfigSection(), KLanguage, language))
        {
        INFO_PRINTF1(_L("Missing language in config section"));
        return EFail;
        }

    #ifdef SYMBIAN_DISTINCT_LOCALE_MODEL 
    _LIT(KLitLanguageLocaleDllNameBase, "elocl_lan");
    //Region and collation code values are hard coded, as the check, after changing the locale is made for the language only.
    _LIT(KLitRegionLocaleDllNameBase, "elocl_reg.826");        
    _LIT(KLitCollationLocaleDllNameBase, "elocl_col.001");
    _LIT(ThreeDigExt,".%03d");
            
    TExtendedLocale localeDll;    
    const TUidType uidType(TUid::Uid(0x10000079),TUid::Uid(0x100039e6));
    TBuf<16> languageLocaleDllName(KLitLanguageLocaleDllNameBase);  
    languageLocaleDllName.AppendFormat(ThreeDigExt, language);
    TBuf<16> regionLocaleDllName(KLitRegionLocaleDllNameBase);  
    TBuf<16> collationLocaleDllName(KLitCollationLocaleDllNameBase);  
    // Try to load the locale dll
    TInt error=localeDll.LoadLocale(languageLocaleDllName, regionLocaleDllName, collationLocaleDllName);
        
    if (error==KErrNotFound)
        {
        // Locale dll is not found for the asked language. 
        ERR_PRINTF2(_L("Failed to find the locale dll for %d"), language);
        }
           
    User::LeaveIfError(error);
    localeDll.SaveSystemSettings();
    
    #else
    _LIT(KLitLocaleDllNameBase, "ELOCL");
    _LIT(TwoDigExt,".%02d");
    
    RLibrary localeDll; 
    CleanupClosePushL(localeDll);
    
    const TUidType uidType(TUid::Uid(0x10000079),TUid::Uid(0x100039e6));
    TBuf<16> localeDllName(KLitLocaleDllNameBase);  
    localeDllName.AppendFormat(TwoDigExt, language);
    
    // Try to load the locale dll
    TInt error=localeDll.Load(localeDllName, uidType);
    if (error==KErrNotFound)
        {
        // Locale dll is not found for the asked language. 
        ERR_PRINTF2(_L("Failed to find the locale dll for %d"), language);
        }
    
    User::LeaveIfError(error);
    User::LeaveIfError(UserSvr::ChangeLocale(localeDllName));
    CleanupStack::PopAndDestroy(); // localeDll
    #endif
    
    // Check if the device locale has changed
    if (language == User::Language())
        {
        SetTestStepResult(EPass);
        }
    else
        {
        ERR_PRINTF3(_L("Failed to change the locale to %d whereas the current locale is"), language, User::Language());
        }

    return TestStepResult();
    }


/////////////////////////////////////////////////////////////////////
// CRegistryFilesStep - obtain a list of RegistryFiles
/////////////////////////////////////////////////////////////////////
CRegistryFilesStep::CRegistryFilesStep()
    {    
    }

TVerdict CRegistryFilesStep::doTestStepL()
    {
    TPtrC package;
    TPtrC vendor;
    TUid packageUid;
    TInt index;

    if(!GetStringFromConfig(ConfigSection(), KPackage, package))
        {
        ERR_PRINTF1(_L("Failed to find the package name in the ini"));
        SetTestStepResult(EFail);    
        return TestStepResult();
        }
    
    if(! GetStringFromConfig(ConfigSection(), KVendor, vendor))
        {
        ERR_PRINTF1(_L("Failed to find the vedor name in the ini"));
        SetTestStepResult(EFail);    
        return TestStepResult();
        }
   
    if(!GetIntFromConfig(ConfigSection(), KIndex, index))
        {
        ERR_PRINTF1(_L("Failed to find the registry index in the ini"));
        SetTestStepResult(EFail);    
        return TestStepResult();
        }
    
    if(!GetUidFromConfig(ConfigSection(), KUid, packageUid))
        {
        ERR_PRINTF1(_L("Failed to find the package Uid in the ini"));
        SetTestStepResult(EFail);    
        return TestStepResult();
        }        
    // Read the expected reg file name for the package uid and append it to the expected files array
    TPtrC regFileName;
    RPointerArray<HBufC> expectedFilesArray;
    CleanupResetAndDestroyPushL(expectedFilesArray);
    GetStringFromConfig(ConfigSection(), KRegFileName, regFileName);    
    expectedFilesArray.AppendL(regFileName.AllocLC());   
    CleanupStack::Pop();
    
    // Read the registry files for the package uid
    TInt ctlcount = 0;
    TPtrC ctlName;
    TInt expectedFileCount(0);
    GetIntFromConfig(ConfigSection(), KCtlCount, ctlcount);
    _LIT(KCtlName,"ctlName");
    
    for(TInt i=0 ; i < ctlcount ; i++)
        {            
            TBuf<20> integerAppendStr;
            TPtrC name;
            integerAppendStr.Format(_L("%S%d"), KCtlName().AllocLC(),i);            
            GetStringFromConfig(ConfigSection(), integerAppendStr, name);
            CleanupStack::PopAndDestroy();
            expectedFilesArray.AppendL(name.AllocLC());   
            CleanupStack::Pop();
        }    
    
    // Read the expected hash file name(2) for the package uid and append it to the expected files array
    TInt hashcount(0);
    _LIT(KHashFileName,"hashfilename");
    GetIntFromConfig(ConfigSection(), KHashCount, hashcount);
    for(TInt i=0 ; i < hashcount ; i++)
           {            
               TBuf<20> integerAppendStr;
               TPtrC hashFileName;
               integerAppendStr.Format(_L("%S%d"), KHashFileName().AllocLC(),i);            
               GetStringFromConfig(ConfigSection(), integerAppendStr, hashFileName);
               CleanupStack::PopAndDestroy();
               expectedFilesArray.AppendL(hashFileName.AllocLC()); 
               CleanupStack::Pop();
           }
            
    //Call the RegistryFiles API    
    RPointerArray<HBufC> obtainedRegistryFiles;
    CleanupResetAndDestroyPushL(obtainedRegistryFiles);  
    CSisRegistryPackage* registryPackage = NULL;
    Swi::RSisRegistryEntry entry;
    registryPackage = CSisRegistryPackage::NewLC(packageUid, package, vendor);
    registryPackage->SetIndex(index);
    
    //open the registry entry
    TRAPD(err, entry.OpenL(iSisRegistry,*registryPackage));    
    if(err != KErrNone)
        {
        SetTestStepResult(EFail); 
        User::Leave(err);
        }
    
    //Get the Regisrty files(.reg, .ctl and hash) for ythe registry entry
    TRAP(err, entry.RegistryFilesL(obtainedRegistryFiles));
    if(err != KErrNone)
        {
        SetTestStepResult(EFail); 
        User::Leave(err);
        }
    
    // Total no of expected files        
    expectedFileCount = ctlcount + hashcount + 1;     //ctl + hash + reg     
    
    if(obtainedRegistryFiles.Count() != expectedFileCount)  
        {
        ERR_PRINTF1(_L("Number of obtained files not same as the expected files"));
        SetTestStepResult(EFail);    
        }                
   
   //Compare the obtained Registry files with the expected files in the order(.reg, , .ctls and hash).  
   for (TInt i = 0 ; i < expectedFileCount ; i++)
       {      
       if (KErrNone != obtainedRegistryFiles[i]->Compare(*expectedFilesArray[i]))
           {                     
           ERR_PRINTF1(_L("Obtained Registry files does not match with the exptected Registry Files"));
           SetTestStepResult(EFail);    
           break;
           }
    }

    entry.Close();    
    CleanupStack::PopAndDestroy(registryPackage);  
    CleanupStack::Pop(&obtainedRegistryFiles);
    obtainedRegistryFiles.ResetAndDestroy();
    obtainedRegistryFiles.Close();    
    CleanupStack::Pop(&expectedFilesArray);
    expectedFilesArray.ResetAndDestroy();
    expectedFilesArray.Close();
    return TestStepResult();
    }

