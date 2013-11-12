// Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// main.c - Has the main function for the ps command
// This file is part of ps command, the external command in Z shell.
//



#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include "process.h"


int eflag=0,lflag=0,fflag=0,oflag=0,sflag=0;
char pid[10] = "PID",args[10] = "COMMAND",comm[10] = "COMMAND",cputime[10] = "TIME";
int pidflag=0,argflag=0,timeflag=0,comflag=0;
TFindProcess pfinder;
TFindServer sfinder;
/* Print the headers */
void printheader()
	{
	if(oflag)
		{
		if(pidflag)
			{
			printf("%-5s",pid)	;						
			}
		if(timeflag)
			{
			printf("%-12s",cputime)	;					
			}
		if(argflag && comflag)
			{
			printf("%-35s  %s",comm,args)	;					
			}
		else if(argflag )
			{
			printf("%s",args)	;					
			}
		else if(comflag )
			{
			printf("%s",comm)	;						
			}
			printf("\n");
		}
			
	else if(lflag)
		{
		printf(" S PID  PRI     ADDR        SZ       TIME     CMD\n");					
		}
			
	else
		{
		printf(" PID     TIME      CMD\n");	
		}	
	}

/* Prints the result in a multicolumnar format*/
void printresult(TDes& aResult)
	{

	//print the information
	wprintf(L"%ls\n",(wchar_t*)aResult.PtrZ());
	fflush(stdout);
	}
	
// Function to parse the arguments passed to ps -o option
// Syntax:ps -o "pid=PID1",args -o "time=CPUTIME",comm
void AddOption(char *options)
	{
	int n,i;
	char *str;
	char *optarr[4] = {0};
	str = strtok(options,",");
	for(n=0;str!=NULL && n<4;n++)
		{			
		optarr[n] = str;
		str = strtok(NULL,",");
		
		}
	for(i=0;i<n;i++)
		{
		char *option;
		char *value = NULL;
		option = strtok(optarr[i],"=");
		TBool validOption = EFalse;
		if(option)
			{
			if(!strcasecmp(option,"pid"))
				{
				pidflag=1;
				value = strtok(NULL,"=");
				validOption = ETrue;
				if( value )
					{
					strncpy(pid,value,9);	
					}
			
				
				}
			
			 if(!strcasecmp(option,"args"))
				{
				argflag=1;
				validOption = ETrue;
				value = strtok(NULL,"=");
				if( value )
					{
					strncpy(args,value,9);	
					}
				
				}
				
			 if(!strcasecmp(option,"comm"))
				{
				comflag=1;
				validOption = ETrue;
				value = strtok(NULL,"=");
				if( value )
					{
					strncpy(comm,value,9);	
					}
				
				}
				
			 if(!strcasecmp(option,"time"))
				{
				timeflag=1;
				validOption = ETrue;
				value = strtok(NULL,"=");
				if( value )
					{
					strncpy(cputime,value,9);	
					}
				
				}
			 // This is to check if there is any invalid colum option in ps -o
			 if( !validOption )
				 {
				 printf("error: Unknown user defined format specifier\n");
				 exit(1);
				 }
			}
				
		}
	
	}
						

int main(int argc, char* argv[])
	{
	int res;
	const char* optstr = "fhlo:s";
	int procprio;
	int notime;
	int c,rem=0;
	TBuf<5> procstate;
	RProcess proc;
	TFileName procname;
	_LIT(KHelp,"-f\tGenerate a full listing.\n-h\tDisplay a brief help message\n-l\tGenerate a long listing.\n-o\t Customise output.\n-s\tList the servers running as some process\n");
	
	RBuf Result;
	c = Result.Create(75);
	if(c!=KErrNone)
		{
		printf("Error")	;
		exit(1);
		}
	while ((c = getopt(argc, argv,optstr)) != -1) 
		{
	    switch(c)
	    	{
		    case 'f':
		    	fflag=1;
		        break;
	    	case 'h':
	    		wprintf((wchar_t*)((KHelp()).Ptr()));
	    		exit(0);
		    case 'l':
		    	lflag=1;
		        break;
		    case 'o':
		    	oflag=1;
		    	AddOption(optarg);
		    	break;
	    	case 's':
	    		sflag=1;
	    		break;
		    case ':':
		        printf("error: -%c specified without option\n", optopt);
		        exit(1);
		    case '?':
		        printf("error: Unknown argument - %c\n", optopt);
		        exit(1);
	    	}
		}
	
	if((lflag && oflag) || (fflag && oflag))
		{
		printf("error: Conflicting Options\n");
		exit(1);
		}	
	printheader();	
	while(1)
		{
		res = pfinder.Next(procname);
		if(res != KErrNone)
		break;
		res = proc.Open(pfinder);
		if(res != KErrNone)
		break;
		
		procstate.Zero();
		procprio = getProcessState(proc,procstate);
		if(procprio==-1)
			{
			proc.Close();
			continue;	
			}
		
		Result.Zero();
			
		if(!oflag) //for long,full and default listing
			{
			if(lflag)
				{
				Result.AppendFormat(_L("%-5s"),procstate.PtrZ());
				}
	
			Result.AppendFormat(_L("%-5d"),(TUint)proc.Id());
			if(lflag)
				{
				Result.AppendFormat(_L("%-4d"),procprio);
				res = getMemoryInfo(proc,Result);
				if(res!=KErrNone)
					{
					proc.Close();
					continue;	
					}
				}
			notime = getCpuCount(procname,Result);
		
			rem = Result.MaxLength() - Result.Length();
			if((fflag))
				{
				if(rem<=(proc.FileName()).Length()+5)
				    {
					c = Result.ReAlloc((proc.FileName()).Length()+Result.MaxLength());
					if(c!=KErrNone)
					    {
					    printf("Error") ;
					    exit(1);
					    }                   
				    }
				Result.AppendFormat(_L(" [%s]"),(proc.FileName()).PtrZ());
				}
			else 
				{
				if(rem<=procname.Length())
				    {
					c = Result.ReAlloc(procname.Length()+Result.MaxLength());
					if(c!=KErrNone)
					    {
					    printf("Error") ;
					    exit(1);
					    }					    
				    }
				Result.Append(procname);		
				}
				
					
			}
		else   //info for oflag : Output can be customised to show some fields
			{
			if(pidflag)
				{
				Result.AppendFormat(_L("%-5d"),(TUint)proc.Id());	
				}
			if(timeflag)
				{
				notime = getCpuCount(procname,Result);
							
				}
			rem = Result.MaxLength() - Result.Length();
			
			if(argflag && comflag)
				{
				if(rem<=(proc.FileName()).Length()+5)
					Result.ReAlloc((proc.FileName()).Length()+Result.MaxLength());
				Result.AppendFormat(_L(" [%-35s] ?"),(proc.FileName()).PtrZ());	
				}
			else if (comflag)
				{
				if(rem<(proc.FileName()).Length())
					Result.ReAlloc((proc.FileName()).Length()+Result.MaxLength());
				Result.Append(proc.FileName());
				}
			else if (argflag)
				{
				if(rem<=(proc.FileName()).Length()+5)
					Result.ReAlloc((proc.FileName()).Length()+Result.MaxLength());
				Result.AppendFormat(_L(" [%s]"),(proc.FileName()).PtrZ());	
				}
			
			}	
		proc.Close();
		printresult(Result);
		}
	if(sflag)
		{
		Result.Zero();
		printf("Servers\n");
		while(1)
			{
			res = sfinder.Next(procname);
			if(res != KErrNone)
				break;
			printresult(procname);
			}
		
		}
	Result.Close();
	return 0;		
	}
