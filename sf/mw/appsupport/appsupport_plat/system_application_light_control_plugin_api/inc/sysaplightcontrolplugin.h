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
* Description:  Light control extension plugin interface
*
*/



#ifndef SYSAPLIGHTCONTROLPLUGIN_H
#define SYSAPLIGHTCONTROLPLUGIN_H

/*****************************************************
*       S60 Customer
*       Needs customer implementation
*****************************************************/

#include <e32base.h>
#include <ecom/ecom.h>
#include <sysaplightcontrolpluginconstants.hrh>
#include <sysaplightextensionenums.h>

/**
*  Base class for System Application Light Control plugin implementation.
*
*  A plugin that inherits this base class is able to implement customized system
*  light control. 
*
*  Plugin will receive light events and commands via interface methods. 
*  
*  Light event is signaled when such a system state change occurs that 
*  System Application is about to change state of lights.
*   
*  A light event is followed by one or several light commands. Light command's purpose is to 
*  inform the plug-in about the light controlling measure System Application is about to take. 
*  Plugin can then either allow or deny the action with the return value of the notification method.
*
*  Below is an example how plugin can alter System Application's way of handling lights. In this scenario
*  the plugin wishes to blink lights three times every time when an accessory is connected.
*  In order to achieve this plugin monitors EAccessoryConnection light event for setting internal state variable.
*  When the consequential light command is notified, plugin uses HWRM Light API for setting the lights to blink 
*  and gives a response to System Application that it must not execute light commands related to this event.
*
*  @code
*       void CSysApTestLightPluginAccDemo::LightEventL( TInt aEvent, const TDesC8& aData )
*           {
*           switch ( aEvent )
*               {
*               case SysApLightExtension::EAccessoryConnection:
*                   {
*                   TPckgBuf<TBool> connected;
*                   connected.Copy( aData );
*                   iConnected = connected(); // blink if accessory is just connected    
*                   }
*                   break;
*                    
*               default:
*                   iConnected = EFalse;
*                   break;
*               }
*           }
*
*       TInt CSysApTestLightPluginAccDemo::HandleLightCommandL( TInt aCommand, const TDesC8& aData )
*           {
*           TInt retval( SysApLightExtension::ECommandNotHandled );
*            
*           // Blink lights only in case System Application would set lights on when an accessory is connected
*           if ( aCommand == SysApLightExtension::ELightCommandOn && iConnected )
*               {
*               CHWRMLight* lightApi = CHWRMLight::NewLC();
*                
*               lightApi->LightOnL( CHWRMLight::ESystemTarget ); // ensure that lights are on after blinking
*
*               lightApi->LightBlinkL( CHWRMLight::ESystemTarget, // blink lights for three times
*                                      1200, // ms
*                                      200, // ms
*                                      200, // ms
*                                      KHWRMDefaultIntensity );
*                
*               CleanupStack::PopAndDestroy( lightApi );
*
*               retval = SysApLightExtension::ECommandHandled;
*               }
*            
*           return retval;
*           }
*  @endcode
*
*  @since S60 3.2
*  @lib None.
*/
class CSysApLightControlPlugin : public CBase
    {
    public:  // construction and destruction
    
        /**
        * Symbian two-phased constructor.
        *
        * This method uses ECom for finding and creating the instance
        * of the plugin. 
        *
        * @since S60 v3.2
        * @return New CSysApLightControlPlugin instance.
        * @leave Standard Symbian leaves.
        */
        inline static CSysApLightControlPlugin* NewL();

        /**
        * Destructor
        */
        inline virtual ~CSysApLightControlPlugin();       

        /**
        * A light event has occurred.
        *
        * A light event is a system state change relevant to System Application
        * light control, for example user activity or accessory connection.
        * 
        * Plugin can use information received via this method for preparing its internal
        * state before the actual light command is notified.
        *
        * @since S60 3.2
        * @param aEvent event to be handled (defined in TLightEvent). 
        * @param aData  data related to the event.
        *
        * @see TLightEvent
        * 
        * @leave a Symbian error code. Leaves are not propagated.   
        */
        virtual void LightEventL( TInt aEvent, const TDesC8& aData ) = 0;
        
        /**
        * A light command is about to be executed by System Application.
        *
        * A light commad is the concrete light controlling action (on, off or blink)
        * that System Application will take after a light event. This method is called
        * with a special value (ELightCommandNoAction) also in case the light status
        * should not be changed due to a light event. 
        *
        * @since S60 3.2
        *
        * @param aCommand the light command (defined in TLightCommand).
        * @param aData data related to the command.
        *
        * @return SysApLightExtension::ECommandHandled    command was handled by plug-in
        *         SysApLightExtension::ECommandNotHandled command was not handled by plug-in
        *
        * @see TLightCommand
        * @see TLightCommandResponse
        *
        * @leave a Symbian error code. Leaves are not propagated. If a leave occurs,
        *        System Application will continue light processing as it had received
        *        return value ECommandHandled.      
        */
        virtual TInt HandleLightCommandL( TInt aCommand, const TDesC8& aData ) = 0;

   protected:
   
        /**
        * Destructor identifier to be used with ECom framework.
        */
        TUid iDestructorIDKey;
    };

#include <sysaplightcontrolplugin.inl>

#endif      // SYSAPLIGHTCONTROLPLUGIN_H   
            
// End of File
