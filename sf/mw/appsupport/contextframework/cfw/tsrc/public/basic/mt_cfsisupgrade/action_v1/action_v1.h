/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef ACTION_V1_H
#define ACTION_V1_H

// INCLUDE FILES
#include <CFActionPlugin.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 * 
 */
class CAction_V1 : public CCFActionPlugIn
    {
public:
    // Constructors and destructor

    static CAction_V1* NewL( );
    static CAction_V1* NewLC( );

    virtual ~CAction_V1( );

public: // Functions from CActionPlugIn

    // From CActionPlugIn
    void InitializeL( );

    // From CActionPlugIn
    void PrepareExecutionL( );

    // From CActionPlugIn
    TExecutionTime ExecuteL( CCFActionIndication* aActionIndication );

    // From CActionPlugIn
    void FinishedExecution( );

    // From CActionPlugIn
    void GetActionsL( CDesCArray& aActionList ) const;

    // From CActionPlugIn
    const TSecurityPolicy& SecurityPolicy( ) const;

protected:
    // Constructors

    // C++ constrcutor
    CAction_V1( );

    // 2nd phase constructor
    void ConstructL( );

private:

    TSecurityPolicy iSecurityPolicy;
    };

#endif // C_TESTACTIONPLUGIN_H

