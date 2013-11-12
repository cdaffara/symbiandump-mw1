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
* Description:  Declaration of CExcludeListEntry class
*
*/


#ifndef C_EXCLUDELISTENTRY_H
#define C_EXCLUDELISTENTRY_H

#include <e32base.h>

/**
* CExcludeListEntry class represents one exclude list item.
* It contains the definition from one row of the exclude list file. The
* definition is a file or directory name, possibly containing wildcards.
* CExcludeListEntry can calculate and return the method of preserving the
* file or directory based on the wildcards in the defintions.
*/
class CExcludeListEntry : public CBase
    {
public:

    /**
    * Helper class used in path comparison and matching operations.
    */
    class TMatchEntry
       {
    public:
        TMatchEntry( const TDesC& aPath ); // sets iPath and iFolderLevel
    public:
        TPtrC iPath; /** File or directory path. Not own; must exist the lifetime of this objest. */
        TInt iFolderLevel; /** Deepness of directory structure in the path. */
    };

    /** Methods of preserving directory contents. */
    enum TPreserveType
        {
        EPreserveNone = 0, /** Do not preserve the file or directory. */
        EPreserve,         /** Preserve the file or directory. */
        EPreserveAll,      /** Preserve the directory and all its content. */
        EPreserveMatching  /** The entry contains wildcards - preserve all */
                           /** matching files and directories. */
        };

public:

    /**
    * Two-phased constructor.
    *
    * @param aPath Exclude list item
    * @return The new object
    */
    static CExcludeListEntry* NewL( const TDesC& aPath );

    /**
    * Two-phased constructor.
    *
    * @param aPath Exclude list item
    * @return The new object
    */
    static CExcludeListEntry* NewLC( const TDesC& aPath );

    /**
    * Destructor.
    */
    virtual ~CExcludeListEntry();

    /**
    * Return the exclude list item.
    *
    * @return The exclude list item
    */
    TPtrC Path() const;

    /**
    * Return the type of preserving the exclude list item.
    *
    * @param aMatchEntry In some cases the preserving type depends on the
    * directory entry matching the exlude list entry. Directory entry details
    * are given as parameter to select correct preserving type.
    * @return Preserving type
    */
    TPreserveType Type( const TMatchEntry& aMatchEntry ) const;

    /**
    * Ordering function for adding items to the exclude list array in order.
    * this way duplicates can be eliminated.
    *
    * @param aLhs Left hand side of the comparison expression.
    * @param aRhs Right hand side of the comparison expression.
    * @return
    *   1. zero, if the two objects are equal.
    *   2. a negative value, if the first object is less than the second.
    *   3. a positive value, if the first object is greater than the second.
    */
    static TInt Compare(
        const CExcludeListEntry& aLhs,
        const CExcludeListEntry& aRhs );

    /**
    * Comparison function for full or partial match.
    * Example:
    *    c:\temp.txt matches c:\*.txt
    *    c:\ matches c:\a\* (it is part of it)
    *    c:\temp.txt matches c:\ (also part of the other path)
    *
    * @param aMatchEntry Directory entry (file or directory name) to compare.
    * @param aExclude Exclude list item to compare the entry to.
    * @return 1 if there is a match, 0 otherwise
    */
    static TBool CompareMatch(
        const TMatchEntry* aMatchEntry,
        const CExcludeListEntry& aExclude );

private:

    /**
    * First phase constructor.
    */
    CExcludeListEntry();

    /**
    * Second phase constructor.
    *
    * @param aPath Exclude list item
    */
    void ConstructL( const TDesC& aPath );

private: // Data

    /** Exclude list item. Not NULL. Own. */
    HBufC* iPath;

    /** Deepness of directory structure in the iPath. */
	TInt iFolderLevel;

    /** Identifies how to preserve directory contents. */
    TPreserveType iType;
    };

#endif // C_EXCLUDELISTENTRY_H
