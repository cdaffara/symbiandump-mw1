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
* Description:  CCFFakeEnv class declaration.
*
*/



#ifndef C_CFFAKEENV_H
#define C_CFFAKEENV_H

// INCLUDES
#include <e32base.h>
#include <f32file.h>

#include <CFKeyValuePair.h>

#include "cfextendedcontextinterface.h"
#include "CFScriptEngineInterface.h"
#include "ScriptEventNotifierSession.h"
#include "cfsecuritychecker.h"
#include "CFContextSubscriptionListener.h"
#include "cfscriptowner.h"

// FORWARD DECLARATIONS
class CCFScriptHandler;
class CCFContextManager;
class CCFContextObject;
class CCFContextIndication;

// CLASS DECLARATION

/**
*  This class implements fake Context Framework environment for testing only.
*
*  @lib
*  @since S60 v5.0
*/
NONSHARABLE_CLASS( CCFFakeEnv ): public CBase,
    public MCFExtendedContextInterface,
    public MCFScriptEngineInterface,
    public MCFActionHandler,
    public MCFSecurityChecker,
    public MCFContextSubscriptionListener,
    public MCFScriptOwner
    {
public: // Constructors and destructor

    /**
    * Creates a CCFFakeEnv object.
    * @return a pointer to the created instance of CCFFakeEnv.
    */
    static CCFFakeEnv* NewL();

    /**
    * Creates a CCFFakeEnv object.
    * @return a pointer to the created instance of CCFFakeEnv.
    */
    static CCFFakeEnv* NewLC();

    /**
    * Destructor.
    */
    ~CCFFakeEnv();

public: // New functions

    /**
    * Clears fire client event flag.
    * @return None.
    */
    void ClearFireClientEvent();

    /**
    * Tells whether client event has been fired or not.
    * @return ETrue if client event has been fired, EFalse otherwise.
    */
    TBool ClientEventFired() const;

    /**
    * Clears fire client event count.
    * @return None.
    */
    void ClearFireClientEventCount();

    /**
    * Gets client event fired count.
    * @return The count how many times client event has been fired.
    */
    TInt ClientEventFiredCount() const;

    /**
    * Gets script id from the previously fired client event.
    * @return Script id from the previously fired client event.
    */
    TInt ScriptEventScriptId() const;

    /**
    * Gets script event identifier from the previously fired client event.
    * @return Script event identifier from the previously fired client event.
    */
    TPtrC ScriptEventIdentifier() const;

    /**
    * Gets script event parameters from the previously fired client event.
    * @return Script event parameters from the previously fired client event.
    */
    const RKeyValueArray& ScriptEventParameters() const;

    /**
    * Sets the count how many times the call to GetSecurityPolicy will not find
    * the action (and returns KErrNotFound).
    * @param aCount is the count how many times action will not be found.
    * @return None.
    */
    void SetActionNotFound( TInt aCount );

    /**
    * Sets the count how many times the call to GetSecurityPolicy will always
    * fail (returns always failing security policy).
    * @param aCount is the count how many times always fail security policy will
    *   be returned by the GetSecurityPolicy call.
    * @return None.
    */
    void SetAlwaysFailingActionPolicy( TInt aCount );

    /**
    * Tells whether context indication has arrived or not.
    * @return ETrue when context indication has arrived, EFalse otherwise.
    */
    TBool IndicationArrived();

    /**
    * Sets context indication arrived flag to a value.
    * @param aValue is the value for context indication arrived flag.
    * @return None.
    */
    void SetIndicationArrived( TBool aValue );

    /**
    * Gets context from last context indication.
    * @return Constant reference to CCFContextObject from the last context
    *   indication.
    */
    const CCFContextObject& IndicationContext();

    /**
    * Sets the count for bypass security check count that controls the how many
    *  subsequent calls to CheckClientSecurity will always pass.
    * @param aCount is the count defining the times security check will be
    *   bypassed.
    * @return None.
    */
    void SetBypassSecurityCheckCount( TInt aCount );

    /**
    * Define context made easy.
    * Version not requiring knowledge of context source etc.
    * @see MCFContextInterface and MCFExtendedContextInterface.
    */
    TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadPolicy,
            const TSecurityPolicy& aWritePolicy );

    /**
    * Add script made easy.
    * Version not requiring knowledge of script action handler since this
    * instance acts as the action handler.
    * @see MCFScriptEngineInterface.
    */
    TInt AddScript( const TDesC& aName,
            const TDesC8& aScript,
            const TUid& aOwner,
            const RThread& aOwnerThread );

    /**
    * Subscribe context made easy.
    * Version not requiring knowledge of context subscription listener since
    * this instance acts as the listener.
    * @see MCFContextInterface.
    */
    TInt SubscribeContext( CCFContextSubscription* aSubscription,
        RThread& aClientThread );
    
    /**
     * Instantiate first phase script loading.
     */
    void InitDeviceStartingPhaseL();

    /**
     * Instantiate first phase script loading.
     */
    void InitDeviceStartedPhaseL();
    
    /**
     * Init all phases.
     */
    void InitAllPhasesL();

