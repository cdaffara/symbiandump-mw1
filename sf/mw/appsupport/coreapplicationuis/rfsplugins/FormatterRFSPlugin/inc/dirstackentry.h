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
* Description:  Declaration of CDirStackEntry class
*
*/


#ifndef C_DIRSTACKENTRY_H
#define C_DIRSTACKENTRY_H

#include <e32base.h>

/**
* CDirStackEntry represents one directory to check and possibly delete.
* It  holds the name of the directory and reads a list of directory contents
* from File Server. The directory content list can be stepped through using
* GetNextEntry() method.
*
* CDirStackEntry objects are designed to be stored in a stack data structure
* - class also has static methods for pushing new CDirStackEntry objects to a
* stack and popping them.
*/
class CDirStackEntry : public CBase
    {

public:

    /**
    * Two-phased constructor.
    *
    * @param aDirName Full path name of the directory
    * @param aFs File server session handle
    * @return The new object
    */
    static CDirStackEntry* NewLC( const TDesC& aDirName, RFs& aFs );

    /**
    * Destructor.
    */
    virtual ~CDirStackEntry();

    /**
    * Get full path of the directory entry.
    *
    * @param aFullPath On return, contains the path of the directory entry.
    *        The descriptor needs to be big enough to contain any directory or
    *        file name.
    */
    void FullPath( TDes& aFullPath );

    /**
    * Get next item from the list of files/subdirectories in the directory
    * represented by this object.
    *
    * @param aFullPath On return, contains the path of the item returned.
    *        The descriptor needs to be big enough to contain any directory or
    *        file name.
    * @return The entry object containing information about the item.
    * Ownership is not transferred.
    */
    const TEntry* GetNextEntry( TDes& aFullPath );

    /**
    * Create a new CDirStackEntry object and push it to the stack.
    *
    * @param aStack The stack object
    * @param aDirName Full path name of the directory
    * @param aFs File server session handle
    */
    static void PushDirEntryL(
        RPointerArray<CDirStackEntry>& aStack,
        const TDesC& aDirName,
        RFs& aFs );

    /**
    * Pop a CDirStackEntry object from the stack and destroy it.
    *
    * @param aStack The stack object
    */
    static void PopAndDestroyDirEntry( RPointerArray<CDirStackEntry>& aStack );

private:

    /**
    * First phase constructor.
    */
    CDirStackEntry();

    /**
    * Second phase constructor.
    *
    * @param aDirName Full path name of the directory
    * @param aFs File server session handle
    */
    void ConstructL( const TDesC& aDirName, RFs& aFs );

private: // Data

    /** Name of the directory. Not NULL. Own. */
    HBufC* iDirName;

    /** List of contents of the directory. Not NULL. Own. */
    CDir* iDirList;

    /** Index to iDirList for going through its contents. */
    TInt iDirListIndex;
    };

#endif // C_DIRSTACKENTRY_H
