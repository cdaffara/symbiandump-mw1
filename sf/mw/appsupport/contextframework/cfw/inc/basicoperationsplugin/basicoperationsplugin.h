/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Plugin for basic script operations
*
*/



#ifndef C_BASICOPERATIONSPLUGIN_H
#define C_BASICOPERATIONSPLUGIN_H

// INCLUDES
//#include <viewcli.h>

#include <cfoperationplugin.h>


// CLASS DECLARATION

/**
* Basic operations plugin.
* 
* @lib -
* @since S60 5.0
*/
NONSHARABLE_CLASS( CBasicOperationsPlugIn ) : public CCFOperationPlugIn
    {
    public: // Constructors and destructor

        // Two phased constructors
        static CBasicOperationsPlugIn* NewL( 
        		TOperationPluginInitParams* aParams );
        static CBasicOperationsPlugIn* NewLC( 
        		TOperationPluginInitParams* aParams );

        // Destructor
        ~CBasicOperationsPlugIn();

    public: // From CCFOperationPlugIn

        // @see CCFOperationPlugIn
    	CCFOperationNode* ParseL( CCFOperationNode* aParent,
            CMDXMLNode& aDOMNode, 
            MCFOperationServices& aOperationServices );

        // @see CCFOperationPlugIn
        CCFScriptRoot* ParseScriptRootL( CCFOperationNode* aParent,
            CMDXMLNode& aDOMNode, 
            MCFOperationServices& aOperationServices );


    private: // Constructors

        // C++ constrcutor
        CBasicOperationsPlugIn( TOperationPluginInitParams* aParams );

        // Symbian second-phase constructor
        void ConstructL();

    private: // New functions


    private: // Data

    };

#endif // C_BASICOPERATIONSPLUGIN_H
