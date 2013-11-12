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
* Description:  CCFContextSourceCmd class declaration.
*
*/


#ifndef C_CFCONTEXTSOURCECMD_H
#define C_CFCONTEXTSOURCECMD_H


// SYSTEM INCLUDE FILES
#include <e32base.h>
#include <cfcontextsourcecommand.h>

// USER INCLUDE FILES
#include "cfcmd.h"

// FORWARD DECLARATIONS
class MCFContextSourceCommand;

// DATA TYPES

// CLASS DECLARATION

/**
 * Context source command.
 *
 * @lib CFServer
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFContextSourceCmd ) : public CCFCmd
{
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param aSourceCommandHandler is the context source command handler to
     *   process the command.
     * @param aCommand is the context source command action.
     * @return CCFContextSourceCmd
     */
    static CCFContextSourceCmd* NewL(
            MCFContextSourceCommand& aSourceCommandHandler,
            const CCFContextSourceCommand& aCommand );
    static CCFContextSourceCmd* NewLC(
            MCFContextSourceCommand& aSourceCommandHandler,
            const CCFContextSourceCommand& aCommand );

    /**
     * C++ destructor.
     */
    virtual ~CCFContextSourceCmd();

public: // From CCFCmd

    /**
     * Executes the context source command.
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

    CCFContextSourceCmd( MCFContextSourceCommand& aSourceCommandHandler );
    void ConstructL( const CCFContextSourceCommand& aCommand );

private: // Data

    // Reference to the context source command handler.
    MCFContextSourceCommand& iContextSourceCommandHandler;

    // Context source command; owned.
    CCFContextSourceCommand* iCommand;
    };

#endif // C_CFCONTEXTSOURCECMD_H
