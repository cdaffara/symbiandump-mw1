// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "testserver.h"

_LIT(KCntPrivatePath, "\\Private\\100012a5\\DBS_100065FF_");
_LIT(KAgnPrivatePath, "\\Private\\10003a5b\\");
_LIT(KLogPath, "c:\\logs\\");
_LIT(KSlash, "\\");
_LIT(KTempPath, "temp\\");
_LIT(KDefaultSecureAgendaFileName,"C:Calendar");

/* CTestRegister implementation
 */
EXPORT_C CTestRegister* CTestRegister::NewLC()
	{
	CTestRegister* self=new(ELeave) CTestRegister();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

EXPORT_C CTestRegister* CTestRegister::NewL()
	{
	CTestRegister* self = CTestRegister::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

void CTestRegister::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	User::LeaveIfError(iServer.Connect());
	}
	
CTestRegister::CTestRegister()
	{
	}

CTestRegister::~CTestRegister()
	{
	// Delete any temporary files:
	for(TInt i = 0; i < iFiles.Count(); ++i)
		{
		TRAP_IGNORE(iServer.DeleteFileL(iFiles[i]));
		}
	// Empty the file list:
	iFiles.Reset();
	iServer.Close();
	iFs.Close();
	}

TBool CTestRegister::FileNameEqual(const TFileName& aFirst,const TFileName& aSecond)
     {
     if (aFirst == aSecond)
         {
         return ETrue;
         }
     return EFalse;
     }

EXPORT_C void CTestRegister::RegisterL(const TDesC& aFileName, TTempFileType aFileType)
	{
	TFileName resolvedFileName;
	TIdentityRelation<TFileName> identityRelation(FileNameEqual);
	// Add the file to the list of files:
	switch(aFileType)
		{
	case EFileTypeUnknown:
		if (iFiles.Find(aFileName,identityRelation) == KErrNotFound)
		User::LeaveIfError(iFiles.Append(aFileName));
		break;
	case EFileTypeCnt:
	case EFileTypeAgn:
		resolvedFileName = ParseFilenameL(aFileName, aFileType); 
		if (iFiles.Find(resolvedFileName, identityRelation) == KErrNotFound) 
		User::LeaveIfError(iFiles.Append(resolvedFileName));
		break;
	default:
		User::Leave(KErrNotSupported);
		break;
		}
	}

EXPORT_C void CTestRegister::CreateLogFileLC(RFile& aFile, const TDesC& aFileName)
	{
	TFileName fullPath;
	GetLogPath(fullPath);
	fullPath.Append(aFileName);
	CreateFileLC(aFile, fullPath);
	}

EXPORT_C void CTestRegister::OpenLogFileLC(RFile& aFile, const TDesC& aFileName)
	{
	TFileName fullPath;
	GetLogPath(fullPath);
	fullPath.Append(aFileName);
	OpenFileLC(aFile, fullPath);
	}
	
EXPORT_C void CTestRegister::CreateTempFileLC(RFile& aFile, const TDesC& aFileName)
	{
	TFileName fullPath;
	GetTempPath(fullPath);
	fullPath.Append(aFileName);
	RegisterL(fullPath);
	CreateFileLC(aFile, fullPath);
	}

EXPORT_C void CTestRegister::OpenTempFileLC(RFile& aFile, const TDesC& aFileName)
	{
	TFileName fullPath;
	GetTempPath(fullPath);
	fullPath.Append(aFileName);
	OpenFileLC(aFile, fullPath);
	}
	
EXPORT_C void CTestRegister::GetLogPath(TDes& aPath)
	{
	aPath = KLogPath;
	RThread thread;
	TInt nameTerminator = thread.FullName().Locate('.');
	if(nameTerminator==KErrNotFound)
	    nameTerminator = thread.FullName().Locate('[');
	
	aPath.Append(thread.FullName().Left(nameTerminator));
	aPath.Append(KSlash);
	}

EXPORT_C void CTestRegister::GetTempPath(TDes& aPath)
	{
	GetLogPath(aPath);
	aPath.Append(KTempPath);
	}

void CTestRegister::CreateFileLC(RFile& aFile, const TDesC& aFileName)
	{
	iFs.MkDirAll(aFileName);
	User::LeaveIfError(aFile.Replace(iFs, aFileName, EFileWrite+EFileShareAny));
	CleanupClosePushL(aFile);
	}

void CTestRegister::OpenFileLC(RFile& aFile, const TDesC& aFileName)
	{
	User::LeaveIfError(aFile.Open(iFs, aFileName, EFileRead+EFileWrite+EFileShareAny));
	CleanupClosePushL(aFile);
	}

TFileName CTestRegister::ParseFilenameL(const TDesC& aFileNameDes, TTempFileType aFileType)
	{
	TFileName filename;
	if (aFileNameDes.Locate('\\')>=0)//there is a path included explicitly
		{
		User::Leave(KErrNotSupported);
		}
		
	const TDesC* path = &(KCntPrivatePath());
	if (aFileType == EFileTypeAgn)
		{
		path = &(KAgnPrivatePath());
		}

	const TInt len = aFileNameDes.Length();
	if (aFileNameDes.Length()>1 && aFileNameDes[1] == KDriveDelimiter)
		{
		filename = aFileNameDes.Left(2);
		filename.Append(*path);
		filename.Append(aFileNameDes.Right(len-2));
		}
	else
		{//default drive is c
		TDriveUnit driveUnit(EDriveC);
		filename = driveUnit.Name();
		filename.Append(*path);
		if (len!=0)
			{
			filename.Append(aFileNameDes);
			}
		else
			{
			if (aFileType == EFileTypeAgn)
				{
				filename.Append(KDefaultSecureAgendaFileName);
				}
			else
				User::Leave(KErrBadName);
			}
		}
	return filename;
	}

