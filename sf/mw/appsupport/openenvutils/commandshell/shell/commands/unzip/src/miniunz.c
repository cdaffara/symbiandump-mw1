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


/*
   miniunz.c
   Version 1.01e, February 12th, 2005

   Copyright (C) 1998-2005 Gilles Vollant
   
*/

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unzip.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)


 
char* get_value(char* var, char** env_list);


/*
  mini unzip, demo of unzip package

  usage :
  Usage : miniunz [-exvlo] file.zip [file_to_extract] [-d extractdir]

  list the file in the zipfile, and print the content of FILE_ID.ZIP or README.TXT
    if it exists
*/

#if 0 // not supported currently
/* change_file_date : change the date/time of a file
    filename : the filename of the file where date/time must be modified
    dosdate : the new date at the MSDos format (4 bytes)
    tmu_date : the SAME new date at the tm_unz format */
void change_file_date(/*const*/ char *filename, uLong dosdate, tm_unz tmu_date)
{
#ifdef unix
  struct utimbuf ut;
  struct tm newdate;
  if (dosdate == 0) { /* do nothing. to avoid warnings */ };
  newdate.tm_sec = tmu_date.tm_sec;
  newdate.tm_min=tmu_date.tm_min;
  newdate.tm_hour=tmu_date.tm_hour;
  newdate.tm_mday=tmu_date.tm_mday;
  newdate.tm_mon=tmu_date.tm_mon;
  if (tmu_date.tm_year > 1900)
      newdate.tm_year=tmu_date.tm_year - 1900;
  else
      newdate.tm_year=tmu_date.tm_year ;
  newdate.tm_isdst=-1;

  ut.actime=ut.modtime=mktime(&newdate);
  utime(filename,&ut);
#endif
}

#endif

/* mymkdir and change_file_date are not 100 % portable
   As I don't know well Unix, I wait feedback for the unix portion */

int mymkdir(/*const*/ char* dirname) 
{
    int ret=0;
#ifdef unix
    ret = mkdir(dirname, 0775);
#endif
    return ret;
}

int makedir (char *newdir)
{
  char *buffer ;
  char *p;
  int  len = (int)strlen(newdir);

  if (len <= 0)
    return 0;

  buffer = (char*)malloc(len+1);
  strcpy(buffer,newdir);

  if (buffer[len-1] == '/') {
    buffer[len-1] = '\0';
  }
  if (mymkdir(buffer) == 0)
    {
      free(buffer);
      return 1;
    }

  p = buffer+1;
  while (1)
    {
      char hold;

      while(*p && *p != '\\' && *p != '/')
        p++;
      hold = *p;
      *p = 0;
      if (mymkdir(buffer) == -1)
        {
          printf("Couldn't create directory %s\n",buffer);
          free(buffer);
          return -1;
        }
      if (hold == 0)
        break;
      *p++ = hold;
    }
  free(buffer);
  return 1;
}



void do_banner()
{
    printf("MiniUnz 1.01b, demo of zLib + Unz package written by Gilles Vollant\n");
    printf("more info at http://www.winimage.com/zLibDll/unzip.html\n\n");
}

void do_help()
{
    printf("Usage : unzip [-e] [-x] [-l] [-o] file.zip [file_to_extr.] [-d extractdir]\n\n" \
           "  -e  Extract without pathname (junk paths)\n" \
           "  -x  Extract with pathname\n" \
           "  -l  list files\n" \
           "  -d  directory to extract into\n" \
           "  -o  overwrite files without prompting\n\n");    
}

int do_list(unzFile uf)
{
    uLong i;
    unz_global_info gi;
    int err;
    unz_file_info file_info;
    uLong ratio=0;
    //const 
    char *filename_inzip = (char*)ALLOC(256); //heap_alloc
    
    err = unzGetGlobalInfo (uf,&gi);
    if (err!=UNZ_OK)
        printf("error %d with zipfile in unzGetGlobalInfo \n",err);
   	printf(" Length  Size   Ratio  Date    Time   Name\n");
   	printf(" ------  ------ -----  ----    ----   ----\n");
    for (i=0;i<gi.number_entry;i++)
    {
        ratio = 0;
        err = unzlocal_GetCurrentFileInfoInternal(uf,&file_info, NULL,
        			filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);
        if (err != UNZ_OK)
        {
            printf("error %d with zipfile in unzlocal_GetCurrentFileInfoInternal\n", err);
            TRYFREE(filename_inzip);
            return err;
        }
        if (file_info.uncompressed_size>0) {
            ratio = (file_info.compressed_size*100)/file_info.uncompressed_size;
        }
        printf("%7lu  %7lu %3lu%%  %2.2lu-%2.2lu-%2.2lu  %2.2lu:%2.2lu  %s\n",        
                file_info.uncompressed_size,
                file_info.compressed_size,
                ratio,
                (uLong)file_info.tmu_date.tm_mon + 1,
                (uLong)file_info.tmu_date.tm_mday,
                (uLong)file_info.tmu_date.tm_year % 100,
                (uLong)file_info.tmu_date.tm_hour,(uLong)file_info.tmu_date.tm_min,
                filename_inzip);
        if ((i+1)<gi.number_entry)
        {
            err = unzGoToNextFile(uf);
            if (err!=UNZ_OK)
            {
                printf("error %d with zipfile in unzGoToNextFile\n",err);
                break;
            }
        }
    }
    TRYFREE(filename_inzip);
    return 0;
}


