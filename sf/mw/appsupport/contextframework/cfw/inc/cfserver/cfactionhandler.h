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
* Description:  MCFActionHandler interface.
*
*/



#ifndef M_CFACTIONHANDLER_H
#define M_CFACTIONHANDLER_H

// FORWARD DECLARATIONS
class CCFScriptEvent;
class CCFContextSourceCommand;

// CLASS DECLARATION

/**
* M interface class for notifying action handler provider about an action event
* and getting a security policy of the action.
*
*  @lib CFServer
*  @since Series 60 2.6
*/
class MCFActionHandler
    {
public:

    /**
    * Notifies action handler provider about the occurred action event.
    *
    * @param aEvent is a pointer to CCFScriptEvent that contains action (script
    *   event) information. Ownership is transferred.
    * @return None.
    */
    virtual void FireActionL( CCFScriptEvent* aEvent ) = 0;

    /**
    * Notifies action handler provider about the occurred context source command
    * action.
    *
    * @param aCommand is the context source command.
    * @return None.
    */
    virtual void FireActionL( const CCFContextSourceCommand& aCommand ) = 0;

	/**
	* Gets the security policy of a particular action.
	*
	* @param aActionIdentifier is the identifier of the action.
	* @param aPolicy is the security policy which will be set by this method.
	* @return KErrNone if action was found, KErrNotFound if not.
	*/
	virtual TInt GetActionSecurityPolicy( const TDesC& aActionIdentifier,
		TSecurityPolicy& aPolicy ) = 0;

    };

#endif // M_CFACTIONHANDLER_H
