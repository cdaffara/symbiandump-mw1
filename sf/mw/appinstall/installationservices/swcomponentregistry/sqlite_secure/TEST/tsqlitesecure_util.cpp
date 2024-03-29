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
//

#include <e32test.h>
#include <e32debug.h>
#include <f32file.h>
#include "tsqlitesecure_util.h"

static TBuf8<500> TheBuf8;
static TBuf16<500> TheBuf16;

static RTest TheTest(_L("tsqlitesecure_api test"));

extern "C" void TestTitle(void)
	{
	TheTest.Title();
	}

extern "C" void TestStart(const char* title)
	{
	TPtrC8 p((const unsigned char*)title);
	TheBuf16.Copy(p);
	TheTest.Start(TheBuf16);
	}

extern "C" void TestNext(const char* title)
	{
	TPtrC8 p((const unsigned char*)title);
	TheBuf16.Copy(p);
	TheTest.Next(TheBuf16);
	}

extern "C" void TestClose(void)
	{
	TheTest.Close();	
	}

extern "C" void TestEnd(void)
	{
	TheTest.End();	
	}

extern "C" void TestAbort(int aLine)
	{
	TheTest(0, aLine);	
	}

extern "C" void TestHeapMark(void)
	{
	__UHEAP_MARK;
	}

extern "C" void TestHeapMarkEnd(void)
	{
	__UHEAP_MARKEND;
	}

extern "C" void Print(const char* msg)
	{
	TPtrC8 p((const unsigned char*)msg);
	TheBuf16.Copy(p);
	TheTest.Printf(TheBuf16);
	}

extern "C" void PrintI(const char* fmt, int a1)
	{
	TPtrC8 p((const unsigned char*)fmt);
	TheBuf16.Copy(p);
	TheTest.Printf(TheBuf16, a1);
	}

extern "C" void PrintIII(const char* fmt, int a1, int a2, int a3)
	{
	TPtrC8 p((const unsigned char*)fmt);
	TheBuf16.Copy(p);
	TheTest.Printf(TheBuf16, a1, a2, a3);
	}

extern "C" void PrintII64I64(const char* fmt, int a1, __int64 a2, __int64 a3)
	{
	TPtrC8 p((const unsigned char*)fmt);
	TheBuf16.Copy(p);
	TheTest.Printf(TheBuf16, a1, a2, a3);
	}

extern "C" void PrintSI(const char* fmt, const char* s, int a1)
	{
	TPtrC8 p((const unsigned char*)fmt);
	TheBuf8.Format(TPtrC8((const TUint8*)fmt), s, a1);
	TheBuf16.Copy(TheBuf8);
	TheTest.Printf(TheBuf16);
	}

//The same as PrintSI() but uses RDebug::Print() instead of RTest::Printf(). RTest::Printf() is not thread safe.
extern "C" void ThreadPrintSSII(const char* fmt, const char* s1, const char* s2, int a1, int a2)
	{
	TPtrC8 p((const unsigned char*)fmt);
	TheBuf8.Format(TPtrC8((const TUint8*)fmt), s1, s2, a1, a2);
	TheBuf16.Copy(TheBuf8);
	RDebug::Print(TheBuf16);
	}

extern "C" void PrintS(const char* fmt, const char* s)
	{
	TPtrC8 p((const unsigned char*)fmt);
	TheBuf8.Format(TPtrC8((const TUint8*)fmt), s);
	TheBuf16.Copy(TheBuf8);
	TheTest.Printf(TheBuf16);
	}

//The same as PrintS() but uses RDebug::Print() instead of RTest::Printf(). RTest::Printf() is not thread safe.
extern "C" void ThreadPrintS(const char* fmt, const char* s)
	{
	TPtrC8 p((const unsigned char*)fmt);
	TheBuf8.Format(TPtrC8((const TUint8*)fmt), s);
	TheBuf16.Copy(TheBuf8);
	RDebug::Print(TheBuf16);
	}

extern "C" void CreatePrivateDir(void)
	{
	RFs fs;
	TInt err = fs.Connect();
	TheTest(err == KErrNone, __LINE__);
	err = fs.CreatePrivatePath(EDriveC);
	TheTest(err == KErrNone || err == KErrAlreadyExists, __LINE__);
	fs.Close();
	}
