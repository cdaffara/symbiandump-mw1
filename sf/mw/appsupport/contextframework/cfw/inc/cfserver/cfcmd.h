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
* Description:  CCFCmd class declaration.
*
*/


#ifndef C_CFCMD_H
#define C_CFCMD_H


// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class MCFContextSubscriptionListener;

// DATA TYPES

// CLASS DECLARATION

/**
 * Abstract base class for commands.
 *
 * @lib CFServer
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFCmd ) : public CBase
    {
public:

    /**
     * C++ destructor.
     */
    virtual ~CCFCmd( );

public:

    /**
     * Executes the command.
     *
     * @since S60 5.0
     * @param None.
     * @return None.
     */
    virtual void ExecuteL() = 0;

    /**
     * Logs the error using error trace macros.
     *
     * @since S60 5.0
     * @param aError is the leave code from a call to ExecuteL.
     * @return None.
     */
    virtual void LogError( TInt aError );

    /**
     * Returns the singly linked list's link offset.
     *
     * @since S60 5.0
     * @param None.
     * @return Singly linked list's link offset.
     */
    static TInt SglQueLinkOffset();

protected:

    CCFCmd();

private: // Data

    // Singly linked list link object.
    TSglQueLink iSlink;
    };

#endif // C_CFCMD_H
