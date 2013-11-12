/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MCFOperationServices interface.
*
*/



#ifndef M_CFOPERATIONSERVICES_H
#define M_CFOPERATIONSERVICES_H

// INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class MCFScriptListener;
class CCFOperationNode;
class MCFContextInterface;
class CCFScriptEvent;
class CCFContextSourceCommand;
class CMDXMLNode;

// CLASS DECLARATION

/**
*  This is a service interface for Script Engine operations.
*
*  @lib CFScriptEngine
*  @since S60 v5.0
*/
class MCFOperationServices
    {
public: // New functions

	/**
	* Parses a xml node and returns a pointer to CCFOperationNode instance.
	* @param aParent is the parsed parent node for the aNode.
	* @param aNode is the node to be parsed.
	* @return a ptr to CCFOperationNode instance parsed from the given node,
	*   Note that the ownership is transferred. Leaves with code
	*   KErrNotSupported if the node cannot be parsed.
	*/
	virtual CCFOperationNode* ParseL( CCFOperationNode* aParent,
	         CMDXMLNode& aNode ) = 0;

    /**
    * Gets id of the script providing operation services.
    * @returns script id of this operation services provider.
    */
    virtual TInt ScriptId() const = 0;

    /**
    * Checks a script owners capabilities against given security policy.
    * This is typically called by the operation nodes when 
    * the CheckSecurity()-method for them is called.
    * @param aPolicy is the security policy.
    * @return KErrNone when security check passed, otherwise any of the system
    *   wide error codes.
    */
    virtual TInt CheckScriptOwnerAccess( const TSecurityPolicy& aPolicy ) = 0;
	
	/**
    * Gets context interface.
    * @return a reference to context interface.
    */
    virtual MCFContextInterface& ContextInterface() const = 0;
	
	/**
    * Adds subscription to Context Framework to get indications of context
    * events.
    * @param aListener is a pointer to listener interface that will be notified
    *  of the specified context events.
    * @return None. If subscribing is unsuccessful, leaves with any of system
    *   wide error codes.
    */
    virtual void SubscribeContextL( MCFScriptListener* aListener ) = 0;

    /**
    * Removes subscription from Context Framework, cancels indications of
    * context events.
    * @param aListener is a pointer to listener interface whose context
    *   subscription is cancelled.
    * @return None.
    */
    virtual void RemoveSubscription( MCFScriptListener* aListener ) = 0;

    /**
    * Checks a context read security.
    * @param aSource is the context source.
    * @param aType is the context type.
    * @return KErrNone when security check passed, otherwise any of the system
    *   wide error codes.
    */
    virtual TInt CheckContextReadSecurity( const TDesC& aSource,
              const TDesC& aType ) = 0;

    /**
     * Stores operation data into a persistent file. If the file exists
     * the old data is replaced by calling ExternalizeL for the operation.
     * @param aFile is the persistent file for operation data.
     * @param aOperation is the operation to be stored.
     * @return None.
     */
     virtual void StoreL( const TDesC& aFile, CCFOperationNode& aOperation ) = 0;
     
     /**
     * Restores operation data from a persistent file. If the file exists
     * InternalizeL is called for the operation.
     * @param aFile is the persistent file containing operation data.
     * @param aOperation is the operation to be restored.
     * @return None.
     */
     virtual void RestoreL( const TDesC& aFile,
            CCFOperationNode& aOperation ) = 0;
     
     /**
     * Deletes the persistent file.
     * Operation node must call this method for all persistent data files
     * it owns when Cleanup() -method is called.
     * @param aFile file name of the persistent data file.
     */
     virtual void Delete( const TDesC& aFile ) = 0;


    /**
    * Check if there are actions to launch and launch them.
    * Should be called after script has been evaluated based on something else
    * than context indication, e.g. timer event.
    * Note that calling this from normal evaluation during context indication
    * results in executing script actions multiple times when script is true.
    */
    virtual void LaunchActivatedActions() = 0;

    /**
    * Fires an action.
    * @param aEvent is the script event defining the action. Ownership is
    *   transferred.
    * @return None.
    */
    virtual void FireActionL( CCFScriptEvent* aEvent ) = 0;

    /**
    * Fires a context source command action.
    * @param aCommand is the context source command.
    * @return None.
    */
    virtual void FireActionL( const CCFContextSourceCommand& aCommand ) = 0;

    /**
    * Gets the security policy of a particular action.
    * @param aActionId is the identifier of the action.
    * @param aPolicy is the security policy which will be set by this method.
    * @return KErrNone if action was found, KErrNotFound if not.
    */
    virtual TInt GetActionSecurityPolicy( const TDesC& aActionId,
        TSecurityPolicy& aPolicy ) = 0;
    };


#endif // M_CFOPERATIONSERVICES_H
