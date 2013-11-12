/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFOperationPlugIn class declaration
*
*/



#ifndef C_CFOPERATIONPLUGIN_H
#define C_CFOPERATIONPLUGIN_H


#include <e32base.h>
#include <cfoperationpluginconst.hrh>

class CCFOperationNode;
class CCFScriptRoot;
class MCFOperationServices;
class CMDXMLNode;

/**
 * Operation plug-in construction parameters.
 */
class TOperationPluginInitParams
     {
     public:
     
         // Default constructor
         //inline TOperationPluginInitParams( )
         //    {
         //    }
     
     public:
     	TAny* iReserved;
     };
    
/**
* Operation plug-in interface UID.
* Every operation plug-in must implement this interface.
*/
const TUid KOperationPlugInInterface = {KOperationPluginInterfaceUid};
    
/**
 *  Operation plug-in API.
 *
 *  All operation plug-ins must define this interface
 *  to be able to provide new operations to be used in CF rule script language.
 *
 *  @lib cfoperationpluginmanager.lib
 *  @since S60 5.0
 */
class CCFOperationPlugIn : public CBase
    {
    public:

        /**
        * Two phased constructors
        * 
        * @since S60 5.0
        * @param aImplementationUid Implementation to create.
        * @param aParams Construction parameters.
        * @return CCFOperationPlugIn*
        */
        IMPORT_C static CCFOperationPlugIn* NewL(
            const TUid& aImplementationUid,
            TOperationPluginInitParams* aParams );
        IMPORT_C static CCFOperationPlugIn* NewLC(
            const TUid& aImplementationUid,
            TOperationPluginInitParams* aParams );

        // Destructor
        IMPORT_C virtual ~CCFOperationPlugIn();
        
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
        * Parse DOM-node into an operation node.
        *
        * @since S60 5.0
        * @param aParent Parent node.
        * @param aDOMNode DOM-node to be parsed.
        * @param aOperationServices Operation services instance to be used
        * by the new operation node.
        * @return Pointer to Operation node. The ownership of the node is
        * transferred to the caller. NULL must be returned, if the DOM-node
        * cannot be parsed.
        */
        virtual CCFOperationNode* ParseL( CCFOperationNode* aParent,
            CMDXMLNode& aDOMNode, 
            MCFOperationServices& aOperationServices ) = 0;

        /**
        * Parse DOM-node into a script root node.
        *
        * @since S60 5.0
        * @param aParent Parent node.
        * @param aDOMNode DOM-node to be parsed.
        * @param aOperationServices Operation services instance to be used
        * by the new script root node.
        * @return Pointer to ScriptRoot node. The ownership of the node is
        * transferred to the caller. NULL must be returned, if the DOM-node
        * cannot be parsed.
        */
        virtual CCFScriptRoot* ParseScriptRootL( CCFOperationNode* aParent,
            CMDXMLNode& aDOMNode, 
            MCFOperationServices& aOperationServices ) = 0;


    protected:

        IMPORT_C CCFOperationPlugIn( TOperationPluginInitParams* aParams );
        
    protected:
    
        /** Own: Descructor key for Ecom */
        TUid iDtorKey;

        /** Reserved */
        TAny* iReserved1;
        TAny* iReserved2;
        TAny* iReserved3;
    };
    
#endif
