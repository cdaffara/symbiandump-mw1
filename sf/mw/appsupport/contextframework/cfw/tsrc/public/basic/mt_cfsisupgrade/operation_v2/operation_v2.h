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
* Description:  Operation plug-in v2 class declaration.
*
*/


#ifndef OPERATION_V2_H
#define OPERATION_V2_H

// SYSTEM INCLUDE FILES
#include <cfoperationplugin.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class CCFContextObject;

// DATA TYPES

// CLASS DECLARATION

/**
 * Operation test plug-in v2.
 *
 * @lib None.
 * @since S60 5.0
 */
class COperation_V2 : public CCFOperationPlugIn
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return COperation_V2
     */
    static COperation_V2* NewL( TOperationPluginInitParams* aParams );
    static COperation_V2* NewLC( TOperationPluginInitParams* aParams );

    /**
     * C++ destructor.
     */
    virtual ~COperation_V2( );

public: // From CCFOperationPlugIn

    // @see CCFOperationPlugIn
    CCFOperationNode* ParseL( CCFOperationNode* aParent,
        CMDXMLNode& aDOMNode, 
        MCFOperationServices& aOperationServices );
    
    // @see CCFOperationPlugIn
    CCFScriptRoot* ParseScriptRootL( CCFOperationNode* aParent,
        CMDXMLNode& aDOMNode, 
        MCFOperationServices& aOperationServices );
    
private:

    COperation_V2( TOperationPluginInitParams* aParams );
    void ConstructL( );

private: // Data

    /** Context object as a condition */
    CCFContextObject* iCo;
    };

#endif // OPERATION_V2_H