public: // From MCFExtendedContextInterface (and MCFContextInterface)

    // @see MCFContextInterface
    TInt DefineContext( const TDesC& aContextSource,
        const TDesC& aContextType,
        const TSecurityPolicy& aReadWritePolicy );

    // @see MCFContextInterface
    TInt DefineContext( const TDesC& aContextSource,
        const TDesC& aContextType,
        const TSecurityPolicy& aReadWritePolicy,
        MCFContextSource& aOwner );

    // @see MCFContextInterface
    TInt DefineContext( const TDesC& aContextSource,
        const TDesC& aContextType,
        const TSecurityPolicy& aReadPolicy,
        const TSecurityPolicy& aWritePolicy,
        MCFContextSource& aOwner );

    // @see MCFContextInterface
    TInt PublishContext( CCFContextObject& aContext,
        RThread& aClientThread );

    // @see MCFContextInterface
    TInt PublishContext( CCFContextObject& aContext,
        CCFContextDataObject& aData,
        RThread& aClientThread );

    // @see MCFContextInterface
    TInt SubscribeContext(
        CCFContextSubscription* aSubscription,
        MCFContextSubscriptionListener* aListener,
        RThread& aClientThread );

    // @see MCFContextInterface
    void UnsubscribeContext(
        CCFContextSubscription& aSubscription,
        MCFContextSubscriptionListener& aListener );

    // @see MCFContextInterface
    void UnsubscribeContexts(
        MCFContextSubscriptionListener& aObserver );

    // @see MCFContextInterface
    TInt RequestContext(
        RContextObjectArray& aResultBuffer,
        CCFContextQuery& aContextQuery,
        RThread& aClientThread );

    // @see MCFContextInterface
    TInt GetSecurityPolicy( const TDesC& aContextSource,
        const TDesC& aContextType,
        TSecurityPolicy& aPolicy );

    // @see MCFExtendedContextInterface
    TInt GetReadSecurityPolicy( const TDesC& aContextSource,
        const TDesC& aContextType,
        TSecurityPolicy& aPolicy );

    // @see MCFExtendedContextInterface
    TInt GetWriteSecurityPolicy( const TDesC& aContextSource,
        const TDesC& aContextType,
        TSecurityPolicy& aPolicy );

    // @see MCFExtendedContextInterface
    TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadPolicy,
            const TSecurityPolicy& aWritePolicy,
            MCFContextSource* aPublisher, const TUid& aPublisherUid );

    // @see MCFExtendedContextInterface
    void DeregisterPublisher(
            MCFContextSource& aPublisher );

    // @see MCFExtendedContextInterface
    TInt InstallContextSourceSetting( RFile& aSettingFile,
            const TUid& aContextSourceUid,
            RThread& aClientThread );

    // @see MCFExtendedContextInterface
    TInt UninstallContextSourceSetting(
            const TDesC& aSettingFilename,
            const TUid& aContextSourceUid,
            RThread& aClientThread );

    // @see MCFExtendedContextInterface
    TInt UninstallContextSourceSettings(
            const TUid& aContextSourceUid,
            RThread& aClientThread );

    // @see MCFExtendedContextInterface
    TInt PublishContextFromAction( CCFContextObject& aContext, TBool aAutoDefine );

    // @see MCFScriptOwner
    void HandleScriptsRemoved( const RArray<TInt>& aScriptIds );

