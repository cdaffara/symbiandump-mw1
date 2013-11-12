// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// TestServer - shared client/server definitions
// 
//


#ifndef __PIM_TEST_CLIENTSERVER_H__
#define __PIM_TEST_CLIENTSERVER_H__


#include <e32std.h>

_LIT(KTestServerName,"testserver");
_LIT(KTestServerImg,"testserver");		// EXE name

enum TTestIpc
	{
	ETestIpcSetHomeTime,
	ETestIpcDeleteFile,
	ETestIpcKillProcess,
	ETestIpcChangeLocale,
	ETestIpcCheckForFile,
    ETestIpcCopyFile,
    ETestIpcCreateEmptyFile,
    ETestIpcSetTimeZone,
    ETestIpcGetTimeZone,
	ETestIpcDeleteDir,
	ETestIpcGetFileSize,
	ETestIpcNotifyChange,
	ETestIpcNotifyChangeCancel,
	ETestIpcCloseTzSession,
	ETestIpcChangeFSAttributes,
	ETestIpcDeleteFilesInDir,
	ETestIpcCopyDirAndFiles
	};

#endif
