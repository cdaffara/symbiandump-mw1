/*
* Copyright (c) 2005, 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Defines a ECom plug-in interface
*
*/



#ifndef SYSAPKEYHANDLERPLUGININTERFACE_H
#define SYSAPKEYHANDLERPLUGININTERFACE_H

// INCLUDES
#include <ecom/ecom.h>
#include <e32base.h>
#include <coedef.h>
#include <w32std.h>
#include <sysapcallback.h>

//CONSTANTS

//Interface UID for KEF plugins
const TUid KSysapKeyHandlerPluginInterfaceUid = {0x10207131};

// FORWARD DECLARATIONS


//CLASS DECLARATION
/**
*  CSysapKeyHandlerPlugin class. This class serves as an interface between 
*  SysAp and the plugins.
*
*  @since S60 3.1
*/  
 
class CSysapKeyHandlerPlugin : public CBase
    {
    public: // Constructors and destructor
        
        /**
        * Description: Wraps ECom object instantitation. Will return the
        *              default interface implementation, which matches to given
        *              aUid.
        *
        * Note:        This is not a "normal" NewL method, since normally NewL
        *              methods are only defined for concrete classes.
        *              Note that also implementations of this interface
        *              provide NewL methods. They are the familiar NewL's,
        *              which create instance of classes.
        *
        * @since S60 3.1
        * @param aUid Uid of the plugin to be instantiated
        * @param aCallback pointer to object that will implement a callback interface. Cannot be NULL.
        * @return pointer to the created object.
        */
        inline static CSysapKeyHandlerPlugin* NewL( TUid aUid, MSysapCallback* aCallback );

        /**
        * Destructor
        */
        inline virtual ~CSysapKeyHandlerPlugin();

    public:
        /**
        * Handles key events
        *
        * @since S60 3.1
        * @param aKeyEvent contains information about the key event
        * @param aType contains information about the type of the key event
        * @return response
        */
        virtual TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )=0;
        
        /**
        * Provides the key events that the plugin wants to listen to.
        * Plugins should overload this method if they are of version 2 or higher.
        * iCode, iScanCode and iModifiers MUST be defined in each TKeyEvent.
        *
        * @since S60 5.1
        * @return array of key events
        */
        virtual void ProvideKeyEventsL( RArray<TKeyEvent>& aKeyEventArray );
        
    protected:
        /**
        * An additional phase for derived class construction.
        * 
        * This method is called immediately after the plug-in has been loaded. 
        * Derived classes may overload this method, if they need to perform initializations
        * that require usage of the callback pointer (obtained via SysapCallback()-function).
        * 
        * @since S60 3.1
        * @leave system wide error code
        * @see CSysapKeyHandlerPlugin::SysapCallback
        */
        virtual void PostConstructL() {}
        
        /**
        * Returns pointer to callback object.
        * 
        * @since S60 3.1
        * @return MSysapCallback*
        * @pre plug-in hase been loaded successfully
        * @panic KErrGeneral raised if method is called during construction.
        *                    Place the initialization code that needs this function into PostConstructL().
        * @see CSysapKeyHandlerPlugin::PostConstructL
        */
        inline MSysapCallback* SysapCallback();
        

    private:
        /** 
        * Instance identifier key. When instance of an
        * implementation is created by ECOM framework, the
        * framework will assign UID for it. The UID is used in
        * destructor to notify framework that this instance is
        * being destroyed and resources can be released.
        */
        TUid iDtor_ID_Key;
        
        /**
        * Pointer to callback object.
        * Not own.
        */
        MSysapCallback* iCallback;
        };

// This includes the implementation of the instantiation functions and
// destructor
#include <sysapkeyhandlerplugininterface.inl>

#endif 

