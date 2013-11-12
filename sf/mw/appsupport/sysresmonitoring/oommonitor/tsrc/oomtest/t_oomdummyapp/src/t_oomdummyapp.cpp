/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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




// INCLUDE FILES
#include <eikstart.h>
#include "t_oomdummyappApplication.h"

_LIT(KFakeUidParam,"uid=");
_LIT(KAllocParam,"alloc=");
LOCAL_C CApaApplication* NewApplication()
	{
	TUid uid(KUidt_oomdummyappApp);
	TUint alloc = 0;
	//override uid from command line (add "uid=6789ABCD" anywhere)
	TInt cmdsize = User::CommandLineLength();
	HBufC *cmdbuf = NULL;
	if(cmdsize > 0) cmdbuf = HBufC::New(cmdsize);
	if(cmdbuf)
		{
		TPtr cmdline(cmdbuf->Des());
		User::CommandLine(cmdline);
		TLex lex(*cmdbuf);
		while(!lex.Eos())
			{
			lex.SkipSpaceAndMark();
			while(!lex.Eos() && lex.Get() != '=')
				{
				}
			if(lex.MarkedToken().CompareF(KFakeUidParam) == 0)
				{
				lex.SkipSpaceAndMark();
				while(!lex.Eos() && lex.Get().IsHexDigit())
					{
					}
				TUint id;
				TLex num(lex.MarkedToken());
				if(KErrNone==num.Val(id,EHex))
					{
					RDebug::Printf("\tapp uid override %x", id);
					uid = TUid::Uid(id);
					}
				}
			else if(lex.MarkedToken().CompareF(KAllocParam) == 0)
				{
				lex.SkipSpaceAndMark();
				while(!lex.Eos() && lex.Get().IsHexDigit())
					{
					}
				TLex num(lex.MarkedToken());
				if(KErrNone!=num.Val(alloc,EHex))
					{
					alloc = 0;
					}
				}
			}
		/*
		TInt offset = cmdbuf->Find(KFakeUidParam);
		if(offset >=0 && offset + 12 <= cmdbuf->Length())
			{
			TLex lex(cmdbuf->Mid(offset+4,8));
			TUint id;
			if(KErrNone==lex.Val(id,EHex))
				{
				RDebug::Printf("\tapp uid override %x", id);
				uid = TUid::Uid(id);
				}
			}
		TInt offset = cmdbuf->Find(KAllocParam);
		if(offset >=0 && offset + 12 <= cmdbuf->Length())*/
		delete cmdbuf;
		}
	return new Ct_oomdummyappApplication(uid, alloc);
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}