int do_extract_currentfile(unzFile uf, const int* popt_extract_without_path, 
							    int* popt_overwrite, const char* password)
{
	int err = UNZ_OK;
    char* filename_withoutpath = NULL;
    char* p = NULL;
    FILE *fout=NULL;
    void* buf = NULL;
    uInt size_buf;
    unz_file_info file_info;
    //const 
	char* write_filename;
    int skip=0;
    char rep=0;
    FILE* ftestexist;
    char answer[10]; //CHANGED to 10 from 128
    int ret;
    char *filename_inzip = (char*)ALLOC(256);     //char filename_inzip[256];  changed to heap_alloc    
    char c;
    
    err = unzlocal_GetCurrentFileInfoInternal(uf, &file_info, NULL,
    		filename_inzip, 
			256, NULL, 0, NULL, 0);  
    if (err != UNZ_OK)
        {
    	printf("Error %d with zipfile in unzGetCurrentFileInfo\n", err);
        return err;
        }
    size_buf = WRITEBUFFERSIZE;
    buf = (void*)malloc(size_buf);   
    if (buf == NULL)
    {
        printf("Error allocating memory\n");
        return UNZ_INTERNALERROR;
    }

    p = filename_withoutpath = filename_inzip;
    while ((*p) != '\0')
    {
        if (((*p)=='/') || ((*p)=='\\'))
            filename_withoutpath = p+1;
        p++;
    }
    if ((*filename_withoutpath)=='\0')
    { 
        if ((*popt_extract_without_path)==0)
        {
            printf("Creating directory: %s\n", filename_inzip);
            mymkdir(filename_inzip);
        }
    }
    else
    {
        if ((*popt_extract_without_path)==0)
            write_filename = filename_inzip;
        else
            write_filename = filename_withoutpath;
        err = unzOpenCurrentFilePassword(uf,password);
        if (err!=UNZ_OK)
        {
            printf("error %d with zipfile in unzOpenCurrentFilePassword\n",err);
        }

        if (((*popt_overwrite)==0) && (err==UNZ_OK))
        {
            ftestexist = fopen(write_filename,"rb");            
            if (ftestexist!=NULL)
            {
                fclose(ftestexist);
                do
                {
                    printf("The file %s exists. Overwrite ? [y]es, [n]o, [A]ll: ",write_filename);
                    fflush(NULL);
                    ret = scanf("%1s",answer);
                    if (ret != 1) {
                    #ifdef __SYMBIAN32__
                        if (buf) 
                    		free(buf);
					#endif//__SYMBIAN32__                        
                    	return -1;
                    }
                    rep = answer[0] ;
                    if ((rep>='a') && (rep<='z'))
                        rep -= 0x20;
                }
                while ((rep!='Y') && (rep!='N') && (rep!='A'));
            }

            if (rep == 'N')
                skip = 1;

            if (rep == 'A')
                *popt_overwrite=1;
        }
        if ((skip==0) && (err==UNZ_OK))
        {
            fout=fopen(write_filename,"wb");

            /* some zipfile don't contain directory alone before file */
            if ((fout==NULL) && ((*popt_extract_without_path)==0) &&
                                (filename_withoutpath!=(char*)filename_inzip))
            {
                c=*(filename_withoutpath-1);
                *(filename_withoutpath-1)='\0';
                makedir(write_filename);
                *(filename_withoutpath-1)=c;
                fout=fopen(write_filename,"wb");
            }
            if (fout==NULL)
            {
                printf("Error opening %s\n",write_filename);
            }
        }
        if (fout!=NULL)
        {
            printf("Extracting: %s\n",write_filename);

            do
            {
                err = unzReadCurrentFile(uf,buf,size_buf);
                if (err<0)
                {
                    printf("Error %d with zipfile in unzReadCurrentFile\n",err);
                    break;
                }
                if (err>0)
                    if (fwrite(buf,err,1,fout)!=1)
                    {
                        printf("Error in writing extracted file\n");
                        err=UNZ_ERRNO;
                        break;
                    }
            }
            while (err>0);
            if (fout)
                    fclose(fout);
#if 0 // not supported currently
            if (err==0)
                change_file_date(write_filename,file_info.dosDate,
                                 file_info.tmu_date);
#endif            
        }
        if (err==UNZ_OK)
        {
            err = unzCloseCurrentFile (uf);
            if (err!=UNZ_OK)
            {
                printf("Error %d with zipfile in unzCloseCurrentFile\n",err);
            }
        }
        else
            unzCloseCurrentFile(uf); /* don't lose the error */
    }

    if (buf) {
    	free(buf);
    }
    if (filename_inzip) {
    	free(filename_inzip);
    }     
    return err;
}


