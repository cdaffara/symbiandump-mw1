/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Writes IBY files
*
*/



#ifndef CBSIBYWRITER_H
#define CBSIBYWRITER_H

#include <e32base.h>

class RFs;

/**
 *  Writes IBY files
 *
 *  @lib
 *  @since S60 v3.2
 */
class CBSIBYWriter : public CBase
{
public:

    static CBSIBYWriter* NewL();

    static CBSIBYWriter* NewLC();

    ~CBSIBYWriter();

    /**
     * Initialize IBY file
     *
     * @param aFs file system handle
     * @param aFileName Full name of the iby file
     * @since S60 3.2
     * @return none
     */
    void InitIbyFileL( RFs& aFs, const TDesC& aFileName );

    /**
     * File item included in IBY file
     *
     * @since S60 3.2
     * @param aSrc source file
     * @param aDest destination file
     * @return none
     */
    void SetFileItemL( const TDesC& aSrc, const TDesC& aDest );

    /**
     * Writes Iby file from given file items
     *
     * @since S60 3.2
     * @param aFs file system handle
     * @param aFileName Full name of the iby file
     * @return none
     */
    void WriteIBYFileL( RFs& aFs, const TDesC& aFileName );

private:

    CBSIBYWriter();

    void ConstructL();

private: // Data

    // Own. Source path array
    RPointerArray<HBufC8> iSourcePath;

    // Own. Source files
    RPointerArray<HBufC8> iSourceFiles;

    // Own. Destination files
    RPointerArray<HBufC8> iDestinations;
    
    // Own. Iby file name
    HBufC* iIbyFile;
};

#endif //CBSIBYWRITER_H
