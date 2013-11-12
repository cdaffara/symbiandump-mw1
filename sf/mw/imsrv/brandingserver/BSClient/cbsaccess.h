/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors: cbsaccess.h
*
* Description:  cbsaccess.h
*
*/



#ifndef __CBSACCESS_H__
#define __CBSACCESS_H__
#include <e32std.h>
#include "mbsaccess.h"
#include "cbsclient.h"


// Forward declarations
class CFbsBitmap;

/**
 * 
 */
NONSHARABLE_CLASS( CBSAccess ): public CBase,
								public MBSAccess
    {
	public:
		/**
		* Two-phased constructor.
		*/
		static CBSAccess* NewL( const TDesC8& aBrandId,
								const TDesC8& aAppId,
								const TDesC8& aDefaultBrandId,
								TLanguage aLanguageId,
        					  	TBool aCacheData,
								TInt aReserved = 0 );

		// destructor
    	virtual ~CBSAccess();

		/*
		* Close
		*/
		void Close();

	private:

    	void ConstructL( const TDesC8& aBrandId, 
    					 const TDesC8& aAppId, 
    					 const TDesC8& aDefaultBrandId );
		CBSAccess( TLanguage aLanguageId, TBool aCacheData, TInt aReserved = 0);


	private: // from MBranding

    	/*
    	* Get text type item
    	* @param aId ID of the item
    	* @return the text item
    	*/
        HBufC* GetTextL( const TDesC8& aId );

    	/*
    	* Get text type item
    	* @param aId ID of the item
    	* @return the text item
    	*/
        HBufC* GetTextLC( const TDesC8& aId );

    	/*
    	* Get 8-bit data type item
    	* @param aId ID of the item
    	* @return the data buffer item
    	*/
        HBufC8* GetBufferL( const TDesC8& aId );

    	/*
    	* Get 8-bit data type item
    	* @param aId ID of the item
    	* @return the data buffer item
    	*/
        HBufC8* GetBufferLC( const TDesC8& aId );

    	/*
    	* Get integer type item
    	* @param aId ID of the item
    	* @return the integer item
    	*/
        TInt GetIntL( const TDesC8& aId );


    	/*
    	* Get an array of items
    	* @param aIds ID's of the items
    	* @return the items
    	*/
        MBSElement* GetSeveralL( MDesC8Array& aIds );

    	/*
    	* Get a file item
    	* @param aId ID of the item
    	* @param aFile on return will contain an open file handle
    	*        to the file
    	*/
        void GetFileL( const TDesC8& aId, RFile& aFile );

    	/*
    	* Get a structure type item
    	* @param aIds ID of the item
    	* @param aFile on return will contain the strucure type item
    	* @return structure branding element
    	*/
        MBSElement* GetStructureL( const TDesC8& aId );

    	/*
    	* Register to observer brand data updates
    	* @param aObserver the observer interface
    	*/
        void RegisterObserverL( MBSBrandChangeObserver* aObserver, MBSBackupRestoreStateObserver *aBackupObserver );

    	/*
    	* Unregister a brand data update observer
    	* @param aObserver the observer interface
    	*/
        void UnRegisterObserverL( MBSBrandChangeObserver* aObserver, MBSBackupRestoreStateObserver *aBackupObserver);

    	/*
    	* Get a bitmap type item
    	* @param aBitmapId the ID of the bitmap item itself
    	* @param aBitmap on return will contain the bitmap
    	* @param aMask on return will contain the mask
    	*/
		void GetBitmapL( const TDesC8& aBitmapId,
						 CFbsBitmap*& aBitmap,
						 CFbsBitmap*& aMask );



	TInt isBrandUpdateRequiredL ();

		
    public:     // New methods



	private: // data
		HBufC8* iBrand;
		HBufC8* iDefaultBrand;
		HBufC8* iAppId;
		TLanguage iLanguageId;
		TBool iUseCache;
		RBSClient iClient;
		TInt iReserved;
    };

#endif      //  __CBSACCESS_H__


//  END OF FILE

