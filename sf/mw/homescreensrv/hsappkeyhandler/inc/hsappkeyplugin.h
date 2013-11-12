/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Main plugin class
*
*/

#ifndef HSAPPKEYPLUGIN_H
#define HSAPPKEYPLUGIN_H

#include <sysapkeyhandlerplugininterface.h>
#include <hb/hbcore/hbdevicedialogsymbian.h>

class CEikonEnv;

/**
*  CAppKeyPlugin
*  An implementation of the CSysapKeyHandlerPlugin definition. The plugin handles EKeyXXX events.
*  This is concrete class, instance of which
*  ECOM framework gives to ECOM clients.
*  @lib CAppKeyPlugin
*  @since Series 60 version 3.1
*/
class CHsAppKeyPlugin : public CSysapKeyHandlerPlugin,
                        public MHbDeviceDialogObserver
    {
public: // Constructors and destructor
    /**
    * Two-phased constructor.
    */
    static CHsAppKeyPlugin* NewL();

    /**
    * Destructor.
    */
    virtual ~CHsAppKeyPlugin();

public: //MHbDeviceDialogObserver interfaces
    void DataReceived( CHbSymbianVariantMap& aData );
    void DeviceDialogClosed( TInt aCompletionCode );

public: // New functions
    /**
    * HandleKeyEventL
    * @since Series 60 Series60_version 3.0
    * @param const TKeyEvent& aKeyEvent, TEventCode aType
    * @return TKeyResponse If key was consumed. Generally it should be, since the KEF calls the plugins only with the right key.
    */
    virtual TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent,
                                          TEventCode aType );

    /**
    * Provides the key events that the plugin wants to listen to.
    * Plugins should overload this method if they are of version 2 or higher.
    * iCode, iScanCode and iModifiers MUST be defined in each TKeyEvent.
    *
    * @since S60 5.1
    * @return array of key events
    */
    void ProvideKeyEventsL( RArray<TKeyEvent>& aKeyEventArray );

private: // Constructors
    /**
    * C++ default constructor.
    */
    CHsAppKeyPlugin();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private: // helper methods
    void HandleShortPressL();
    void HandleLongPressL();
    void DismissTaskSwitcher();

private: //data
    CEikonEnv* iEikEnv;
    TBool iSecondEvent;
    CHbDeviceDialogSymbian* iDialog;
    };

#endif // HSAPPKEYPLUGIN_H
