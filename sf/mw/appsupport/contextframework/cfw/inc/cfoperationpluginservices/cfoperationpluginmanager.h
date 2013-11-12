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
* Description:  CFContextSourceManager class declaration.
*
*/



#ifndef C_CFOPERATIONPLUGINMANAGER_H
#define C_CFOPERATIONPLUGINMANAGER_H

#include "cfecompluginmanager.h"
#include "cfpluginoperation.h"
#include "cfstarterobserver.h"
#include "cfoperationplugin.h"
#include "cfstateobserver.h"

class CCFOperationPlugIn;
class MCFOperationServices;
class TCFOperationPluginInfo;
class CImplementationInformation;
class MCFScriptEngineInterface;

/**
 *  Operation plug-in manager.
 *
 *  Operation plug-in manager is responsible for loading all the
 *  operation plug-ins that are currently installed.
 *
 *  @lib cfscriptengine.lib
 *  @since S60 4.0
 */
NONSHARABLE_CLASS( CCFOperationPluginManager ) : public CCFEComPluginManager,
    public MCFPlugInOperation,
    public MCFStarterObserver,
    public MCFStateObserver
    {
    public:

        // Two phased constrcutors
        IMPORT_C static CCFOperationPluginManager* NewL(
            MCFScriptEngineInterface& aScriptEngine );
        IMPORT_C static CCFOperationPluginManager* NewLC(
            MCFScriptEngineInterface& aScriptEngine );

        // Destructor
        IMPORT_C virtual ~CCFOperationPluginManager();

    public: // From MCFPlugInOperation

        /**
        * Parses a xml node and returns a pointer to CCFScriptRoot instance.
        * @since S60 5.0
        * @param aParent is the parsed parent node for the aNode.
        * @param aNode is the node to be parsed.
        * @param aOperationServices is the operation services reference to be
        *   used by the new script root node.
        * @param aUid Implementation uid of the provider.
        * @return a ptr to CCFScriptRoot instance parsed from the given node,
        *   note that the ownership is transferred to the caller. Returns NULL
        *   if the node cannot be parsed.
        */
        CCFScriptRoot* ParseScriptRootL( CCFOperationNode* aParent,
                CMDXMLNode& aNode,
                MCFOperationServices& aOperationServices,
                TUid& aUid );

        /**
        * Parse DOM-node into an operation node.
        *
        * @since S60 5.0
        * @param aParent Parent node.
        * @param aDOMNode DOM-node to be parsed.
        * @param aOperationServices Operation services instance to be used
        *   by the new operation node.
        * @param aUid Implementation uid of the provider.
        * @return Pointer to Operation node. The ownership of the node is
        *   transferred to the caller. NULL must be returned, if the DOM-node
        *   cannot be parsed.
        */
        CCFOperationNode* ParseL( CCFOperationNode* aParent,
            CMDXMLNode& aDOMNode,
            MCFOperationServices& aOperationServices,
            TUid& aUid );

    public: // From base classes

        // from MCFStateObserver
        void UpdatePlugInsL();

    private: // From base classes

        // @see MCFInitialize
        void InitializePhaseL( CCFPhaseBase::TCFPhaseId aPhase );
        
        // @see MCFStarterObserver
        void SetEventHandler( MCFStarterEventHandler& /*aEventHandler*/ );

    private:

        CCFOperationPluginManager( MCFScriptEngineInterface& aScriptEngine );
        void ConstructL();

    private: // New methods

        // Loads and initialized plug-in
    	CCFOperationPlugIn* CreateAndInitializePlugInL(
            const TUid& aImplementationUid,
            TOperationPluginInitParams& aParams );

        // Init starting phase
        void InitDeviceStartingPhaseL();

        // Init loading plugins phase
        void InitLoadingPluginsPhaseL();

        // Loads plug-ins and appends it into array
        void LoadPluginL( CImplementationInformation& aImplInfo );

        // Releases operation plug-in
        void ReleasePlugIn( CCFOperationPlugIn*& aPlugIn );

        // Finalizes node parsing
        CCFOperationNode* DoParseL( CCFOperationNode* aParent,
            CMDXMLNode& aDOMNode,
            MCFOperationServices& aOperationServices,
            TUid& aUid );

        // Finalizes script root parsing
        CCFScriptRoot* DoParseScriptRootL( CCFOperationNode* aParent,
            CMDXMLNode& aDOMNode,
            MCFOperationServices& aOperationServices,
            TUid& aUid );

    private: // data

        // Own: Context source plugin info array
        RArray<TCFOperationPluginInfo> iPlugIns;

        // Ref: Reference to script engine
        MCFScriptEngineInterface& iScriptEngine;
    };

#endif
