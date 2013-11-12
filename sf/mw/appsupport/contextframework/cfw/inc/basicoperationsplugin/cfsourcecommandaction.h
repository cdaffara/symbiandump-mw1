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
* Description:  CCFSourceCommandAction class declaration.
 *
*/


#ifndef C_CFSOURCECOMMANDACTION_H
#define C_CFSOURCECOMMANDACTION_H


// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES
#include "cfscriptaction.h"

// FORWARD DECLARATIONS
class MCFOperationServices;
class CCFContextSourceCommand;

// DATA TYPES

// CLASS DECLARATION

/**
 * Source command action to send commands to context sources, e.g. to activate
 * or deactivate the context source.
 *
 * @lib BasicOperationsPlugIn
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFSourceCommandAction ) : public CCFScriptAction
{
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param aServices is a reference to script operation services.
     * @param aCommand is the context source command. Ownership is transferred.
     * @return CCFSourceCommandAction
     */
    static CCFSourceCommandAction* NewL( MCFOperationServices& aServices, 
            CCFContextSourceCommand* aCommand );

    static CCFSourceCommandAction* NewLC( MCFOperationServices& aServices, 
            CCFContextSourceCommand* aCommand );

    /**
     * C++ destructor.
     */
    virtual ~CCFSourceCommandAction();

public: // Functions from CCFScriptAction

    /**
    * Notifies the action handler about new action.
    * @see CCFScriptAction::ActL
    */
    void ActL();

    // @see CCFScriptAction::GetSecurityPolicy
    TInt GetSecurityPolicy( TSecurityPolicy& aPolicy );

private:

    CCFSourceCommandAction( MCFOperationServices& aServices );
    void ConstructL( CCFContextSourceCommand* aCommand );

private: // Data

    // Context source command; owned.
    CCFContextSourceCommand* iCommand;
    };

#endif // C_CFSOURCECOMMANDACTION_H
