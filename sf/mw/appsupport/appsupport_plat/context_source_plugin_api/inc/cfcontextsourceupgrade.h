/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


#ifndef M_CFCONTEXTSOURCEUPGRADE_H
#define M_CFCONTEXTSOURCEUPGRADE_H

// CONSTANTS
const TInt KCFContextSourceUpgradeUidValue = 0x2001599D;
const TUid KCFContextSourceUpgradeUid = {KCFContextSourceUpgradeUidValue};

// FORWARD DECLARATIONS
class CCFContextSourceSettingArray;

/**
* Context Source Upgrade API.
*
* RAM based context source settings (configurations) are dynamically added and
* upgraded via this interface. RAM based settings never hide nor upgrage ROM
* based settings.
*
* CheckValidity is first called for RAM based settings before they are given to
* the plugin. The plugin should check that the settings are valid. It may also
* check client's permission to install the settings, if that is appropriate.
* CheckValidity is called when new settings are installed or the settings
* upgrade older ones. Note that the list of settings given for the validity
* check should not be stored by the plugin.
*
* UpdateSettingsL is called if the call to CheckValidity returned KErrNone or
* RAM based settings have been uninstalled. UpdateSettingsL includes all current
* settings for the plugin. The plugin must take the ownership of the settings
* before calling any code that might leave. Leaving for other reason than out of
* memory is not recommended because the settings are already stored by Context
* Framework, and thus the same list of setting block will be provided to the
* source plugin during next start-up of Context Framework.
*
* @since S60 5.0
* @lib None-
*/
class MCFContextSourceUpgrade
    {
    public: // New functions

        /**
        * Checks setting validity.
        * The settings are either new or upgrading existing RAM based settings.
        * The source plugin should check that the settings are valid (correct
        * attributes and parameters) and if necessary, check client's permission
        * to install the settings (security).
        *
        * @since S60 5.0
        * @param aClientThread Client thread installing new or upgrading
        *   existing RAM based settings.
        * @param aSettingList List of setting blocks, either new settings or
        *   settings upgrading older settings from the same client. Note that
        *   these settings should only be referenced for possible security check
        *   needs, they are not to be copied by the plugin.
        * @return KErrNone if the settings are valid, KErrNotSupported if
        *   settings are invalid, KErrPermissionDenied if client does not have
        *   permission to install the settings, otherwise any system-wide error
        *   error code.
        */
        virtual TInt CheckValidity( RThread& aClientThread,
                const CCFContextSourceSettingArray& aSettingList ) = 0;

        /**
        * Updates complete set of settings for the context source.
        * All current setting blocks defined with the plugin name in the XML
        * settings files will be given in an array. Called if CheckSecurity has
        * succeeded when installing new or upgrading RAM based settings, or RAM
        * based settings have been uninstalled. Ownership of the settings is
        * transferred to the plug-in who should take it before calling any code
        * that might leave.
        *
        * @since S60 5.0
        * @param aSettingList List of setting blocks, ownership is transferred.
        *   Note that the array may be empty (zero setting items) if all RAM
        *   based settings have been uninstalled and the source does not have
        *   any ROM based settings. However, the souce plugin has the
        *   ownership (responsibility) of the array.
        * @return None.
        */
        virtual void UpdateSettingsL(
                CCFContextSourceSettingArray* aSettingList ) = 0;

    protected:

        // Deny destruction through this interface.
        virtual ~MCFContextSourceUpgrade(){};
    };

#endif // M_CFCONTEXTSOURCEUPGRADE_H
