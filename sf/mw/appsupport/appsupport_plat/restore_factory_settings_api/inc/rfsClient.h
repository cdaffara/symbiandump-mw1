/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*     This is a client class used for performing Restore Factory Settings
*     operations.
*
*
*/


#ifndef RFS_CLIENT_H
#define RFS_CLIENT_H

// INCLUDES
#include <e32base.h>
#include "rfsHandler.h"

// CONSTANTS

// client panic reasons

enum TRfsPanic
    {
    ERfsBadRequest = 0,
    ERfsScriptFileNotUnicode = 1
    };

_LIT( KRfsPanicCategory, "RFS" );

// CLASS DECLARATION

/**
* RRfsClient API
*
* Restory factory settings functionality is used via RRfsClient class. First,
* the user has to connect to the server by calling Connect() member function.
* After this the client methods are available for use. When finished, the
* session must be closed with Close() member function.
*
* Example code:
*   
*   #include <RfsClient.h>   // link against rfs.lib
*
*   RRfsClient rfs;
*   User::LeaveIfError( rfs.Connect() );  // open a client session
*
*   rfs.PerformRfs( ERfsNormal );         // perform normal restore factory settings
*   
*   rfs.Close();                          // close the client session
*
* ------------------------------------------------------------------------------
*           Rfs Script Format:
* ------------------------------------------------------------------------------
*
* Normal rfs and deep rfs operations can be done simply with single client
* requests ( PerformRfs()-method ). It is also possible to specify a special rfs
* script that is used to achieve the desired operation. The script can be in a
* file or in a descriptor and it consists of RFS commands separated by line
* ends. There must not be more than one command per line. Extra white space is
* allowed in lines. The setting_name parameter in SD-command is case-sensitive
* due to fact that SharedData keywords are too, but everything else is case-
* insensitive. "#" or "//" can be used for commenting out the rest of the line
* but the comment characters need to be in the beginning of the line or there
* must be white space between the last parameter and the comment. C-style
* commenting is not supported. Wildcards (?, *) can be used in all file/path
* names. Any parameter can be enclosed with quotation marks for allowing spaces
* in file names.
*
* NOTE! A file that contains an rfs script must be saved in UNICODE format!
*
* List of rfs commands: (mandatory parameters in <>, optional parameters in [])
*
* ------------------------------------------------------------------------------ 
* CD <path>
* Changes the default path. A full path including a drive letter must be given
* and there MUST be a trailing "\" in the end of the path. If full file names
* are not given in target parameters of other RFS commands, they are assumed to
* be relative to the current default path. This command is added for reducing
* the sizes of RFS scripts. Note that the source file parameter in CP command
* has to contain a full path always.
* ------------------------------------------------------------------------------
* DEL <filename>
* Deletes the file(s). File name has to contain full path. Can't delete a
* directory with this. Wildcards can be used in the filename.
* ------------------------------------------------------------------------------
* RD <directoryname>
* Removes the directory and the whole directory structure under it, recursively.
* All files in directories are deleted, too. Directory name has to contain full
* path. NOTE! There MUST be a trailing "\" in the end of the path.
* ------------------------------------------------------------------------------ 
* MD <directoryname>
* Creates the directory structure given as a parameter. Directory name has to
* contain full path and there MUST be a trailing "\" in the end of the path.
* ------------------------------------------------------------------------------ 
* CP <source_filename> [destination_filename]
* Copies the source file to the destination file. Source file name has to
* contain full path. Destination file name, if given, can be relative to the
* default path given with CD command. This command creates the target directory
* if it doesn't exist and it can be used to copy whole directory structures
* recursively. Wildcards can be used. If the destination file name is not given,
* the same file name as in source file(s) will be used, but the destination
* drive is changed to C: ( useful when copying from Z: )
* ------------------------------------------------------------------------------
* SD <uid> [setting_name1] [setting_name2] [setting_name3] ...
* Restores the setting(s) stored in Series 60 shared data files to the original
* values that are stored in ROM. Uid of the shared data file has to consist
* exatly from 8 hex numbers. If the setting_name parameters are given, only
* those settings are restored, otherwise the whole shared data file is restored.
* ------------------------------------------------------------------------------
* PLUGINS [plugin1] [plugin2] [plugin3] ...
*
* ------------------------------------------------------------------------------
* DOS <level>
* Restores all settings located in DOS side. Normal RFS level is 0, deep RFS 
* level is 1.
* ------------------------------------------------------------------------------
* FORMAT <drive>
* Formats the whole drive erasing all contents. Drive definition must be exactly
* the drive letter followed by a colon.
* ------------------------------------------------------------------------------
*
* Example script: (all commands are in the correct format)
*
* CD C:\system\data\
* // changes the default directory, notice the trailing '\'
*
* DEL myfile.ini
* // deletes a file relative to default path
* // (target is C:\system\data\myfile.ini)
*
* DEL C:\system\data\apps\myapp\settings.dat
* // deletes a file
*
* DEL "C:\system\data\apps\myapp\other setting with long file name.dat"
* // deletes a file
*
* DEL C:\system\data\apps\myapp\*
* // deletes recursively everything under directory C:\system\data\apps\myapp\
*
* RD C:\system\data\apps\myapp\
* // removes directory and its contents
*
* MD C:\system\testdata\
* // creates directory, notice the trailing '\'
*
* CP Z:\system\backup\mysetting.dat C:\system\data\mysetting.dat
* // copies a file to the specified target location
*
* CP Z:\system\backup\mydir\* C:\system\data\mydir\
* // copies recursively everything under Z:\system\backup\mydir\
* // to C:\system\data\mydir\
* 
* CP Z:\system\data\mysetting.dat
* // copies Z:\system\data\mysetting.dat to C:\system\data\mysetting.dat
*
* CP Z:\system\data\mydir\*
* // copies recursively everything under Z:\system\data\mydir\
* // to C:\system\data\mydir\
*
* SD 100059a4
* // Restores the whole shared data file which has the uid 0x100059a4.
* // this is done by deleting the possible shared data file in C: after which
* // the default file in Z: will be automatically used by shared data server.
*
* SD 100059a4 VmbxNumber
* // Restores the setting "VmbxNumber" in a shared data file 100059a4.ini.
* // Default value is taken from a default shared data file in ROM.
*
* FORMAT C:
* // formats the drive C: erasing all contents
*
*/

