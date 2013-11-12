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


/* ioapi.c -- IO base function header for compress/uncompress .zip
   files using zlib + zip or unzip API

   Version 1.01e, February 12th, 2005

  
   Copyright (C) 1998-2005 Gilles Vollant

*/
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//When OE- zlib is used, change this to zlib.h
#ifdef _USE_OE_ZLIB_
#include <zlib.h>
#else
#include <ezlib.h>
#endif

#include "ioapi.h"



#ifndef SEEK_CUR
#define SEEK_CUR    1
#endif

#ifndef SEEK_END
#define SEEK_END    2
#endif

#ifndef SEEK_SET
#define SEEK_SET    0
#endif


voidpf ZCALLBACK fopen_file_func (voidpf opaque, const char* filename, int mode)
{
    FILE* file = NULL;
    const char* mode_fopen = NULL;
    if (opaque == NULL) { /* do nothing. to avoid warnings */ };
    if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER)==ZLIB_FILEFUNC_MODE_READ)
        mode_fopen = "rb";
    else
    if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
        mode_fopen = "r+b";
    else
    if (mode & ZLIB_FILEFUNC_MODE_CREATE)
        mode_fopen = "wb";

    if ((filename!=NULL) && (mode_fopen != NULL)) {
    	//printf("IOAPI: Opening file %s\n", filename);
    	file = fopen(filename, mode_fopen);
    }        
    return file;
}


uLong ZCALLBACK fread_file_func (voidpf opaque, voidpf stream, void* buf, uLong size)
{
    uLong ret;
    if (opaque == NULL) { /* do nothing. to avoid warnings */ };
    ret = (uLong)fread(buf, 1, (size_t)size, (FILE *)stream);
    return ret;
}


uLong ZCALLBACK fwrite_file_func (voidpf opaque, voidpf stream, const void* buf, uLong size)
{
    uLong ret;
    if (opaque == NULL) { /* do nothing. to avoid warnings */ };
    ret = (uLong)fwrite(buf, 1, (size_t)size, (FILE *)stream);
    return ret;
}

long ZCALLBACK ftell_file_func (voidpf opaque, voidpf stream)
{
    long ret;
    if (opaque == NULL) { /* do nothing. to avoid warnings */ };
    ret = ftell((FILE *)stream);
    return ret;
}

long ZCALLBACK fseek_file_func (voidpf opaque, voidpf stream, uLong offset, int origin)
{
    int fseek_origin=0;
    long ret;
    if (opaque == NULL) { /* do nothing. to avoid warnings */ };
    switch (origin)
    {
    case ZLIB_FILEFUNC_SEEK_CUR :
        fseek_origin = SEEK_CUR;
        break;
    case ZLIB_FILEFUNC_SEEK_END :
        fseek_origin = SEEK_END;
        break;
    case ZLIB_FILEFUNC_SEEK_SET :
        fseek_origin = SEEK_SET;
        break;
    default: return -1;
    }
    ret = 0;
    fseek((FILE *)stream, offset, fseek_origin);
    return ret;
}

int ZCALLBACK fclose_file_func (voidpf opaque, voidpf stream)
{
    int ret;
    if (opaque == NULL) { /* do nothing. to avoid warnings */ };
    ret = fclose((FILE *)stream);
    return ret;
}

int ZCALLBACK ferror_file_func (voidpf opaque, voidpf stream)
{
    int ret;
    if (opaque == NULL) { /* do nothing. to avoid warnings */ };
    ret = ferror((FILE *)stream);
    return ret;
}

void fill_fopen_filefunc (zlib_filefunc_def* pzlib_filefunc_def)
{
    pzlib_filefunc_def->zopen_file = fopen_file_func;
    pzlib_filefunc_def->zread_file = fread_file_func;
    pzlib_filefunc_def->zwrite_file = fwrite_file_func;
    pzlib_filefunc_def->ztell_file = ftell_file_func;
    pzlib_filefunc_def->zseek_file = fseek_file_func;
    pzlib_filefunc_def->zclose_file = fclose_file_func;
    pzlib_filefunc_def->zerror_file = ferror_file_func;
    pzlib_filefunc_def->opaque = NULL;
}


//get the value for the given var from the env list...
char* get_value(char* var, char** env_list)
{
	char* pwd = NULL;
	size_t idx=0;
	
	if(!var || !env_list)
		return NULL;
	
	while(env_list[idx])
		{
		char* temp=env_list[idx];
		if(strstr(temp, var))
			{
			while(*temp!= '='&& temp++) {};			
			if(temp++)
				pwd=temp;
			break;
			}
		idx++;	
		}	
	return pwd; 	
}	
