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
* Description:  Abstract CCFScriptAction class declaration.
*
*/



#ifndef C_CFSCRIPTACTION_H
#define C_CFSCRIPTACTION_H

// INCLUDES
#include <e32base.h>
#include <e32std.h>

// FORWARD DECLARATIONS
class MCFOperationServices;

// CLASS DECLARATION

/**
*   Base class for all script actions in Context Framework.
*
*  @lib BasicOperationsPlugIn
*  @since Series 60 5.0
*/
NONSHARABLE_CLASS( CCFScriptAction ): public CBase
    {
public:

    enum TActionType
        {
        EPublishContext,
        ENotify,
        ESourceCommand
        };

    /*
    * Destructor
    */
    virtual ~CCFScriptAction();

public: // New functions

    /*
    * Method to access ActionType
    * @return ActionType
    * @since Series 60 2.6
    */
    const TActionType& ActionType();

    /*
    * Action method that must be implemented by all inherited classes.
    *  @since Series 60 2.6
    */
    virtual void ActL() = 0;

    /**
	* Gets the security policy of particular action.
    * @param aPolicy will contains the actions security policy after successful
    *   call.
    * @return KErrNone if action definition found, KErrNotFound if not.
	*/
	virtual TInt GetSecurityPolicy( TSecurityPolicy& aPolicy ) = 0;

protected:

    /**
    * C++ default constructor. 
    */
    CCFScriptAction( MCFOperationServices& aServices, TActionType aType );

protected: // Data

    // Operation service interface.
    MCFOperationServices& iServices;

private: // Data

    // Action type.
    TActionType  iType;
    }; 

#endif // C_CFSCRIPTACTION_H
