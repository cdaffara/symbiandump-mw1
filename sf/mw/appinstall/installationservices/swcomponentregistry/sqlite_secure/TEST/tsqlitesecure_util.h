/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef SQLITESECURE_UTIL_H
#define SQLITESECURE_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

void TestTitle(void);
void TestStart(const char* title);
void TestNext(const char* title);
void TestClose(void);
void TestEnd(void);
void TestAbort(TInt aLine);

void TestHeapMark(void);
void TestHeapMarkEnd(void);

void Print(const char* msg);
void PrintI(const char* fmt, int a1);
void PrintIII(const char* fmt, int a1, int a2, int a3);
void PrintII64I64(const char* fmt, int a1, __int64 a2, __int64 a3);
void PrintSI(const char* fmt, const char* s, int a1);
void ThreadPrintSSII(const char* fmt, const char* s1, const char* s2, int a1, int a2);
void PrintS(const char* fmt, const char* s);
void ThreadPrintS(const char* fmt, const char* s);

void CreatePrivateDir(void);

#ifdef __cplusplus
}
#endif

#endif
