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
* Description:  CCFServiceBase class declaration.
*
*/


#ifndef CFSERVICEBASE_H
#define CFSERVICEBASE_H

// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION
class RCFClientSession;
class MCFListener;

/**
 * Base class for all Context Framework client services.
 * These services includes context, action and script services.
 *
 * @lib cfclient.lib
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFServiceBase ): public CBase
    {
public:
    
    // Service type
    enum TCFServiceId
        {
        ECFContextService,
        ECFActionService,
        ECFScriptService,
        ECFNumOfServices
        };

public:

    /**
     * C++ destructor.
     */
    virtual ~CCFServiceBase();

public:

    /**
     * Service ID.
     * 
     * @since S60 5.0
     * @param None.
     * @return TCFServiceId
     */
    TCFServiceId Id() const;

protected:

    CCFServiceBase( TCFServiceId aId,
        RCFClientSession& aSession,
        MCFListener& aListener );

protected: // Data

    /** Session handle */
    RCFClientSession& iSession;
    
    /** Reference to client */
    MCFListener& iListener;

private: // Data

    /** Service ID */
    TCFServiceId iId;    
    };

#endif // CFSERVICEBASE_H

