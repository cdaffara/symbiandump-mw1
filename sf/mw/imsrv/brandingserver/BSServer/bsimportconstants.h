/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   BrandingServer constants.
*
*/

// CONSTANTS

// Characters to strip from xml text values
_LIT( KCommonControlCharacters, "\n\t\r" );
_LIT( KEndl,                    "\n\r" );

// === Directory & file constants =============================================
_LIT( KDirSeparator,        "\\" );
_LIT( KDot,        			"." );
_LIT( KDefFileName,         "brandfile.bin.r" );
_LIT( KDefEventLogDir,      "BSImport" );
_LIT( KDefEventLogFile,     "eventlog.txt" );
_LIT( KFileElementStore,    "files" );
_LIT( KBSDataStore,    		"data" );
_LIT( KAllFilesWildCard,    "*.*" );
_LIT( KBSCDrive,			"C:" );
_LIT( KBSZDrive,			"Z:" );
// Codescanner warning : hard-coded drive letters (id:25)
// this folder is always on c-drive
_LIT( KBSIbyDirectory,		"c:\\data\\bs\\" ); // CSI: 25 # See above
_LIT( KBSIbyExtension, 		".iby" );
_LIT( KBSFileLangSuffix,    "r" );
const TInt KMaxVersionLenght = 3;

const TInt KLeadingZero = 0;
const TInt KLangBufLength = 2;

// folders inside server's private: "\\private\\102828DD\\"
_LIT( KInstallObservePath,      "import\\install\\" ); // new files
_LIT( KInstallPath,             "install\\" );         // installed files

// drive where installation files are saved
_LIT( KInstallDrive,            "C:" );

// brand installation file extension
_LIT( KBrandInstallExt,         ".install" );

// Separator which separates brand id from application id in
// uninstall buffer
_LIT( KInstallFileDataSeparator, "$" );

_LIT( KDiscardBrandFileName,         "discardedbrand.txt" );

 




