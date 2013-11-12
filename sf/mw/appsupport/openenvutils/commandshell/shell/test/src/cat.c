/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include <stdio.h>

/**
* External command cat to display the contents of a file
*/
int main(int *argc, char *argv[])
{
	int ret=0;
	int c;
	FILE *fp;
	printf("Cat external command\n");
	fp = fopen(*(argv+1), "r");
	if(!fp)
		{
		printf("File %s not found\n",*(argv+1));
		return 1;
		}
	while((c=fgetc(fp)) != EOF)
	{
		putchar(c);
	}
	fclose(fp);
	return 0;
}
