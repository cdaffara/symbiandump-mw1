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
* Description:  
*
*/


#ifndef C_CFSCRIPT_H
#define C_CFSCRIPT_H

#include <e32base.h>
#include <e32std.h>
#include <f32file.h>

#include "cfcontextsubscriptionlistener.h"
#include "cfoperationservices.h"
#include "CFSecurityChecker.h"

class MCFActionHandler;
class MCFContextInterface;
class CCFContextObject;
class CCFContextSubscriptionImpl;
class CCFScriptSubscription;
class CCFScriptRoot;
class CCFOperationNode;
class CCFScriptEvent;
class CMDXMLNode;
class MCFPlugInOperation;
class MCFPersistentDataInterface;
class CCFPersistentData;
class CCFScriptInfo;
class MCFScriptOwner;

/**
 * Client to ContextManager context object changes; subscribes all
 * CCFContextObject needed for the script. When the script's value changes
 * from EFalse to ETrue, the callback in CCFScriptHandler is called.

 * @lib CFScriptEngine
 * @since Series 60 2.6
 */
NONSHARABLE_CLASS( CCFScript ): public CBase,
public MCFContextSubscriptionListener,
public MCFOperationServices
    {
public: // Constructors and destructor

    /**
     * Two-phased constructor.
     * @param aStartNode contains XML node where the script starts
     * @param aName Script's name, filename or secure id from client thread
     * @param aScriptId is id for this script
     * @param aActionHandler is MCFActionHandler that is notified when
     *  notify action fires.
     */
    static CCFScript* NewL( CMDXMLNode& aStartNode,
        const TDesC& aName,
        TInt aScriptId,
        const TUid& aOwner,
        MCFActionHandler& aActionHandler,
        TInt aLength,
        MCFExtendedContextInterface& aCF,
        MCFSecurityChecker& aSecurityChecker,
        MCFPlugInOperation& aPlugInOperation,
        RFs& aFs,
        MCFScriptOwner* aScriptOwner );

    /**
     * Two-phased constructor that leaves object to the cleanup stack.
     * @param aStartNode contains XML node where the script starts
     * @param aName Script's name, filename or secure id from client thread
     * @param aScriptId is id for this script
     * @param aActionHandler is MCFActionHandler that is notified when
     *  notify action fires.
     */
    static CCFScript* NewLC( CMDXMLNode& aStartNode,
        const TDesC& aName,
        TInt aScriptId,
        const TUid& aOwner,
        MCFActionHandler& aActionHandler,
        TInt aLength,
        MCFExtendedContextInterface& aCF,
        MCFSecurityChecker& aSecurityChecker,
        MCFPlugInOperation& aPlugInOperation,
        RFs& aFs,
        MCFScriptOwner* aScriptOwner );

    /**
     * Destructor.
     */
    ~CCFScript();

public: // New functions

    /**
     * Activates the script by activating the script root.
     */
    void ActivateL();

    /**
     * Check that aOwnerThread has capabilities to do subscriptions
     * and operations needed by the script.
     * @param aOwnerThread is the script owning thread for security checking.
     * @return KErrNone if security check passed, KErrNotFound if
     *   all actions or contexts needed by the script were not found,
     *   KErrPermissionDenied if the aOwnerThread does not have sufficient
     *   capabilities.
     */
    TInt CheckSecurity( const RThread& aOwnerThread );

    /**
     * Gets action handler (session) associated with the script.
     * @return a reference to action handler listening this script.
     */
    MCFActionHandler& ActionHandler() const;

    /**
     * Gets script upgrade security level.
     * @return const TCapabilitySet&.
     */
    const TCapabilitySet& UpgradeSecurity() const;

    /**
     * Request operation nodes to clean up persistent data.
     * @since S60 5.0
     * @param None.
     * @return None.
     */
    void CleanupPersistentData();

    /**
     * Checks if this script has a dependency to the operation plug-in
     * implementation provided as a parameter.
     * @param aUid Provider uid.
     * @return ETrue if has dependency.
     */
    TBool HasDependency( const TUid& aUid ) const;

    /**
     * Returns a copy of script info.
     * @return CCFScriptInfo*.
     */
    CCFScriptInfo* CopyInfoLC() const;
    
    /**
     * Return owner uid.
     * @return TUid&.
     */
    const TUid& OwnerUid() const;
    
    /**
     * Return script name.
     * @return const TDesC&.
     */
    TPtrC Name() const;
    
    /**
     * Return script length.
     * @return TInt.
     */
    TInt Length() const;
    
    /**
     * Return reference to the info object.
     * @return CCFScriptInfo&.
     */
    CCFScriptInfo& Info() const;

public: // Functions from MCFContextSubscriptionListener

    // From MCFContextSubscriptionListener
    void ContextIndicationL( CCFContextIndication* aIndication );

    // From MCFContextSubscriptionListener
    TInt Client( RThread& aThread ) const;

    // From MCFContextSubscriptionListener
    void HandleContextSubscriptionError( TInt aError,
        const TDesC& aSource,
        const TDesC& aType );

public: // Functions from MCFOperationServices

    /**
     * Parses a xml node and returns a pointer to CCFOperationNode instance.
     * @param aParent is the parsed parent node for the aNode.
     * @param aNode is the node to be parsed.
     * @return a ptr to CCFOperationNode instance parsed from the given node,
     *   Note that the ownership is transferred. Leaves with code
     *   KErrNotSupported if the node cannot be parsed.
     */
    CCFOperationNode* ParseL( CCFOperationNode* aParent,
        CMDXMLNode& aNode );

    /**
     * Gets id of the script providing operation services.
     * @returns script id of this operation services provider.
     */
    TInt ScriptId() const;

    /**
     * Checks a script owners capabilities against given security policy.
     * This is typically called by the operation nodes when 
     * the CheckSecurity()-method for them is called.
     * @param aPolicy is the security policy.
     * @return KErrNone when security check passed, otherwise any of the system
     *   wide error codes.
     */
    TInt CheckScriptOwnerAccess( const TSecurityPolicy& aPolicy );

    /**
     * Gets context interface.
     * @return a reference to context interface.
     */
    MCFContextInterface& ContextInterface() const;

    /**
     * Adds subscription to Context Framework to get indications of context
     * events.
     * @param aListener is a pointer to listener interface that will be notified
     *  of the specified context events.
     * @return None. If subscribing is unsuccessful, leaves with any of system
     *   wide error codes.
     */
    void SubscribeContextL( MCFScriptListener* aListener );

    /**
     * Removes subscription from Context Framework, cancels indications of
     * context events.
     * @param aListener is a pointer to listener interface whose context
     *   subscription is cancelled.
     * @return None.
     */
    void RemoveSubscription( MCFScriptListener* aListener );

    /**
     * Checks a context read security.
     * @param aSource is the context source.
     * @param aType is the context type.
     * @return KErrNone when security check passed, otherwise any of the system
     *   wide error codes.
     */
    TInt CheckContextReadSecurity( const TDesC& aSource,
        const TDesC& aType );

    /**
     * Stores operation data into a persistent file. If the file exists
     * the old data is replaced by calling ExternalizeL for the operation.
     * @param aFile is the persistent file for operation data.
     * @param aOperation is the operation to be stored.
     * @return None.
     */
    void StoreL( const TDesC& aFile, CCFOperationNode& aOperation );

    /**
     * Restores operation data from a persistent file. If the file exists
     * InternalizeL is called for the operation.
     * @param aFile is the persistent file containing operation data.
     * @param aOperation is the operation to be restored.
     * @return None.
     */
    void RestoreL( const TDesC& aFile,
        CCFOperationNode& aOperation );

    /**
     * Deletes the persistent file.
     * Operation node must call this method for all persistent data files
     * it owns when Cleanup() -method is called.
     * @param aFile file name of the persistent data file.
     */
    void Delete( const TDesC& aFile );

    /**
     * Check if there are actions to launch and launch them.
     * Should be called after script has been evaluated based on something else
     * than context indication, e.g. timer event.
     * Note that calling this from normal evaluation during context indication
     * results in executing script actions multiple times when script is true.
     */
    void LaunchActivatedActions( );

    /**
     * Fires an action.
     * @param aEvent is the script event defining the action. Ownership is
     *   transferred.
     * @return None.
     */
    void FireActionL( CCFScriptEvent* aEvent );

    /**
    * Fires a context source command action.
    * @param aCommand is the context source command.
    * @return None.
    */
    void FireActionL( const CCFContextSourceCommand& aCommand );

    /**
     * Gets the security policy of a particular action.
     * @param aActionId is the identifier of the action.
     * @param aPolicy is the security policy which will be set by this method.
     * @return KErrNone if action was found, KErrNotFound if not.
     */
    TInt GetActionSecurityPolicy( const TDesC& aActionId,
        TSecurityPolicy& aPolicy );

private:

    /**
     * C++ default constructor.
     */
    CCFScript( MCFExtendedContextInterface& aCF,
        MCFActionHandler& aActionHandler,
        MCFSecurityChecker& aSecurityChecker,
        MCFPlugInOperation& aPlugInOperation,
        RFs& aFs );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL( CMDXMLNode& aStartNode, const TDesC& aName,
        TInt aScriptId, const TUid& aOwner, TInt aLength,
        MCFScriptOwner* aScriptOwner );

private: // New functions

    /**
     * Starting point for script tree creation.
     * @since Series 60 5.0
     * @param aStartNode node to start parsing from.
     * @return KErrNone if successful, otherwise appropriate system wide
     *   error code.
     */
    TInt ParseScriptL( CMDXMLNode& aStartNode );

    /**
     * Evaluates script with a context.
     * @since Series 60 4.0
     * @param aContext is the context used for evaluating the script.
     * @param aContextLevelDelay will be set to context level delay in
     *  milliseconds if required by evaluation, otherwise left untouched.
     * @return ETrue if any script operation was really evaluated, EFalse
     *   otherwise.
     */
    TBool EvaluateScript( const CCFContextObject& aContext,
        TInt& aContextLevelDelay );

    /**
     * Adds new dependency in script.
     * The dependency is the implementation uid of the operation plug-in.
     * This link is needed since if the operation plug-in is uninstalled,
     * all the scripts which has the operation plug-in dependency must be
     * deregistered.
     * @param aUid Implementation uid of the dependecy.
     */
    void AddDependencyL( const TUid& aUid );

private: // Data

    // Reference to context services.
    MCFExtendedContextInterface& iCF;

    // Reference to the action handler.
    MCFActionHandler& iActionHandler;

    // Interface for security checking.
    MCFSecurityChecker& iSecurityChecker;

    // Reference to parsing plugin operations.
    MCFPlugInOperation& iPlugInOperation;

    // Reference to File server session.
    RFs& iFs;

    // Script info
    CCFScriptInfo* iInfo;

    // The root node of the script.
    CCFScriptRoot* iScriptRoot;

    // Script subscriptions, owned.
    RPointerArray< CCFScriptSubscription > iScriptSubscriptions;

    // Script listeners requiring all context indications, not owned.
    RPointerArray< MCFScriptListener > iRequiresAll;

    // Script owning thread for security checking. Valid only during when
    // CheckSecurity() is called, other times pointer is NULL.
    const RThread* iScriptThreadForSecurityCheck;

    // Own: Persistent data handling for scripts.
    CCFPersistentData* iPersistentData;

    // Own: Array of operation plug-in uids
    RArray<TUid> iDependencyList;
    };

#endif // C_CFSCRIPT_H

