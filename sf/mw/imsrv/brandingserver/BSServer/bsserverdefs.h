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
* Description:  server defintions.
*
*/

#ifndef __BSSERVERDEFS_H__
#define __BSSERVERDEFS_H__
#include <e32std.h>

/**
 *
 *
 */
const TInt KBSVersionMajor = 1;
const TInt KBSVersionMinor = 1;
const TInt KBSVersionBuild = 1;

/**
 *
 *
 */
_LIT( KBSServerExe, "bsserver.exe" );


/**
 *
 *
 */
_LIT( KBSServerName, "bs" );


/**
 *
 *
 */
const TUint KBSMsgSlotCount = 4;


enum TTransactionType
	{
	EBSTxInstall = 1,
	EBSTxUninstall,
	EBSTxAppend,
	EBSTxReplace,
	EBSTxAccess
	};

/**
 *
 *
 */
enum TBSMessages
    {
    // access API
    EBSInitInstall = 1,
    EBSInitUninstall,
    EBSInitAppend,
    EBSInitReplace,
    EBSInitAccess,
    EBSPrepareText,
    EBSGetText,
    EBSGetInt,
    EBSPrepareBuffer,
    EBSGetBuffer,
    EBSPrepareSeveral,
    EBSGetSeveral,
    EBSGetFile,
    EBSPrepareStructure,
    EBSGetStructure,
    
    EBSStartTransaction,
    EBSStopTransaction,
    EBSCancelTransaction,
    
    // installing
    EBSInstall,
    
    // replacing
    EBSReplace,
    
    // appending
    EBSAppend,
    
    // removing
    EBSRemoveBrand,
    
    EBSRemoveApplication,
    
    // observing
    EBSObserveBrand,
    EBSObserveGetNewVersion,
    EBSObserveGetChange,
    EBSObserveGetBackupState,
    EBSIsBrandUpdateRequired,
    EBSOperationLast
    };


#endif  //__BSSERVERDEFS_H__

// END OF FILE





