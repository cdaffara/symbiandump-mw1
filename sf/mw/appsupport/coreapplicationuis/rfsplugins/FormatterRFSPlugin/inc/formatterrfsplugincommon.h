/*
* Copyright (c) 2009-10 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Common defines for FormatterRFSPlugin
*
*/

// Constants for constructing exclude list name
/**
* Exclude list path name extension
*/
const TInt KExcludeListPathNameLenExt( 2 );

/**
* Exclude list path and file
*/
_LIT( KExcludeList, "\\private\\100059C9\\excludelist.txt" );

/**
* Application exclude list path
*/
_LIT( KApplicationExcludeListPath, "?:\\private\\102073ea\\excludes\\" );

/**
* Application exclude list file search pattern
*/
_LIT( KApplicationExcludeListFileSearchPattern, "*.exc" );

/**
* Uid separator
*/
_LIT( KEmptyParameter, "-" );

/**
* ROM drive
*/
_LIT( KDefaultRom, "Z" );

// Constants used to run Secure Formatter
/**
* Secure formatter argument extension length
*/
const TInt KSecureFormatterArgLenExt( 4 );

/**
* Base error code.
*/
const TInt KErrBase = 1000;

/**
* Error code for invalid exclude list.
*/
const TInt KErrInvalidExcList = KErrBase;

/**
* Secure formatter argument format
*/
_LIT( KSecureFormatterArgFmt, "%S %c%c%c" );

/**
* Secure formatter
*/
_LIT( KSecureFormatter, "secureformatter.exe" );

// Directories to create after format
/**
* Central repository path
*/
_LIT( KCenRepPrivatePath, "\\Private\\10202be9\\" );

/**
* Central repository persists path
*/
_LIT( KCenRepPersistsPath, "\\Private\\10202be9\\persists\\" );

/**
* Central repository bur path
*/
_LIT( KCenRepBurPath, "\\Private\\10202be9\\bur\\" );

// Constants used in RFS script
/**
* Hexadecimal length
*/
const TInt KHexLength( 8 );

/**
* Key separator
*/
_LIT( KScriptKeySeparator, ":" );

/**
* Uid separator
*/
_LIT( KScriptUidSeparator, ";" );
