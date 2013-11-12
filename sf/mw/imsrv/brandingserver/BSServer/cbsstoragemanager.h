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
* Description:   Storage handler
*
*/



#ifndef CBSSTORAGEMANAGER_H
#define CBSSTORAGEMANAGER_H

#include <e32base.h>
#include <f32file.h>

class CBSSession;
class CBSIBYWriter;
class MBSElement;
class CBSStorage;
class RWriteStream;


/**
 *  Storage handler
 *
 *  @lib .exe
 *  @since S60 v3.2
 */
class CBSStorageManager : public CBase
{
public:

    static CBSStorageManager* NewL( CBSSession* aSession, const TDesC& aAppId );

    static CBSStorageManager* NewLC( CBSSession* aSession, const TDesC& aAppId );

    ~CBSStorageManager();


    /**
     * Initializes the transaction to the
     * storagemanager
     *
     * @since S60 3.2
     * @return none
     */
    void ConnectTransactionL(const TDesC& aAppId,
    					   const TDesC& aBrandId,
    					   TLanguage aLanguageId,
    					   TBool aAppending = EFalse,
    					   TInt aReserved = 0 );


    /**
     * Create a new storage and set it to current.
     *
     * @since S60 3.2
     * @return none
     */
    void CreateStorageL();

    /**
     * Return the count of storages
     *
     * @since S60 3.2
     * @return count of elements
     */
    TInt StorageCount();

    /**
     * Get stored storage at index or current storage.
     *
     * @since S60 3.2
     * @param aIndex index of storage to fetch.
     * @return Stored storage
     */
    CBSStorage* Storage( TInt aIndex );
    CBSStorage* Storage();

    /**
     * Externalize storage files into file system
     *
     * @since S60 3.2
     * @param aFs filesystem handle
     * @param aBaseDir Base directory for writing
     * @return the new brand version
     */
    TInt WriteStorageFilesL();

	void CancelTransactionL();

    void BrandHandleL( const TDesC& aAppId,
					   const TDesC& aBrandId,
					   TLanguage aLanguageId,
					   RFile& aFile, 
					   TInt aReserved );

    void FileElementHandleL( const TDesC& aAppId,
						     const TDesC& aBrandId,
						     const TDesC& aFileName,
						     const TDesC& aLanguageBuf,
						     RFile& aFile );

    void CleanupFileL( const TDesC& aFileName );

	TPtrC FilenameWithoutVersion( const TDesC& aFileName );

    /**
     * Externalize storage files into file system
     *
     * @since S60 3.2
     * @param aFileName the name of the installed file
     * 		  including path
     * @return the filename without path component
     */
	HBufC* InstallFileLC( const TDesC& aFileName );

	void RemoveBrandL(  const TDesC& aAppId,
						const TDesC& aBrandId,
						TLanguage aLanguageId,
						TInt aReserved = 0);

	void RemoveApplicationL(  const TDesC& aAppId );

	TInt NextAvailableVersionL();
	void SetVersion( TInt aVersionId );

	TInt GetNewestVersionL( const TDesC& aAppId,
							const TDesC& aBrandId,
							TLanguage aLanguageId,
						    TPtr* aDrive = NULL);
	TInt WriteIbyFiles();
    
    void LockStorage();
    
    void ReleaseLockL();
    
    TBool CheckBrandDiscarded(const TDesC& aAppId,
							const TDesC& aBrandId) ;
	static HBufC* ConstructDiscardBrandFileNameL( const TDesC& aAppId,
										      const TDesC& aBrandId );
										      
	static HBufC* FullDiscardBrandFileNameLC( TDesC& aFilename ) ;
							
    TInt isActualBrandInstalledL ( const TDesC& aAppId, const TDesC& aBrandId, TLanguage aLanguage);
private:

    CBSStorageManager();

    void ConstructL( CBSSession* aSession, const TDesC& aAppId );
    
    HBufC* ConstructFileNameL( TInt aVersion = 0 );
    
    HBufC* ConstructFileNameL( const TDesC& aAppId,
						       const TDesC& aBrandId,
						       TLanguage aLanguage,
						       TInt aReserved = 0 );
    
	void CleanupTransaction();
	
	HBufC* FullFileNameLC( const TDesC& aFilename, TBool aForceCDrive = EFalse );
	
	TInt ReadVersionL( const TDesC& aFilename );
	
	HBufC* FindAvailableVersionL( TInt aCurrentVersion, TInt& aNewVersion );

	TBool IsBaseFileL( const TDesC& aFileName );

	
	/**
     * Mark a brand package as discarded. Future attempts to create an 
     * access to this brand package will fail.
     *
     * @since S60 3.2
     * @param aAppId a application id
     * @param aBrandId a brand id
     */
	void MarkBrandAsDiscardedL( const TDesC& aAppId,
		                        const TDesC& aBrandId );

private: // Data

    // Own. Array of stored storages.
    RPointerArray<CBSStorage> iStorages;

    // Index of currently active storage
    TInt iCurrent;

    // IBY writer
    CBSIBYWriter* iWriter;
    
    RFs		iFs;
    TBool	iActive;
    
    HBufC* iAppId;
	HBufC* iBrandId;
	HBufC*  iLanguageId;
	TLanguage iLanguageIdNum;
	
    // Own. Proposed file name
    HBufC* iFilename;

    // Proposed directory (points to iFilename)
    TPtrC iDir;
	
	// doesn't own: the server
	CBSSession* iSession;
	
	TBool iAppending;
	
	HBufC* iIbyFile;
	TInt  iReserved;
	
};

#endif //CBSSTORAGEMANAGER_H
