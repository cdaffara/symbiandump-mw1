/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionCmd class declaration.
*
*/


#ifndef C_CFACTIONCMD_H
#define C_CFACTIONCMD_H


// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES
#include "cfcmd.h"

// FORWARD DECLARATIONS
class MCFActionHandler;
class CCFScriptEvent;

// DATA TYPES

// CLASS DECLARATION

/**
 * Action command.
 *
 * @lib CFServer
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFActionCmd ) : public CCFCmd
{
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param aActionHandler is the action handler to process the action.
     * @param aEvent is the action. Ownership is transferred.
     * @return CCFActionCmd
     */
    static CCFActionCmd* NewL( MCFActionHandler& aActionHandler,
            CCFScriptEvent* aEvent );
    static CCFActionCmd* NewLC( MCFActionHandler& aActionHandler,
            CCFScriptEvent* aEvent );

    /**
     * C++ destructor.
     */
    virtual ~CCFActionCmd();

public: // From CCFCmd

    /**
     * Executes the action command.
     *
     * @since S60 5.0
     * @param None.
     * @return None.
     */
    virtual void ExecuteL();

    /**
     * Logs the error using error trace macros.
     *
     * @since S60 5.0
     * @param aError is the leave code from a call to ExecuteL.
     * @return None.
     */
    virtual void LogError( TInt aError );

private:

    CCFActionCmd( MCFActionHandler& aActionHandler,
            CCFScriptEvent* aEvent );

private: // Data

    // Reference to the action handler.
    MCFActionHandler& iActionHandler;

    // The action (script event); owned.
    CCFScriptEvent* iEvent;
    };

#endif // C_CFACTIONCMD_H
