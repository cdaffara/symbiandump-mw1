/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  factory class to create access and updateer class
*
*/



#ifndef __CIMCACHEFACTORY_H
#define __CIMCACHEFACTORY_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MIMCacheUpdater;
class CIMCacheClient;
class CIMCacheUpdater;
class MIMCacheAccessor;
class CIMCacheAccessor;
// CLASS DECLARATION

class CIMCacheFactory : public CBase
    {

	public:
    	
    	/**
		* read the instance from tls if exist or create return
	    * if exist instace return or create return
		*/
    	IMPORT_C static CIMCacheFactory* InstanceL() ;
    	
       /**
		* release the all instaces create by client factory
		* @param aApplicationId application id
		*/
	    	
    	IMPORT_C static void  Release();

	private:
	
		/**
		* two phase construction
	    * return the new of CIMCacheFactory 
		*/
		 static CIMCacheFactory* NewL();

		/**
		* destructor
	   	*/
		 ~CIMCacheFactory();

		/**
		* default constructor
	   	*/
		 CIMCacheFactory();
		
		/**
		* ConstructL
	   	*/
    	 void ConstructL();
    	 
    	 /**
    	  * IncrementReference.
    	  * Increments the count of the imcache factory reference.
    	  */
    	 void IncrementReference();

    	 /**
    	  * DecrementReference.
    	  * Decrements the count of the imcache factory reference.
    	  * @return - Current Reference Count
     	  */
    	 TInt DecrementReference();

 public:
   		/**
		* Creates a updater interface
		* @param aServiceId ,the service id to be used with this interface instance
		* @param aUserId the aUserId to be used with this interface instance
		* @param aRegistrationNeeded , used for register ,iunregister e
		* @return the access interface
		*/
		IMPORT_C MIMCacheUpdater* CreateUpdaterL(TInt aServiceId,
												const TDesC& aUserId ,
												TBool aRegistrationNeeded );
	
		/**
		* Creates a accessor interface
		* @param aServiceId ,the service id to be used with this interface instance
		* @param aUserId the aUserId to be used with this interface instance
		* @return the access interface
		*/
		EXPORT_C MIMCacheAccessor* CreateAccessorL(TInt aServiceId,
													const TDesC& aUserId  );
	private: // data
		
		// owned , instantmessagingcache client
		CIMCacheClient* iServerKeepAlive;
		
		// owned , array to hold updater instaces  
		RPointerArray<CIMCacheUpdater> iUpdaterArray;
		
		// owned , array to hold accessor instaces  
		RPointerArray<CIMCacheAccessor> iAccessorArray;
		
		// Owns : factory reference count
		TInt iReferenceCount;

    };

#endif      // __CIMCACHEFACTORY_H

// End of File
