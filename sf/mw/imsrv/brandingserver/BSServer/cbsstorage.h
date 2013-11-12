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
* Description:   Stores element data and writes it to stream
*
*/



#ifndef CBSSTORAGE_H
#define CBSSTORAGE_H

#include <e32base.h>

#include "rbsobjowningptrarray.h"

class MBSElement;
class RWriteStream;
class MBSImportLogger;


/**
 *  Stores element data and writes it to stream
 *
 *  @lib brandimporter.exe
 *  @since S60 v3.2
 */
class CBSStorage : public CBase
{
public:

    static CBSStorage* NewL();

    static CBSStorage* NewLC();

    ~CBSStorage();

    /**
     * Set version of brand storage
     *
     * @since S60 3.2
     * @param aVersion version
     */
    void SetVersion( TInt aVersion );

    /**
     * Set application ID of brand storage
     *
     * @since S60 3.2
     * @param aAppId application ID
     * @return none
     */
    void SetApplicationIdL( const TDesC& aAppId );

    /**
     * Set storage ID of brand storage
     *
     * @since S60 3.2
     * @param aStorageId storage ID
     * @return none
     */
    void SetStorageIdL( const TDesC& aStorageId );

    /**
     * Set language of brand storage
     *
     * @since S60 3.2
     * @param aLanguage language
     * @return none
     */
    void SetLanguageL( TLanguage aLanguage );

    /**
     * Store element to storage. Storage takes the ownership of element.
     *
     * @since S60 3.2
     * @param aElement element to store.
     * @return General Symbian errorcode.
     */
    void AppendElementsL( MBSElement* aElement );

    /**
     * Return the count of elements in storage
     *
     * @since S60 3.2
     * @return count of elements
     */
    TInt ElementCount();

    /**
     * Externalize storage into stream
     *
     * @since S60 3.2
     * @param aWriteStream output stream
     * @return none
     */
    void ExternalizeL( RWriteStream& aWriteStream );

    /**
     * Replaces an element in the storage
     *
     * @since S60 3.2
     * @param aElement the element to be replaced
     */
    void ReplaceElementL( MBSElement* aElement );
	

    TPtrC ProposeFileNameL();
    TPtrC ProposedDirL();
    void GetListOfFilesL( RArray<TPtrC>& aFileList );

private:

    CBSStorage();

    void ConstructL();

    /**
     * Check that header information is complete
     *
     * @since S60 3.2
     * @return ETrue  - header information ok
     *         EFalse - header information missing or incomplete
     */
    TBool VerifyHeader();

    TBool VerifyDataL( TArray<MBSElement*> aArray,
                      TBool aIgnoreEmptyID = EFalse );
	TBool VerifyDataL( RBSObjOwningPtrArray<MBSElement>& aArray,
                      TBool aIgnoreEmptyID = EFalse );

    void HandleIfErrorL( TInt aError, const TDesC& aText );

    void AppendFilesL( MBSElement* aElement, RArray<TPtrC>& aFileList );

private: // Data

    // Own. Array of stored elements.
    RBSObjOwningPtrArray<MBSElement> iElements;

    // Own. Proposed file name
    HBufC* iFilename;

    // Proposed directory (points to iFilename)
    TPtrC iDir;

    // HEADER DATA

    // Version number of storage
    TInt iVersion;

    // Own. Language ID of storage
    HBufC* iLanguage;

    // Own. Application ID of storage
    HBufC* iAppId;

    // Own. Storage ID.of storage
    HBufC* iStorageId;
};

#endif //CBSSTORAGE_H
