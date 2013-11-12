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
// process.h - Declarations for all the functions supported
// This file is part of ps command, the external command in Z shell.
// 
//

#include<e32std.h>


 

 // Gets the state and priority of process aproc. Appends state into buffer
//pointed to by aprocstate and returns priority as an integer
 extern int getProcessState(RProcess& aProc,TDes& aprocstate);
 
 // Gets the CPU count of all threads in process aproc 
//and appends the hour,min,and sec to aResult
//If CPU count cant be obtained for some threads returns -1
 extern int getCpuCount(TFileName& aprocname,TDes& aResult); 
 
 // Gets memory information of the process aproc
 extern int getMemoryInfo(RProcess& aProc,TDes& aResult);
 
 