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
* Description:  Declaration of CFileSpec class
*
*/


#ifndef C_FILESPEC_H
#define C_FILESPEC_H

#include <e32base.h>
#include <f32file.h>

class CFileCreator;

/**
* Reads a specification of files and directories from a text file and creates
* files and directories based on it - or checks that the specified files and
* directories exist.
*/
class CFileSpec : public CBase
    {

public:

    /**
    * Constructs a CFileSpec object.
    *
    * @return The new object
    */
    static CFileSpec* NewLC( RFs& aFs, const TDesC& aSpecFileName );

    /**
    * Destructor.
    */
    virtual ~CFileSpec();

    /**
    * Read spec file and create files and folders.
    * Only call this once and then delete object.
    */
    void CreateL();

    /**
    * Read spec file and verify files and folders.
    * Only call this once and then delete object.
    */
    void VerifyL( const TDesC& aRootDir );

private:

    /**
    * First phase constructor.
    */
    CFileSpec( RFs& aFs );

    /**
    * Second phase constructor.
    */
    void ConstructL( const TDesC& aSpecFileName );

    void CreateOneDirectoryL( TFileText& aSpecFile );
    void CreateOneFileL( TFileText& aSpecFile );
    void CreateFilesL( TFileText& aSpecFile );
    void CreateFilesL( const TDesC& aName, TInt aSizeInBytes, TInt aCount );

    void ReadOneItemL( TFileText& aSpecFile );
    void ReadManyItemsL( TFileText& aSpecFile );

    void CheckAllKeptFilesExistL();
    void CheckExistsL( const TDesC& aName );
    void CheckOnlyKeptFilesExistL( const TDesC& aRootDir );
    void CheckFileMayExistL( const TDesC& aName );

private: // data

    /**
    * File server session.
    */
    RFs& iFs;

    /**
    * File containing specification on what to create / verify.
    */
    RFile iSpecFile;

    /**
    * Helps creating files.
    * Own. May be NULL.
    */
    CFileCreator* iFileCreator;

    /**
    * Used to store a list of files that should be preserved over RFS.
    * Own. May be NULL.
    */
    RPointerArray<HBufC> iKeptFiles;
    };

#endif // C_FILESPEC_H
