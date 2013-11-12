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
* Description:  Declaration of CFileCreator class
*
*/


#ifndef C_FILECREATOR_H
#define C_FILECREATOR_H

#include <e32base.h>

class RFs;

/**
*  ?one_line_short_description
*  ?more_complete_description
*
*  @code
*   ?good_class_usage_example(s)
*  @endcode
*
*/
class CFileCreator : public CBase
    {

public:

    /**
    * Constructs a CFileCreator object.
    *
    * @return The new object
    */
    static CFileCreator* NewL( RFs& aFs );

    /**
    * Destructor.
    */
    virtual ~CFileCreator();

    /**
    * Create one file.
    */
    void CreateOneFileL( const TDesC& aName, TInt aSizeInBytes );

private:

    /**
    * First phase constructor.
    */
    CFileCreator( RFs& aFs );

    /**
    * Second phase constructor.
    */
    void ConstructL();

private: // data

    RFs& iFs; // File server session.

    HBufC8* iWriteBuf; // Buffer of data to write to fill the files.

    };

#endif // C_FILECREATOR_H