int do_extract(unzFile uf, int opt_extract_without_path, int opt_overwrite, const char* password)
{
    uLong i;
    unz_global_info gi;
    int err;

    err = unzGetGlobalInfo (uf,&gi);
    if (err != UNZ_OK) {
        printf("Error %d with zipfile in unzGetGlobalInfo \n", err);
        return -1;
    }
    for (i=0;i<gi.number_entry;i++)
    {
        if (do_extract_currentfile(uf,&opt_extract_without_path,
                                      &opt_overwrite,
                                      password) != UNZ_OK)
            break;
        if ((i+1)<gi.number_entry)
        {
            err = unzGoToNextFile(uf);
            if (err!=UNZ_OK)
            {
                printf("Error %d with zipfile in unzGoToNextFile\n",err);
                break;
            }
        }
    }

    return 0;
}

int do_extract_onefile(unzFile uf, const char* filename, int opt_extract_without_path,
					    int opt_overwrite, const char* password)
{
    if (unzLocateFile(uf,filename,CASESENSITIVITY)!=UNZ_OK)
    {
        printf("file %s not found in the zipfile\n",filename);
        return 2;
    }

    if (do_extract_currentfile(uf,&opt_extract_without_path,
                                      &opt_overwrite,
                                      password) == UNZ_OK)
        return 0;
    else
        return 1;
}



int main(int argc, char *argv[], char** env_list)
{
    //const 
    char *zipfilename=NULL;
    //const 
    char *filename_to_extract=NULL;
    //const 
    char *password=NULL;
    char *filename_try = (char*)ALLOC(MAXFILENAME+16);  // heap_alloc
    int i;
    int opt_do_list=0;
    int opt_do_extract=1;
    int opt_do_extract_withoutpath=0;
    int opt_overwrite=0;
    int opt_extractdir=0;
    //const 
    char *dirname=NULL;
    unzFile uf=NULL;
    char *p = NULL;
  	int err1 = 0;
  	
    if (argc==1)
    {
        do_help();
        free(filename_try);
        return 0;
    }
    else
    {
#ifdef __SYMBIAN32__  // <hack> changing directory to the working dir
	char* path = get_value("PWD", env_list); 
	if(path) {
		if (chdir(path) == -1) {
				printf("%s: %s\n", path, strerror(errno));
		        free(filename_try);
				return -1;
		}
	}
#endif //__SYMBIAN32__ 	// </hack>
        for (i=1;i<argc;i++)
        {
            if ((*argv[i])=='-')
            {
            	p = argv[i]+1;

                while ((*p)!='\0')
                {
                    char c=*(p++);;
                    if ((c=='l') || (c=='L'))
                        opt_do_list = 1;
                    if ((c=='v') || (c=='V'))
                        opt_do_list = 1;
                    if ((c=='x') || (c=='X'))
                        opt_do_extract = 1;
                    if ((c=='e') || (c=='E'))
                        opt_do_extract = opt_do_extract_withoutpath = 1;
                    if ((c=='o') || (c=='O'))
                        opt_overwrite=1;
                    if ((c=='d') || (c=='D'))
                    {
                        opt_extractdir=1;
                        dirname=argv[i+1];
                        i++;
                    }

                    if (((c=='p') || (c=='P')) && (i+1<argc))
                    {
                        password=argv[i+1];
                        i++;
                    }
                }
            }
            else
            {
                if (zipfilename == NULL)
                    zipfilename = argv[i];
                else if ((filename_to_extract==NULL) && (!opt_extractdir))
                        filename_to_extract = argv[i] ;
            }
        }
    }

    if (zipfilename!=NULL)
    {
        strncpy(filename_try, zipfilename,MAXFILENAME-1);
        /* strncpy doesnt append the trailing NULL, of the string is too long. */
        filename_try[ MAXFILENAME ] = '\0';
        uf = unzOpen(zipfilename);
    }
    if (uf==NULL)
    {
        printf("Cannot open %s\n", zipfilename );
        free(filename_try);
        return -1;
    }

		    if (opt_do_list==1) 
    {
    	free(filename_try);
        return do_list(uf); 
    }
    else if (opt_do_extract==1)
    {
        if (opt_extractdir && chdir(dirname)) 
        {
          printf("Error changing into %s, aborting\n", dirname);
          free(filename_try);
          return -1;
        }
      
        if (filename_to_extract == NULL)
        {
            err1 = do_extract(uf,opt_do_extract_withoutpath,opt_overwrite,password);
        }
        else
        {
        	err1 = do_extract_onefile(uf,filename_to_extract,
        				opt_do_extract_withoutpath,opt_overwrite,password);
        }
		free(filename_try);
		return err1;
    }
    unzCloseCurrentFile(uf);    
	free(filename_try);
    return 0;
}
