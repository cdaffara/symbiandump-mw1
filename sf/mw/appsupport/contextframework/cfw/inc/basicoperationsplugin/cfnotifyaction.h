/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFNotifyAction class declaration.
*
*/



#ifndef C_CFNOTIFYACTION_H
#define C_CFNOTIFYACTION_H

// INCLUDES
#include <e32base.h>
#include <e32std.h>

#include "cfscriptaction.h"

// FORWARD DECLARATIONS
class MCFOperationServices;
class CCFKeyValuePair;

// CLASS DECLARATION

/**
*   Implements notification action.
*   Instance of this class is created for every notifyScript action in scripts.
*
*  @lib BasicOperationsPlugIn
*  @since Series 60 5.0
*/
NONSHARABLE_CLASS( CCFNotifyAction ): public CCFScriptAction
    {
public: // Constructors and destructor

    /**
    * Creates a new CCFNotifyAction.
    * @param aServices is a reference to script operation services.
    * @param aActionId is the action identifier.
    * @param aParameters contains the action parameters.
    */
    static CCFNotifyAction* NewL( MCFOperationServices& aServices, 
            const TDesC& aActionId, 
            const RPointerArray< CCFKeyValuePair >& aParameters );

    /**
    * Creates a new CCFNotifyAction, leaves it into CleanupStack.
    * @param aServices is a reference to script operation services.
    * @param aActionId is the action identifier.
    * @param aParameters contains the action parameters.
    */
    static CCFNotifyAction* NewLC( MCFOperationServices& aServices, 
            const TDesC& aActionId, 
            const RPointerArray< CCFKeyValuePair >& aParameters );

    /**
    * Destructor.
    */
    ~CCFNotifyAction();

public: // Functions from CCFScriptAction

    /**
    * Notifies the action handler about new action.
    * See CCFScriptAction::ActL
    */
    void ActL();

	// See CCFScriptAction::GetSecurityPolicy
	TInt GetSecurityPolicy( TSecurityPolicy& aPolicy );

protected:

    /**
    * C++ default constructor.
    */
    CCFNotifyAction( MCFOperationServices& aServices );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( const TDesC& aActionId,
            const RPointerArray< CCFKeyValuePair >& aParameters );

private: // Data

    // Action parameters.
    RPointerArray< CCFKeyValuePair >    iParameters;

    // Action identifier.
    HBufC*                              iIdentifier;
    };

#endif // C_CFNOTIFYACTION_H
