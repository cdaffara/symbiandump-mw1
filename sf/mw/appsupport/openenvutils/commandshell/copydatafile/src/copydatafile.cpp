// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Name        : CopyDateFile.cpp
// Implements CopyDateFile
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, this 
// list of conditions and the following disclaimer. 
// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution. 
// Neither the name of the <ORGANIZATION> nor the names of its contributors 
// may be used to endorse or promote products derived from this software 
// without specific prior written permission. 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
//

#include "copydatafile.h"

/*
 * Copy the given file from C:\\ShellPublic\\$SID\\
 * to process private directory.
 */
  
TInt CopyDataFile::CopyToPrivateL(TDesC& aFileName)
{
	TInt ret=KErrNone;
	RFs iRFs;
	
	User::LeaveIfError(iRFs.Connect());	
	CFileMan* fileMan=CFileMan::NewL(iRFs);	
	
	//get the sid and convert it to hex
	TSecureId sid = RProcess().SecureId(); 
	TBuf<20> hex;	
	hex.Num(sid, EHex); 
	hex.Append(_L("\\"));
	
	//generate the source path - C:\\ShellPublic\\$SID
	TBuf<128> path(_L("C:\\ShellPub\\"));
	path.Append(hex);//make the source directory path
	
	//generate the absolute path of the source file
	TBuf<128> file (path);				
	file.Append(aFileName);
	
	//copy to process private directory
	ret=fileMan->Copy(file, _L("."));
		
	iRFs.Close();
	delete fileMan;
	
	return ret;
}

/*
 * Copy the given file from process  private directory
 * to C:\\ShellPublic\\$SID\\
 */
 
TInt CopyDataFile::CopyToPublicL(TDesC& aFileName)
{
	TInt ret=KErrNone;
	RFs iRFs;
	
	User::LeaveIfError(iRFs.Connect());	
	CFileMan* fileMan=CFileMan::NewL(iRFs);	
	
	//get the sid and convert it to hex
	TSecureId sid = RProcess().SecureId();
	TBuf<20> hex;	
	hex.Num(sid, EHex);
	hex.Append(_L("\\"));
	
	//generate the target path - C:\\ShellPublic\\$SID
	TBuf<128> path(_L("C:\\ShellPub\\"));
	path.Append(hex);
	
	//create the target directory
	TBuf<128> file (path);			
	ret=iRFs.MkDir(path);
	
	if(ret==KErrNone || ret==KErrAlreadyExists)
		{		
		//generate the absolute path of the target file
		file.Append(aFileName);
		//copy the file 
		ret=fileMan->Copy(aFileName, file);	
		}		
	
	iRFs.Close();
	delete fileMan;
	
	return ret;
}


