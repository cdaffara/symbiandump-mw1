/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFPublishContextAction class declaration.
*
*/



#ifndef C_CFPUBLISHCONTEXTACTION_H
#define C_CFPUBLISHCONTEXTACTION_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>

#include "cfscriptaction.h"

// FORWARD DECLARATIONS
class CCFContextObject;
class MCFExtendedContextInterface;
class MCFOperationServices;

// CLASS DECLARATION

/**
* Implements PublishContext action.
* Instance of this class is created for every publishContext action in scripts.
*
* @lib BasicOperationsPlugIn
* @since Series 60 5.0
*/
NONSHARABLE_CLASS( CCFPublishContextAction ): public CCFScriptAction
    {
public: // Constructors and destructor
    /**
    * Creates a new CCFPublishContextAction.
    * @param aServices is a reference to script operation services.
    * @param aContextObject is a new TContextObject that will be stored.
    *   Ownership is transferred to CCFPublishContextAction object.
    * @param aAutoDefine Context is automatically defined if not found.
    */
    static CCFPublishContextAction* NewL( MCFOperationServices& aServices,
            CCFContextObject* aContextObject,
            TBool aAutoDefine );

    /**
    * Creates a new CCFPublishContextAction, leaves it into CleanupStack.
    * @param aServices is a reference to script operation services.
    * @param aContextObject is a new TContextObject that will be stored.
    *   Ownership is transferred to CCFPublishContextAction object.
    * @param aAutoDefine Context is automatically defined if not found.
    */
    static CCFPublishContextAction* NewLC( MCFOperationServices& aServices,
            CCFContextObject* aContextObject,
            TBool aAutoDefine );

    /**
    * Destructor.
    */
    ~CCFPublishContextAction();

public: // New functions

    /**
    * Publishes new context into ContextFramework.
    * @see CCFScriptAction
    */
    void ActL();

	// @see CCFScriptAction
	TInt GetSecurityPolicy( TSecurityPolicy& aPolicy );

protected: // New functions

    /**
    * C++ default constructor. 
    */
    CCFPublishContextAction( MCFOperationServices& aServices,
            MCFExtendedContextInterface& aCF,
            CCFContextObject* aContextObject,
            TBool aAutoDefine );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private: // Data

    // Context interface.
    MCFExtendedContextInterface& iCF;

    // Context published by this action.
    CCFContextObject* iContextObject;
    
    // Define context automatically if not found
    TBool iAutoDefine;
    };

#endif // C_CFPUBLISHCONTEXTACTION_H
