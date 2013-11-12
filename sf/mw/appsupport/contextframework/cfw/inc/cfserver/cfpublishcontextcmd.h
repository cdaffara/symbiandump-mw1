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
* Description:  CCFPublishContextCmd class declaration.
*
*/


#ifndef C_CFPUBLISHCONTEXTCMD_H
#define C_CFPUBLISHCONTEXTCMD_H


// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES
#include "cfcmd.h"

// FORWARD DECLARATIONS
class CCFContextManager;
class CCFContextObject;
class CCFContextDataObject;
class CCFContextDataProxy;

// DATA TYPES

// CLASS DECLARATION

/**
 * Publish context command.
 *
 * @lib CFServer
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFPublishContextCmd ) : public CCFCmd
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param aContextManager is the manager handling context subscriptions.
     * @param aContext is the context to be published.
     * @return CCFPublishContextCmd
     */
    static CCFPublishContextCmd* NewL( CCFContextManager& aContextManager,
            const CCFContextObject& aContext );
    static CCFPublishContextCmd* NewLC( CCFContextManager& aContextManager,
            const CCFContextObject& aContext );

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param aContextManager is the manager handling context subscriptions.
     * @param aContext is the context to be published.
     * @param aData is the context data to be published.
     * @return CCFPublishContextCmd
     */
    static CCFPublishContextCmd* NewL( CCFContextManager& aContextManager,
            const CCFContextObject& aContext,
            CCFContextDataObject& aData );
    static CCFPublishContextCmd* NewLC( CCFContextManager& aContextManager,
            const CCFContextObject& aContext,
            CCFContextDataObject& aData );

    /**
     * C++ destructor.
     */
    virtual ~CCFPublishContextCmd();

public: // From CCFCmd

    /**
     * Executes the publish context command.
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

    CCFPublishContextCmd( CCFContextManager& aContextManager );
    void ConstructL( const CCFContextObject& aContext );
    void ConstructL( const CCFContextObject& aContext,
            CCFContextDataObject& aData );

private: // Data

    // Context manager handling context publications.
    CCFContextManager& iContextManager;

    // Context published by this command; owned.
    CCFContextObject* iContextObject;

    // Context data; owned, optional: NULL = no data.
    CCFContextDataProxy* iData;
    };

#endif // C_CFPUBLISHCONTEXTCMD_H
