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
// process.cpp - Native implementation for collecting the information about process
// This file is part of ps command, the external command in Z shell.
// 
//

#include<e32std.h>
#ifdef __WINSCW__
#pragma warn_unusedarg off
#pragma warn_possunwant off
#endif//__WINSCW__

// Gets the state and priority of process aproc. Writes state into buffer
//pointed to by aprocstate and returns priority as an integer
	int getProcessState(RProcess& aproc,TDes& aprocstate)
		{
		TExitType exittype;
		TProcessPriority prio;
		exittype = aproc.ExitType();
		prio = aproc.Priority();
		switch(exittype)
			{
				case EExitPending:
					aprocstate.Append(_L("R"));
					break;
				case EExitPanic:
					aprocstate.Append(_L("P"));
					break;
				case EExitKill:
				case EExitTerminate:
					aprocstate.Append(_L("X"));
					return -1;
			}
			
		switch(prio)	
			{
			case EPriorityLow:
 			case EPriorityBackground:
			case EPriorityForeground:
				aprocstate.Append(_L("N"));
				break;
 
 			default:
 				// All other cases
				aprocstate.Append(_L("<"));
				break;
		}
		return prio;
		}

// Gets the CPU count of all threads in process aproc 
//and returns the hour minute second in the references passed
//If CPU count cant be obtained returns -1
	int getCpuCount(TFileName& apname,TDes& aResult)
	{
		TInt64 proctime=0;
		int res;
		RThread thrd;
		TTimeIntervalMicroSeconds cputime(0);
		TBool notime;
		TFileName aprocname(apname);
		aprocname.Append('*');
		TFindThread tfinder(aprocname);
		notime=false;
		while(1)
			{
			res = tfinder.Next(aprocname);
			if(res!=KErrNone)	
				{
				break;							
				}
			res=thrd.Open(tfinder);
			if(res!=KErrNone)
				{
				notime=true;
				continue;
				}
			res=thrd.GetCpuTime(cputime);
			if(res!=KErrNone)	
				{
				// GetCpuTime() not supported. 
				//kernel doesn't support this
				}
			proctime+=cputime.Int64();
			thrd.Close();
			}
		TTime time(proctime);
		TDateTime date=time.DateTime();
			
		if(date.Day()==0)
			{
			aResult.AppendFormat(_L(" %02d:%02d:%02d "),date.Hour(),date.Minute(),date.Second());
			}
		else
			{	
			aResult.AppendFormat(_L(" %d-%02d:%02d:%02d "),date.Day(),date.Hour(),date.Minute(),date.Second());
			}
		if(notime)
			{	
			aResult.Append(_L("?"));
			}
		if(!notime)
			return 0;
		else
			return -1;	
	}

// Gets memory information of the process aproc
	int getMemoryInfo(RProcess& aproc,TDes&  aResult)
		{
		
		int res=KErrNone;
		#ifdef __WINSCW__
		aResult.AppendFormat(_L("     ?           ?      "));
		#else
		TModuleMemoryInfo mem;
		res=aproc.GetMemoryInfo(mem);
		aResult.AppendFormat(_L("%-11u %-10u"),mem.iCodeBase,(mem.iCodeSize + mem.iInitialisedDataSize+mem.iConstDataSize + mem.iUninitialisedDataSize)/512);
		#endif
		return res;
		}
