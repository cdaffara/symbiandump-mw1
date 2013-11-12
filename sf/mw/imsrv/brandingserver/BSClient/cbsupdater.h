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
* Contributors:
*
* Description:  CBSUpdater.h
*
*/



#ifndef __CBSUPDATER_H__
#define __CBSUPDATER_H__
#include <e32std.h>
#include "mbsupdater.h"
#include "cbsclient.h"

/**
 * Simple ClientServer for
 * testing and example purposes.
 */
NONSHARABLE_CLASS( CBSUpdater ): public CBase,
								 public MBSUpdater
    {
	public:
		/**
		* Two-phased constructor.
		*/
		static CBSUpdater* NewL( const TDesC8& aApplicationId );

    	virtual ~CBSUpdater();

		/*
		* @see MBSUpdater
		*/ 
		void Close();

	private:
    	void ConstructL( const TDesC8& aApplicationId );
		CBSUpdater();


	private: // from MBSUpdater

     	/*
     	* @see MBSUpdater
     	*/
        void StartTransactionL( const TDesC8& aBrandId,
         						TLanguage aLanguageId,
         						TUpdateTransactionType aType = EUpdateInstall,
								TInt aReserved = 0 );
     	/*
     	* @see MBSUpdater
     	*/
		TInt StopTransactionL();

     	/*
     	* @see MBSUpdater
     	*/
		void CancelTransactionL();


     	/*
     	* @see MBSUpdater
     	*/
		void RemoveBrandL( const TDesC8& aApplicationId,
								   const TDesC8& aBrandId );

     	/*
     	* @see MBSUpdater
     	*/
		void RemoveBrandsL( const TDesC8& aApplicationId );


		//*** BRAND INSTALLING ***//
     	/*
     	* @see MBSUpdater
     	*/
        void InsertTextL( const TDesC8& aId,
        				  const TDesC& aText );

     	/*
     	* @see MBSUpdater
     	*/
        void InsertBufferL( const TDesC8& aId,
        					const TDesC8& aBuffer );

     	/*
     	* @see MBSUpdater
     	*/
        void InsertIntL( const TDesC8& aId,
        				 TInt aInt );

     	/*
     	* @see MBSUpdater
     	*/
        void InsertFileL( const TDesC8& aId,
        				  const TDesC& aFileName );

     	/*
     	* @see MBSUpdater
     	*/
        void InsertElementL( MBSElement* aElement );



		//*** BRAND UPDATING - replacing ***//
     	/*
     	* @see MBSUpdater
     	*/
        void ReplaceTextL( const TDesC8& aId,
        				   const TDesC& aText );

     	/*
     	* @see MBSUpdater
     	*/
        void ReplaceBufferL( const TDesC8& aId,
        					 const TDesC8& aBuffer );

     	/*
     	* @see MBSUpdater
     	*/
        void ReplaceIntL( const TDesC8& aId,
        				  TInt aInt );

     	/*
     	* @see MBSUpdater
     	*/
        void ReplaceFileL( const TDesC8& aId,
        				   const TDesC& aFileName );

     	/*
     	* @see MBSUpdater
     	*/
        void ReplaceElementL( MBSElement* aElement );



		//*** BRAND UPDATING - appending ***//
     	/*
     	* @see MBSUpdater
     	*/
        void AppendTextL( const TDesC8& aId,
        				  const TDesC& aText );

     	/*
     	* @see MBSUpdater
     	*/
        void AppendBufferL( const TDesC8& aId,
        					const TDesC8& aBuffer );

     	/*
     	* @see MBSUpdater
     	*/
        void AppendIntL( const TDesC8& aId,
        				 TInt aInt );

     	/*
     	* @see MBSUpdater
     	*/
        void AppendFileL( const TDesC8& aId,
        				  const TDesC& aFileName );

     	/*
     	* @see MBSUpdater
     	*/
        void AppendElementL( MBSElement* aElement );

     	/*
     	* @see MBSUpdater
     	*/
        void RegisterObserverL( MBSBackupRestoreStateObserver* aBackupObserver ) ;
		
     	/*
     	* @see MBSUpdater
     	*/
        void UnRegisterObserverL( MBSBackupRestoreStateObserver* aObserver ) ;

		
	private: // data

		// tells if a transaction is ongoing or not
		TBool iActive;

		HBufC8* iApplicationId;
		HBufC8* iBrandId;
		TLanguage iLanguageId;
		TBool iUseCache;
		RBSClient iClient;
		TUpdateTransactionType iTxType;
		TInt iReserved;
    };

#endif      //  __CBSUPDATER_H__


//  END OF FILE

