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
* Description:  CCFRemoveSubscriptionCleanupCmd class declaration.
*
*/


#ifndef C_CFREMOVESUBSCRIPTIONCLEANUPCMD_H
#define C_CFREMOVESUBSCRIPTIONCLEANUPCMD_H


// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES
#include "cfcmd.h"

// FORWARD DECLARATIONS
class CCFContextManager;

// DATA TYPES

// CLASS DECLARATION

/**
 * Remove subscription cleanup command.
 *
 * @lib CFServer
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFRemoveSubscriptionCleanupCmd ) : public CCFCmd
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param aContextManager is the manager handling context subscriptions.
     * @return CCFRemoveSubscriptionCleanupCmd
     */
    static CCFRemoveSubscriptionCleanupCmd* NewL(
            CCFContextManager& aContextManager );
    static CCFRemoveSubscriptionCleanupCmd* NewLC(
            CCFContextManager& aContextManager );

    /**
     * C++ destructor.
     */
    virtual ~CCFRemoveSubscriptionCleanupCmd();

public: // From CCFCmd

    /**
     * Executes the remove subscription cleanup command.
     *
     * @since S60 5.0
     * @param None.
     * @return None.
     */
    virtual void ExecuteL();

private:

    CCFRemoveSubscriptionCleanupCmd( CCFContextManager& aContextManager );

private: // Data

    // Context manager handling context subscriptions.
    CCFContextManager& iContextManager;
    };

#endif // C_CFREMOVESUBSCRIPTIONCLEANUPCMD_H