public: // From MCFScriptEngineInterface

    // @see MCFScriptEngineInterface
    TInt RemoveScriptById( TInt aScriptId, const RThread& aOwner );

    // @see MCFScriptEngineInterface
    TInt RemoveScriptsBySession( const MCFActionHandler& aSession );

    // @see MCFScriptEngineInterface
    TInt RemoveScriptByName( const TDesC& aScriptName, const TUid& aOwnerUid );

    // @see MCFScriptEngineInterface
    TInt RemoveScriptByUid( const TUid& aUid );

    // @see MCFScriptEngineInterface
    TInt AddScript( const TDesC& aName,
            const TDesC8& aScript,
            const TUid& aOwner,
            const RThread& aOwnerThread,
            MCFActionHandler& aSession,
            MCFScriptOwner* aScriptOwner );

    // @see MCFScriptEngineInterface
    TInt NumberOfScriptsByOwner( const TUid& aOwner );

    // @see MCFScriptEngineInterface
    TInt NumberOfScripts();

    // @see MCFScriptEngineInterface
    TInt ScriptLength( TInt aScriptId );

    // @see MCFScriptEngineInterface
    const RArray< TInt >& GetEveryScriptId();

    // @see MCFScriptEngineInterface
    const RArray< TInt >& GetEveryScriptIdByOwner( const TUid& aScriptOwner );

    // @see MCFScriptEngineInterface
    TInt UpdateScript( TInt aScriptID,
            const RThread& aOwnerThread,
            const TDesC8& aUpdatedScript,
            MCFScriptOwner* aScriptOwner );

    // @see MCFScriptEngineInterface
    TInt SaveScript( const TDesC8& aScript,
            TInt aScriptId,
            const TUid& aOwnerUid );

    // @see MCFScriptEngineInterface
    TInt DeleteScriptByName( const TDesC& aScriptName,
        const TUid& aOwner );

    // @see MCFScriptEngineInterface
    TInt DeleteScriptByUid( const TUid& aUid );

    // @see MCFScriptEngineInterface
    TBool AlreadyExists( const TDesC& aScriptName,
        const TUid& aOwnerUid,
        TInt& aScriptId ) const;

    // @see MCFScriptEngineInterface
    void CleanupPersistentDataByName( const TDesC& aScriptName,
        const TUid& aOwnerUid );

    // @see MCFScriptEngineInterface
    void CleanupPersistentDataByUid( const TUid& aOwnerUid );
    
    // @see MCFScriptEngineInterface
    TInt RestoreRomScript( TInt aScriptId, const TUid& aOwnerUid,
        const RThread& aClient );

    // @see MCFScriptEngineInterface
    TInt IsUpgradeAllowed( const TDesC& aName,
        const TDesC8& aScript,
        const TUid& aOwner,
        const RThread& aOwnerThread,
        MCFActionHandler& aActionHandler );

    // @see MCFScriptEngineInterface
    TInt RemoveScriptByProviderUid( const TUid& aProviderUid,
        TBool aRollback );

    // @see MCFScriptEngineInterface
    void RollbackScripts();

    // @see MCFScriptEngineInterface
    void DeregisterScriptOwner( MCFScriptOwner* aScriptOwner );

public: // From MCFActionHandler

    // @see MCFActionHandler
    void FireActionL( CCFScriptEvent* aEvent );

    // @see MCFActionHandler
    void FireActionL( const CCFContextSourceCommand& aCommand );

    // @see MCFActionHandler
    TInt GetActionSecurityPolicy( const TDesC& aActionIdentifier,
        TSecurityPolicy& aPolicy );

public: // From MCFSecurityChecker

    // @see MCFSecurityChecker
    TBool CheckClientSecurity( const RThread& aClientThread,
        const TSecurityPolicy& aSecurityPolicy );

public: // From MCFContextSubscriptionListener

    // @see MCFContextSubscriptionListener
    void ContextIndicationL( CCFContextIndication* aIndication );

    // @see MCFContextSubscriptionListener
    TInt Client( RThread& aThread ) const;

    // @see MCFContextSubscriptionListener
    void HandleContextSubscriptionError( TInt aError,
        const TDesC& aSource,
        const TDesC& aType );
    
private:

    /**
    * C++ default constructor.
    */
    CCFFakeEnv();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private: // Data

    RFs iFs;

    CCFScriptHandler* iScriptHandler;
    CCFContextManager* iContextManager;

    TInt iBypassSecurityCheck;

    CCFScriptEvent* iScriptEvent;
    TBool iFireClientEvent;
    TInt iFireClientEventCount;

    TInt iActionNotFound;
    TInt iAlwaysFailingActionPolicy;

    TBool iIndication;
    CCFContextIndication* iContextIndication;

    TBool iSubscriptionError;
    };


#endif // C_CFFAKEENV_H
