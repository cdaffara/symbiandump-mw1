/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CSelectiveFormatter class
*
*/


#ifndef C_SELECTIVEFORMATTER_H
#define C_SELECTIVEFORMATTER_H

#include <e32base.h>

#include "excludelistentry.h"

class CDirStackEntry;
class CRfsFileMan;

/**
*  Reads exclude list and deletes other files on drive but those listed in
*  the exclude list.
*/
class CSelectiveFormatter : public CBase
    {
public:

    /**
    * Two-phased constructor.
    *
    * @param aExcludeListFile Identifies the exclude list to use
    * @return The new object
    */
    static CSelectiveFormatter* NewLC( const TDesC& aExcludeListFile );

    /**
    * Destructor.
    */
    virtual ~CSelectiveFormatter();

    /**
    * Recursively find and delete all files which have not been read into the
    * excludelist array from the exclude list.
    *
    * @param aFullPath The root directory in which the delete operation takes
    * place.
    */
    void FindAndDeleteL( const TDesC& aFullPath );

private:

    /**
    * First phase constructor.
    */
    CSelectiveFormatter();

    /**
    * Second phase constructor.
    *
    * @param aExcludeListFile Identifies the exclude list to use
    */
    void ConstructL( const TDesC& aExcludeListFile );

    /**
    * Reads the excludelist into the array. Exclude list rules:
    * - It must explicitly state every directory, subdirectory and file that is to be kept
    * - Directories must have a trailing backslash
    * - It must be saved as a unicode text file
    * - It must end with a new line
    * - It must be kept in a private directory
    *
    * @param aExcludeListFile Identifies the exclude list to use
    */
    void ReadExcludeListL( const TDesC& aExcludeListFile );

    /**
    * Gets the preserving type for a path (file or directory name).
    * If the path can be found in the exclude list, the preserving type of
    * the matching exclude list is returned. Otherwise the preserving type is
    * EPreserveNone.
    *
    * @param aPath Name of file or directory.
    * @return The preserving type for the directory or file.
    */
    CExcludeListEntry::TPreserveType PreserveType( const TDesC& aFullPath ) const;

    /**
    * Handles one entry from the stack of directory entries - based on the
    * preserving type of the entry and whether it is a directory or a file
    * the directory or file is either deleted or preserved, and possible new
    * directory entry is pushed to the stack.
    *
    * @param aFullPath Name of file or directory.
    * @param aEntry Details about the entry such as whether it is a file or a
    * directory.
    */
    void HandleDirEntryL( const TDesC& aFullPath, const TEntry& aEntry );
    
    /**
    * Handles application specific exclude list loading.       
    */
    void HandleAppExcludeListsL();
    
    /**
    * Handles application specific exclude list loading on specified drive.
    */
    void HandleAppExcludeListsOnDriveL( TPtr aBuf, TChar aDrive );

    /**
    * Handles NR-Application specific exclude list loading.       
    */
    void HandleNrExcludeListsL();

    /**
    * Append the list of nr-files to the excludelist entry.
    */
    void AppendNrlisttoExcludeListL(RPointerArray<HBufC> &nrFileList);

private: // Data

    /** File server session. */
    RFs iFs;

    /** File manager object for reading directory contents. Own. */
    CRfsFileMan* iFileMan;

    /** Exclude list contents read from file. */
    RPointerArray<CExcludeListEntry> iExcludeList;

    /** Stack of directory names found and possibly deleted during the operation. */
    RPointerArray<CDirStackEntry> iDirStack;
    
    /** Identifies the drive to restore. */
    TChar iSystemDrive;
        
    /** ETrue if there is valid exclude list content. */
    TBool iValidExcludeListFound;
    };

/** 
Template class CleanupResetAndDestroy to clean up the array
of implementation information from the cleanup stack.
*/

template <class T>
class CleanupResetAndDestroy
    {
public:
    /**
    Puts an item on the cleanup stack.

    @param  aRef 
            The implementation information to be put on the cleanup stack.
    */
    inline static void PushL(T& aRef);
private:
    static void ResetAndDestroy(TAny *aPtr);
    };
template <class T>
inline void CleanupResetAndDestroyPushL(T& aRef);
template <class T>
inline void CleanupResetAndDestroy<T>::PushL(T& aRef)
    {CleanupStack::PushL(TCleanupItem(&ResetAndDestroy,&aRef));}
template <class T>
void CleanupResetAndDestroy<T>::ResetAndDestroy(TAny *aPtr)
    {(STATIC_CAST(T*,aPtr))->ResetAndDestroy();}
template <class T>
inline void CleanupResetAndDestroyPushL(T& aRef)
    {CleanupResetAndDestroy<T>::PushL(aRef);}

#endif // C_SELECTIVEFORMATTER_H
