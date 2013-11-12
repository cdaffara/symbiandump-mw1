/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class to access the different object factories in the datamodel
*
*/

#ifndef SEARCHAPIDATAOBJFACTORYACCESSOR_H
#define SEARCHAPIDATAOBJFACTORYACCESSOR_H

#include <apidataobjfactoryaccessor.h>

class MEventCodec;
class CSearchEventCodec;
class MApiDataObjFactory;
class CSearchApiDataObjFactory;
class MApiDataObjFactoryAccessor;
/**
 * Class to access the different object factories in the datamodel
 *
 * @lib searchdatamodel.dll
 * 
 */
NONSHARABLE_CLASS( CSearchApiDataObjFactoryAccessor ) : public MApiDataObjFactoryAccessor
    	{
		public:
		    /**
		     ** NewL method 
		     **/
		    IMPORT_C static CSearchApiDataObjFactoryAccessor* NewL();
		    
		    /**
		     * Returns the feature plugin specific event codec object
		     */
 			MEventCodec& EventCodec() ;
    
		    /**
		     * Returns the feature plugin specific api object data factory object
		     */    
		    MApiDataObjFactory& ApiDataObjFactory();
		    /**
		     * destructor
		     */
		    ~CSearchApiDataObjFactoryAccessor();

		private:
            /**
		     * constructor
		     */
		    CSearchApiDataObjFactoryAccessor();
		    /**
		     * construcL 
		     */
		    void ConstructL();
    
		//data
		private:
		// Own
		   CSearchEventCodec* iEventCodec;

		// Own
		 CSearchApiDataObjFactory* iSearchApiDataObjFactory;

	    };

#endif // SEARCHAPIDATAOBJFACTORYACCESSOR_H






