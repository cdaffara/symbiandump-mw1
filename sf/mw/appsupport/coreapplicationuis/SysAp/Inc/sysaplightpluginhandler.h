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
* Description:  System Application light enxtension plugin handler
*
*/


#ifndef SYSAPLIGHTPLUGINHANDLER_H
#define SYSAPLIGHTPLUGINHANDLER_H


#include <e32base.h>
#include <sysaplightextensionenums.h>

class CSysApLightControlPlugin;



/**
 *  Light control extension plugin handler.
 *
 *  This class encapsulates managing of light control
 *  extension plugin.
 *
 *  @lib None.
 *  @since S60 3.2
 */
class CSysApLightPluginHandler : public CBase
    {

public:
    /**
    * Symbian two-phased constructor
    */
    static CSysApLightPluginHandler* NewL();

    /**
    * Destructor
    */
    virtual ~CSysApLightPluginHandler();

    /**
    * Passes the light event to the plugin.
    *
    * @since S60 3.2
    * @param aLightEvent the light event
    * @param aData parameter data, depends on the light event
    */
    void HandleEvent( SysApLightExtension::TLightEvent aLightEvent, const TDesC8& aData = KNullDesC8 );

    /**
    * Passes the light event to the plugin and completes it with command ELightCommandNoAction.
    * This method can be used for substituting subsequent calls for HandleEvent and HandleCommand,
    * in case latter one would be used for signaling no light commands taking place.
    *
    * @since S60 3.2
    * @param aLightEvent the light event
    * @param aData parameter data, depends on the light event
    */
    void HandleEventNoAction( SysApLightExtension::TLightEvent aLightEvent, const TDesC8& aData = KNullDesC8 );
    
    /**
    * Passes the light command to the plugin.
    *
    * @since S60 3.2
    *
    * @param aLightCommand the light command
    * @param aData parameter data, depends on the light command
    */
    TBool HandleCommand( SysApLightExtension::TLightCommand aLightCommand, const TDesC8& aData = KNullDesC8 );
    
private:
    
    /**
    * C++ default constructor.
    * Not implemented.
    */
    CSysApLightPluginHandler();
    
    /**
    * Symbian 2nd-phase constructor.
    */
    void ConstructL();

private: // data
    
    /**
    * Light control plugin.
    * Own.
    */
    CSysApLightControlPlugin* iPlugin;
    
    /**
    * Event completion status. An event must always be followed
    * by one or several commmands.
    */
    TBool iEventCompleted;
    };



#endif // SYSAPLIGHTPLUGINHANDLER_H
