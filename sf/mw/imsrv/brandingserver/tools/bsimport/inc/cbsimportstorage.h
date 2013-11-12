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



#ifndef CBSIMPORTSTORAGE_H
#define CBSIMPORTSTORAGE_H

#include <e32base.h>

class MBSElement;
class RWriteStream;


/**
 *  Stores element data and writes it to stream
 *
 *  @lib brandimporter.exe
 *  @since S60 v3.2
 */
class CBSImportStorage : public CBase
{
public:

    static CBSImportStorage* NewL();
    
    static CBSImportStorage* NewLC();
    
    ~CBSImportStorage();
    
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
    TInt AppendElement( MBSElement* aElement );
    
    /**
     * Return the count of elements in storage
     *
     * @since S60 3.2
     * @return count of elements
     */
    TInt ElementCount();

    /**
     * Return one element
     *
     * @since S60 3.2
     * @return requested element
     */
    MBSElement* Element( TInt aIndex );
    
    /**
     * Externalize storage into stream
     *
     * @since S60 3.2
     * @param aWriteStream output stream
     * @return none
     */
    void ExternalizeL( RWriteStream& aWriteStream );
    void FlushToServerL();

    /**
     * Gives a full filename which is constructed from header data
     *
     * @since S60 3.2
     * @return filename
     */
    TPtrC ProposeFileNameL();

    /**
     * Scan through all elements in storage and find EFile items.
     * Appends all found items to aFileList array.
     *
     * @since S60 3.2
     * @param aFileList filelist array
     */
    void GetListOfFiles( RArray<TPtrC>& aFileList );

    /**
     * Compare storages header data to another storage.
     *
     * @since S60 3.2
     * @param aStorage storage to compare
     * @return ETrue  - storages match
     *         EFalse - storages don't match
     */
    TBool Compare( const CBSImportStorage* aStorage ) const;
    
    /**
     * Take content of one storage into this storage. Will delete
     * elements in aStorage!
     *
     * @since S60 3.2
     * @param aStorage storage to append
     */
    void TakeElements( CBSImportStorage* aStorage );

private:

    CBSImportStorage();

    void ConstructL();
    
    /**
     * Check that header information is complete
     *
     * @since S60 3.2
     * @return ETrue  - header information ok
     *         EFalse - header information missing or incomplete
     */
    TBool VerifyHeader();

    /**
     * Check that data information is correct.
     *  - verify that elements have unique ID's
     *
     * @since S60 3.2
     * @param aArray array of elements to check
     * @param aIgnoreEmptyID ETrue: empty id's are considered as unique
     *                       EFalse: 2 empty id's will fail this check
     * @return ETrue  - data ok.
     *         EFalse - bad data.
     */
    TBool VerifyDataL( TArray<MBSElement*> aArray, 
                       TBool aIgnoreEmptyID = EFalse );
    
    /**
     * Creates a filename from given header data.
     * \[application_id]\[brand_id]\[def_filename][language_id]
     *
     * @since S60 3.2
     */
    void ConstructFileNameL();
    
    
    /**
     * Check element for EFile items.
     * Appends all found items to aFileList array.
     *
     * @since S60 3.2
     * @param aElement element to check
     * @param aFileList filelist array
     */
    void AppendFiles( MBSElement* aElement, RArray<TPtrC>& aFileList );
    
private: // Data

    // Own. Array of stored elements.
    RPointerArray<MBSElement> iElements;
    
    // Own. Proposed file name
    HBufC* iFilename;
    
    // HEADER DATA
    
    // Version number of storage
    TInt iVersion;
    
    // Language ID of storage
    TLanguage iLanguage;
    
    // Own. Application ID of storage
    HBufC8* iAppId;
    
    // Own. Storage ID.of storage
    HBufC8* iStorageId;
};

#endif //CBSIMPORTSTORAGE_H
