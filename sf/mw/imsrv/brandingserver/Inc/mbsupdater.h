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
* Description:  MBSUpdater
 *
*/




 #ifndef __MBSUPDATER_H
 #define __MBSUPDATER_H

 #include <e32std.h>
 #include <mbselement.h>

class MBSBackupRestoreStateObserver ;

const TInt KBSAutomaticVersionUpdate = -1;

 /**
  * An interface for updating branding information
  *
  * @since
  */
 class MBSUpdater
     {
     public:

		enum TUpdateTransactionType
			{
			EUpdateInstall = 1,
			EUpdateAppend,
			EUpdateReplace,
			EUpdateUninstall
			};

		//*** TRANSACTION HANDLING ***//

     	/*
     	* Start transaction for installing a brand.
     	* @param aApplicationId id of the application
     	* @param aBrandId id of the brand
     	* @param aLanguageId languageid of the brand
     	* @param aReserved for further use
     	*/
        virtual void StartTransactionL( const TDesC8& aBrandId,
         								TLanguage aLanguageId,
         								TUpdateTransactionType aType = EUpdateInstall,
         								TInt aReserved = 0 ) = 0;

     	/*
     	* Stop brand installation transaction.
     	* Calling this will indicate that the brand has
     	* been updated completely and it will be taken
     	* into use.
     	* @return new version of the installed/updated brand
     	*/
		virtual TInt StopTransactionL() = 0;

     	/*
     	* Cancels brand installation transaction.
     	* All the elements updated in this transaction
     	* will be discarded.
     	*/
		virtual void CancelTransactionL() = 0;


		//*** BRAND REMOVING ***//
		
     	/*
     	* Removes all brand data from the specified brand.
     	*/
		virtual void RemoveBrandL( const TDesC8& aApplicationId,
								   const TDesC8& aBrandId ) = 0;

     	/*
     	* Removes all brand data from the specified application.
     	*/
		virtual void RemoveBrandsL( const TDesC8& aApplicationId ) = 0;


		//*** BRAND INSTALLING ***//


    	/*
    	* Insert text type item into the new brand
    	* @param aId ID of the item
    	* @param aText the text item
    	*/
        virtual void InsertTextL( const TDesC8& aId, const TDesC& aText ) = 0;

    	/*
    	* Insert 8-bit data type item into the new brand
    	* @param aId ID of the item
    	* @param aBuffer the data item
    	*/
        virtual void InsertBufferL( const TDesC8& aId, const TDesC8& aBuffer ) = 0;

    	/*
    	* Insert integer type item into the new brand
    	* @param aId ID of the item
    	* @param aInt the integer item
    	*/
        virtual void InsertIntL( const TDesC8& aId, TInt aInt ) = 0;

    	/*
    	* Insert a file type item into the new brand.
    	* Note that the this is only the id and file name pair,
    	* the file itself has to be imported separately.
    	* @param aId ID of the item
    	* @param aFile the integer item
    	*/
        virtual void InsertFileL( const TDesC8& aId,
        					   	  const TDesC& aFileName ) = 0;

     	/*
     	* Inserts an element into the brand that is being installed.
     	* @param aElement the element
     	*/
        virtual void InsertElementL( MBSElement* aElement ) = 0;





		//*** BRAND UPDATING - replacing ***//


    	/*
    	* Insert text type item into the new brand
    	* @param aId ID of the item
    	* @param aText the text item
    	*/
        virtual void ReplaceTextL( const TDesC8& aId, const TDesC& aText ) = 0;

    	/*
    	* Insert 8-bit data type item into the new brand
    	* @param aId ID of the item
    	* @param aBuffer the data item
    	*/
        virtual void ReplaceBufferL( const TDesC8& aId, const TDesC8& aBuffer ) = 0;

    	/*
    	* Insert integer type item into the new brand
    	* @param aId ID of the item
    	* @param aInt the integer item
    	*/
        virtual void ReplaceIntL( const TDesC8& aId, TInt aInt ) = 0;

    	/*
    	* Insert a file type item into the new brand.
    	* Note that the this is only the id and file name pair,
    	* the file itself has to be imported separately.
    	* @param aId ID of the item
    	* @param aFile the integer item
    	*/
        virtual void ReplaceFileL( const TDesC8& aId,
        					   	  const TDesC& aFileName ) = 0;

     	/*
     	* Inserts an element into the brand that is being installed.
     	* @param aElement the element
     	*/
        virtual void ReplaceElementL( MBSElement* aElement ) = 0;



		//*** BRAND UPDATING - appending ***//


    	/*
    	* Insert text type item into the new brand
    	* @param aId ID of the item
    	* @param aText the text item
    	*/
        virtual void AppendTextL( const TDesC8& aId, const TDesC& aText ) = 0;

    	/*
    	* Insert 8-bit data type item into the new brand
    	* @param aId ID of the item
    	* @param aBuffer the data item
    	*/
        virtual void AppendBufferL( const TDesC8& aId, const TDesC8& aBuffer ) = 0;

    	/*
    	* Insert integer type item into the new brand
    	* @param aId ID of the item
    	* @param aInt the integer item
    	*/
        virtual void AppendIntL( const TDesC8& aId, TInt aInt ) = 0;

    	/*
    	* Insert a file type item into the new brand.
    	* Note that the this is only the id and file name pair,
    	* the file itself has to be imported separately.
    	* @param aId ID of the item
    	* @param aFile the integer item
    	*/
        virtual void AppendFileL( const TDesC8& aId,
        					   	  const TDesC& aFileName ) = 0;

     	/*
     	* Inserts an element into the brand that is being installed.
     	* @param aElement the element
     	*/
        virtual void AppendElementL( MBSElement* aElement ) = 0;


        /*
        * Closes the handle to server and deletes the object.
        */
		virtual void Close() = 0;
		
    	/*
    	* Register to observer backup restore state change
    	* @param aObserver the observer interface
    	*/
        virtual void RegisterObserverL( MBSBackupRestoreStateObserver* aBackupObserver ) = 0;
		
    	/*
    	* Unregister a backup restore state change observer
    	* @param aObserver the observer interface
    	*/
        virtual void UnRegisterObserverL( MBSBackupRestoreStateObserver* aObserver ) = 0;
		

	protected:
     	/*
     	* Destructor
     	*/
     	virtual ~MBSUpdater() {};
     };

 #endif //__MBSUPDATER_H
