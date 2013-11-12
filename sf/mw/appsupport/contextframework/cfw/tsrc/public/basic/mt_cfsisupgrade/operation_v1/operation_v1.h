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
* Description:  Operation plug-in v1 class declaration.
*
*/


#ifndef OPERATION_V1_H
#define OPERATION_V1_H

// SYSTEM INCLUDE FILES
#include <cfoperationplugin.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class CCFContextObject;

// DATA TYPES

// CLASS DECLARATION

/**
 * Operation test plug-in v1.
 *
 * @lib None.
 * @since S60 5.0
 */
class COperation_V1 : public CCFOperationPlugIn
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return COperation_V1
     */
    static COperation_V1* NewL( TOperationPluginInitParams* aParams );
    static COperation_V1* NewLC( TOperationPluginInitParams* aParams );

    /**
     * C++ destructor.
     */
    virtual ~COperation_V1( );

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

    COperation_V1( TOperationPluginInitParams* aParams );
    void ConstructL( );

private: // Data

    /** Context object as a condition */
    CCFContextObject* iCo;
    };

#endif // OPERATION_V1_H

