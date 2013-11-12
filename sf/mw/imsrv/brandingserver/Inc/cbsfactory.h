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
* Description:  cbsfactory.h
*
*/


#ifndef __CBSFACTORY_H
#define __CBSFACTORY_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MBSAccess;
class MBSUpdater;
class RBSClient;


// CLASS DECLARATION

class CBSFactory : public CBase
    {

	public:

		/**
		* Two-phased constructor.
		* @param aDefaultBrandId the default brand id for branding interfaces
		* 		 that are created through this factory. If this parameter
		*        is given the branding items will be taken from the given
		*		 default brand if they are not found from the wanted brand.
		*        brand.
		* @param aApplicationId application id
		*/
		IMPORT_C static CBSFactory* NewL( const TDesC8& aDefaultBrandId,
										  const TDesC8& aApplicationId );

		// destructor
    	IMPORT_C virtual ~CBSFactory();

	private:

		// constructor
		CBSFactory();

    	void ConstructL( const TDesC8& aDefaultBrandId, const TDesC8& aApplicationId );


    public:

		/**
		* Creates a brand access interface
		* @param aBrandId the brand to be used with this interface instance
		* @param aLanguageId the language id to be used with this interface instance
		* @param aCacheData should the interface cache the branding data
		* @param aReserved for further use		
		* @return the access interface
		*/
		IMPORT_C MBSAccess* CreateAccessL( const TDesC8& aBrandId,
											 TLanguage aLanguageId,
        					  		   		 TBool aCacheData = EFalse,
        					  		   		 TInt aReserved = 0 );

		/**
		* Creates a brand access interface
		* @param aBrandId the brand to be used with this interface instance
		* @param aLanguageId the language id to be used with this interface instance
		* @param aCacheData should the interface cache the branding data
		* @param aReserved for further use				
		* @return the access interface
		*/
		IMPORT_C MBSAccess* CreateAccessLC( const TDesC8& aBrandId,
											 TLanguage aLanguageId,
        					  		   		 TBool aCacheData = EFalse,
        					  		   		 TInt aReserved = 0 );

		/**
		* Creates a brand update interface
		* @return the interface
		*/
		IMPORT_C MBSUpdater* CreateUpdaterL();

		/**
		* Creates a brand update interface
		* @return the interface
		*/
		IMPORT_C MBSUpdater* CreateUpdaterLC();


	private: // data
		HBufC8* 	iDefaultBrand;
		bool		iUseDefaultBrand;
		HBufC8*  iApplicationId;
		RBSClient* iServerKeepAlive;

    };

#endif      // __CBSFACTORY_H

// End of File
