/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApKeyManagement class definition. 
*
*/


#ifndef SYSAPKEYMANAGEMENT_H
#define SYSAPKEYMANAGEMENT_H

// INCLUDES
#include <sysapkeyhandlerplugininterface.h>

// FORWARD DECLARATIONS
class RWindowGroup;
class MSysapCallback;

// DATA TYPES

/**
*  KeyManagement class for Sysap Key Event Framework
*  Handles Virtual KeyEvents with Plugins
*
*  @since S60 3.0
*/    
class CSysApKeyManagement: public CBase
    {
    private: // data types
        class CSysApPluginInfo : public CBase
            {
            public: // construction
                
                /**
        	    * Constructor.
        	    * 
        	    * @param aPluginUid UID of the plugin that this object represents
        	    */
                CSysApPluginInfo( TUid aPluginUid );
                
                /**
	            * Destructor
	            */
                virtual ~CSysApPluginInfo();
            
            private:
                /**
        	    * C++ default constructor. Prohibitten
        	    */
                CSysApPluginInfo();
            
            public: // data
                
                /**
                * UID of the plug-in, used with non-resident plugins.
                */
                TUid iPluginUid;
                
                /**
                * Instantiated plugin, used only with resident plug-ins.
                * If NULL, the plug-in is interpreted to be non-resident.
                * Own.
                */
                CSysapKeyHandlerPlugin* iPtrPlugin;
                                
                /**
                * The keys handled by this plugin.
                */
                RArray<TKeyEvent> iKeys;
                
                /**
                * Handles of the captured keys.
                */
                RArray<TInt32> iKeyHandles; //Handles of keys when capturing them
                
                /**
                * Handles of the captured up and down events.
                */
                RArray<TInt32> iUpAndDownKeyHandles; //Handles of keys when capturing them
                
                RArray<TInt32> iLongKeyPressHandle ; //Handles long key when capturing them
                /**
                 * Tells if the plugin is a key provider plugin (plugin version 2)
                 */
                TBool iIsKeyProvider;
                
            };
            
        typedef RPointerArray<CSysApPluginInfo> RSysApPluginsArray;                        	
        
    public: // Constructors and destructor
     
        /**
        * Default constructor
        *
        * @param aRootWindowGroup reference of the root window group
        * @param aSysApCallback reference to callback object
        * @return pointer to the constructed object
        */
        static CSysApKeyManagement* NewL(RWindowGroup& aRootWindowGroup, MSysapCallback& aSysApCallback);

        /**
        * Destructor
        */
        virtual ~CSysApKeyManagement();	
            
    public: // New functions
            
        /**
        * Handles key events
        *
        * @param aKeyEvent contains information about the key event
        * @param aType contains information about the type of the key event
        * @return response
        */
        TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType );
            
            
    private: // New functions
    		
        /**
        * C++ default constructor.
        *
        * @param aRWindowGroup the root window group
        * @param aSysApCallback reference to callback object
        * @return void
        */
        CSysApKeyManagement(RWindowGroup& aRWindowGroup, MSysapCallback& aSysApCallback);


        /**
        * 2nd phase constructor.
        *
        * @return void
        */
        void ConstructL( );

        /**
        * Registers in windows server for keys to capture.
        *
        * @return void
        */
        void CaptureKeys();

        /**
        * Unregisters in windows server for keys to capture.
        *
        * @return void
        */
        void CancelCaptureKeys();

        /**
        * Gets information of all suitable plugins
        *
        * @return void
        */
        void GetPluginsL();

        /**
        * Finds a key in plugins. Helper function.
        *
        * @param aKeycode key to find
        * @return index of key in array
        */
        TInt FindKeyCode( TUint aKeycode );
        
        /**
        * Finds a key scancode in plugins. Helper function.
        *
        * @param aScanCode key scancode to find
        * @return index of key in array
        */
        TInt FindScanCode( TInt aScanCode );

        /**
        * Get keys from text
        *
        * @param aLex reference to the object to be used, 
        * @param aKeys reference to the array to be filled.
        * @return error code
        */
        TInt GetKeys( TLex8& aLex, RArray<TKeyEvent>& aKeys );
        
        /**
        * Check keys for duplicates and array emptiness
        *
        * @param aKeys reference to the array to be checked.
        * @return error code
        */
        TInt CheckKeys( RArray<TKeyEvent>& aKeys );
    
    private:    // Data

        // Our window group from SysappUi
        RWindowGroup& iRootWindowGroup;
        
        // Reference to SysAp application callback
        MSysapCallback& iSysApCallback;
                        
        /**
        * Key handler plugins managed by SysAp.
        */ 
        RSysApPluginsArray iSysApPlugins;
    };

#endif //SYSAPKEYMANAGEMENT_H

