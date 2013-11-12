// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __PIM_TEST_SERVER_H__
#define __PIM_TEST_SERVER_H__

#include <e32base.h>
#include <f32file.h>
#include <e32std.h>

class RPIMTestServer : public RSessionBase
	{
public:
	IMPORT_C TInt Connect();
	IMPORT_C TInt SetHomeTime(const TTime& aTime);
	IMPORT_C void DeleteFileL(const TDesC& aFileName);
	IMPORT_C void KillProcessL(const TDesC& aProcessName);
	IMPORT_C void ChangeLocaleNameL(const TDesC& aLocaleDllName);
	IMPORT_C TInt CheckForFile(const TDesC& aFileName);
    IMPORT_C void CopyFileL(const TDesC& aSource, const TDesC& aDest);
    IMPORT_C void CreateEmptyFileL(const TDesC& aFilename);
    IMPORT_C void GetTimeZoneL(TDes8& aTimeZoneName);
	IMPORT_C void DeleteDirL(const TDesC& aFileName);
	IMPORT_C TInt GetFileSizeL(const TDesC& aFileName);
	IMPORT_C void DeleteFilesInDirL(const TDesC& aDirName);
	IMPORT_C void CopyDirAndFilesL(const TDesC& aSource, const TDesC& aDest);

	/** Used to set the time zone in tests.
	Note that in Calendar tests (and any other tests using TZ), the time zone converter
	stored within the Calendar is not updated with the new time zone's rules until a
	callback is received from the TZ server.
	In order to make these tests work, the test must be a low priority active object. 
	Use a RunL to call each test step, and put the SetTimeZoneL method into a separate RunL call.
	Look at tcal_tz for an example.

	Real life systems will not have this problem because the TZ will be set from a completely 
	different thread to the Calendar.
	*/
    IMPORT_C void SetTimeZoneL(const TDesC8& aTimeZoneName);
    
    /**
     * Used to register change notification request.
     * Making use of allfiles capability in test server. 
     */
    IMPORT_C void FileServerNotifyChange(TNotifyType aType, TRequestStatus &aStat, const TDesC &aPathName);
    IMPORT_C void FileServerNotifyChangeCancel ();
	IMPORT_C void CloseTzSession();
	IMPORT_C void ChangeFSAttributesL(const TDesC& aPath, TUint aSetAttMask, TUint aClearAttMask);
	};


enum TTempFileType
	{
	EFileTypeUnknown,
	EFileTypeCnt,
	EFileTypeAgn
	};

/* This class handles the removal of temporary files and directories
 * after a test has completed.
 * The removal of files is performed during object deletion, so these
 * can be added to the cleanupstack to make sure.
 */
class CTestRegister : public CBase
	{
public:
	IMPORT_C static CTestRegister* NewLC();
	IMPORT_C static CTestRegister* NewL();
	~CTestRegister();
	IMPORT_C void RegisterL(const TDesC & aFileName,TTempFileType aFileType=EFileTypeUnknown);
	IMPORT_C void CreateLogFileLC(RFile& aFile, const TDesC& aFileName);
	IMPORT_C void OpenLogFileLC(RFile& aFile, const TDesC& aFileName);
	IMPORT_C void CreateTempFileLC(RFile& aFile, const TDesC& aFileName);
	IMPORT_C void OpenTempFileLC(RFile& aFile, const TDesC& aFileName);
	IMPORT_C void GetLogPath(TDes& aPath);
	IMPORT_C void GetTempPath(TDes& aPath);
	static TBool FileNameEqual(const TFileName& aFirst,const TFileName& aSecond);	
private:
	CTestRegister();
	void ConstructL();
	void CreateFileLC(RFile& aFile, const TDesC& aFileName);
	void OpenFileLC(RFile& aFile, const TDesC& aFileName);
	TFileName ParseFilenameL(const TDesC & aFileNameDes, TTempFileType aFileType);
private:
	RFs iFs;
	RPIMTestServer iServer;
	RArray<TBuf<KMaxFileName> > iFiles;
	};


#endif
