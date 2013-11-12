/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextSourcePlugIn class declaration
*
*/



#ifndef C_CFCONTEXTSOURCEPLUGIN_H
#define C_CFCONTEXTSOURCEPLUGIN_H


#include <e32base.h>
#include <cfcontextsourcepluginconst.hrh>

class MCFContextInterface;
class MCFContextSourceSetting;
class TCFContextObject;
class TContextSourceInitParams;
class CCFContextSubscription;
class CCFContextSourceSettingArray;

/**
* Context source plug-in construction parameters.
*/
class TContextSourceInitParams
    {
    public:
    
        // Default constructor
        inline TContextSourceInitParams( MCFContextInterface& aCF ):
            iCF( aCF )
            {
            }
    
    public:
    
        // Context manager
        MCFContextInterface& iCF;
    };
    
/**
* Context Source plug-in interface UID.
* Every Context Source plug-in must implement this interface.
*/
const TUid KContextSourcePlugInInterface = {KContextSourcePluginInterfaceUid};
    
/**
 *  Context source plug-in API.
 *
 *  All context source plug-ins must define this interface
 *  to be able to publish contexts and subscribe contexts.
 *  It must be noticed that time consuming actions must not
 *  be done in construction phase. Only context definitions
 *  are allowed. All other initialization is done when InitializeL
 *  is called by Context Framework.
 *  Context source plug-in will be initialized in following order:
 *  - HandleSettingL
 *  - DefineContextsL
 *  - InitializeL
 *
 *  @lib CFContextSourceManager.lib
 *  @since S60 5.0
 */
class CCFContextSourcePlugIn : public CBase
    {
    public:

        /**
        * Two phased constructors
        * 
        * @since S60 5.0
        * @param aImplementationUid Implementation to create.
        * @param aParams Construction parameters.
        * @return CCFContextSourcePlugIn*
        */
        IMPORT_C static CCFContextSourcePlugIn* NewL(
            const TUid& aImplementationUid,
            TContextSourceInitParams* aParams );
        IMPORT_C static CCFContextSourcePlugIn* NewLC(
            const TUid& aImplementationUid,
            TContextSourceInitParams* aParams );

        // Destructor
        IMPORT_C virtual ~CCFContextSourcePlugIn();
        
    public: // New methods
    
        /**
        * Returns an extension interface.
        * The extension interface is mapped with the extension UID.
        *
        * The default implemementation returns NULL.
        *
        * @since S60 5.0
        * @param aExtensionUid: The identifier of the extension.
        * @return Pointer to the extension.
        */
        IMPORT_C virtual TAny* Extension( const TUid& aExtensionUid ) const;

    public: // Implementation specific
    
        /**
        * All contexts by this source must be defined in this stage.
        * Contexts can be defined though MCF interface.
        * Definition must include:
        * - Context source
        * - Context type
        * - Security policy
        * Called after settings are handled.
        *
        * @since S60 5.0
        * @param None
        * @return None
        */
        virtual void DefineContextsL() = 0;
    
        /**
        * Handles settings.
        * All the setting blocks defined with the plug-in name
        * in the XML settings file will be given in an array.
        * Ownership of aSettingList is transferred to the plug-in.
        *
        * @since S60 5.0
        * @param aSettingList List of setting blocks defined
        *  in the settings file.
        * @return None
        */
        virtual void HandleSettingL(
            CCFContextSourceSettingArray* aSettingList ) = 0;
            
        /**
        * Plug-in is allowed to initialize.
        * E.g. start listening some events or triggers.
        *
        * @since S60 5.0
        * @param None
        * @return None
        */
        virtual void InitializeL() = 0;
            
    protected:

        IMPORT_C CCFContextSourcePlugIn( TContextSourceInitParams* aParams );
        
    protected:
    
        /** Own: Descructor key for Ecom */
        TUid iDtorKey;

        /** Ref: CF interface */
        MCFContextInterface& iCF;

        /** Reserved */
        TAny* iReserved1;
        TAny* iReserved2;
        TAny* iReserved3;
    };
    
#endif
