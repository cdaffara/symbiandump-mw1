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
* Description:  CCFAsyncCmdQueue class declaration.
*
*/


#ifndef C_CFASYNCCMDQUEUE_H
#define C_CFASYNCCMDQUEUE_H


// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class CCFCmd;
class MCFContextSubscriptionListener;

// DATA TYPES

// CLASS DECLARATION

/**
 * Queue for commands that need to be executed asynchronously.
 *
 * @lib CFServer
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFAsyncCmdQueue ) : public CActive
{
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CCFAsyncCmdQueue
     */
    static CCFAsyncCmdQueue* NewL();
    static CCFAsyncCmdQueue* NewLC();

    /**
     * C++ destructor.
     */
    virtual ~CCFAsyncCmdQueue();

public: // New functions

    /**
     * Adds command to the end of the asynchronous command queue.
     * Takes the command's ownership.
     *
     * @since S60 5.0
     * @param aCmd is the command to be added. Ownership is transferred.
     * @return None.
     */
    void Add( CCFCmd* aCmd );

    /**
     * Adds command to the front of the asynchronous command queue making it the
     * next one to be executed.
     * Takes the command's ownership.
     *
     * @since S60 5.0
     * @param aCmd is the command to be added. Ownership is transferred.
     * @return None.
     */
    void AddFirst( CCFCmd* aCmd );

private: // Functions from CActive

    // @see CActive.
    void DoCancel();

    // @see CActive.
    void RunL();

    // @see CActive.
    TInt RunError( TInt aError );

private: // New functions

    /**
     * Activates the queue to execute the first command.
     *
     * @since S60 5.0
     * @param None.
     * @return None.
     */
    void Activate();

private:

    CCFAsyncCmdQueue();

private: // Data

    // The queue for asynchronous commands.
    TSglQue< CCFCmd > iQueue;

    // The queue iterator for asynchronous commands.
    TSglQueIter< CCFCmd > iQueueIter;

    // Current command being executed, removed from the queue.
    CCFCmd* iCurrentCmd;
};

#endif // C_CFASYNCCMDQUEUE_H