class RRfsClient : public RSessionBase
    {
    public: // New functions        

        /**
        * Constructor.
        */      
        IMPORT_C RRfsClient();

        /**
        * Starts the server if it's not already running. Creates a new session.
        * If a session is already established by the client, does nothing.
        *
        * @return Symbian OS standard error code
        */      
        IMPORT_C TInt Connect();

        /**
        * Performs normal or deep level restore factory settings operation.
        * This is done by running an rfs script that is stored in ROM.
        * ("Z:\system\rfs\script0.txt" for normal rfs and
        *  "Z:\system\rfs\script1.txt" for deep rfs)
        * 
        * @param aType type of the rfs operation (normal or deep level)
        *
        * @param aIncludeNosRfs if ETrue, DOS side is called to perform
        *        DOS RFS, otherwise not.
        *
        * @return KErrGeneral: Syntax error in the RFS script.
        *         KErrNotFound or KErrPathNotFound: The script was not found.
        *         KErrNoMemory: no memory for running the RFS script.
        *
        *         Otherwise, number of unsuccessful commands in executed script.
        */      
        IMPORT_C TInt PerformRfs( TRfsType aType, TBool aIncludeNosRfs = ETrue );

        /**
        * Performs those RFS operations that need the presence of UIKON
        * environment. UIKON server must be started before calling this.
        *
        * @param aType type of the rfs operation (normal or deep level)
        *
        * @return Standard Symbian OS error code.
        */
        IMPORT_C TInt PerformPostEikonRfs( TRfsType aType );

        /**
        * Runs the given rfs script.
        * 
        * @param aScript rfs script in a descriptor in the correct format.
        *        If there is a format error somewhere in the script, the whole
        *        script is ignored and an error code is returned.
        *
        * @return KErrGeneral: syntax error in the RFS script.
        *         KErrNoMemory: no memory for running the RFS script.
        *
        *         Otherwise, number of unsuccessful commands in executed script.
        */      
        IMPORT_C TInt RunScriptInDescriptor( const TDesC& aScript );

        /**
        * Runs the given rfs script.
        * 
        * @param aFileName filename of the rfs script
        *
        * @return KErrGeneral: Syntax error in the RFS script.
        *         KErrNotFound or KErrPathNotFound: The script was not found.
        *         KErrNoMemory: no memory for running the RFS script.
        *
        *         Otherwise, number of unsuccessful commands in executed script.  
        */      
        IMPORT_C TInt RunScriptInFile( const TDesC& aFileName );
        
        /**
        * Closes the session to the server.
        * Remember to close all connected sessions!
        */
        IMPORT_C void Close();

    private: // New functions

        /**
        * Gives the client side code version.
        *
        * @return version
        */      
        TVersion Version() const;

        // Prohibit copy constructor
        RRfsClient( const RRfsClient& );

        // Prohibit assigment operator
        RRfsClient& operator= ( const RRfsClient& );

    };

#endif      // RFS_CLIENT_H
