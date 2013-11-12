/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Presence Service Connection globals holder
 *
*/

#ifndef FEATUREPLUGINHOLDER_H
#define FEATUREPLUGINHOLDER_H

class MApiDataObjFactoryAccessor;

/**
 * Static helper class for accessing the Feature Plug-in datamodel internal interfaces.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v5.0
 */
class FeaturePluginHolder 
    {
    public: 

        /**
         * Initializes the TLS where the pointers to the different accessor objects 
         * from feature plug-ins are stores. The TLS is reference counted.
         *
         *
         */
        IMPORT_C static void InitializeL();

         /**
         * Returns the API Data Model Object Factory Accessor from the presence feature plug-in.
         *
         *
         */
        IMPORT_C static MApiDataObjFactoryAccessor* PresenceObjFactoryAccessor(  );

         /**
         * Cleans the TLS, reference counted.
         *
         *
         */
        IMPORT_C static void Release();
        
         /**
         * Returns the API Data Model Object Factory Accessor count
         *
         *
         */
        IMPORT_C static TInt AccessorCount();
        
         /**
         * Returns the API Data Model Object Factory Accessor from given index
         *
         *
         */
        IMPORT_C static MApiDataObjFactoryAccessor* Accessor(TInt index) ;
        
        
        
    private:

         /**
         * Loads all ecom accessor 
         *
         *
         */
    	void static LoadAccessorPluginsL() ;    
    };

#endif      // FEATUREPLUGINHOLDER_H


