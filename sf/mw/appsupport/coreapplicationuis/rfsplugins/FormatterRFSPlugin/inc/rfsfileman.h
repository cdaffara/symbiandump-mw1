/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CRfsFileMan class
*
*/


#ifndef C_RFSFILEMAN_H
#define C_RFSFILEMAN_H

#include <e32base.h>

class CDirStackEntry;

/**
* CRfsFileMan can delete files and directories, also with read-only or hidden
* attributes.
*/
class CRfsFileMan : public CBase
    {

public:

    /**
    * Two-phased constructor.
    *
    * @param aFs File server session handle
    * @return The new object
    */
    static CRfsFileMan* NewL( RFs& aFs );

    /**
    * Destructor.
    */
    virtual ~CRfsFileMan();

    /**
    * Remove file or a directory with all its content.
    * Also removes hidden and read-only files and directories.
    *
    * @param aFullPath The name of the file or directory.
    * @param aRootEntry File system entry for the file or directory.
    */
    void DeleteL( const TDesC& aFullPath, const TEntry& aRootEntry );

private:

    /**
    * First phase constructor.
    *
    * @param aFs File server session handle
    */
    CRfsFileMan( RFs& aFs );

    /**
    * Second phase constructor.
    */
    void ConstructL();

    /**
    * Delete file or an empty directory.
    * Also removes hidden and read-only files and directories.
    *
    * @param aFullPath The name of the file or directory.
    * @param aEntry File system entry for the file or directory.
    */
    TInt Delete( const TDesC& aFullPath, const TEntry& aEntry );

    /**
    * Remove read-only and hidden attributes from the item.
    *
    * @param aFullPath Name of the file or directory.
    * @param aEntry File system entry for the file or directory.
    */
    void ResetAttributes( const TDesC& aFullPath, const TEntry& aEntry );

private: // Data

    /** File server session handle. */
    RFs iFs;

    /**
    * Stack of directory names which can not be deleted immediately because
    * they have some content - they will be deleted later on.
    */
    RPointerArray<CDirStackEntry> iDirStack;

    };

#endif // C_DIRSTACKENTRY_H
