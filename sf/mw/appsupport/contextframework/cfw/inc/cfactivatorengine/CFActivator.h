/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActivator class declaration.
 *
*/


#ifndef C_CFACTIVATOR_H
#define C_CFACTIVATOR_H

#include <e32base.h>

#include "cfactionhandler.h"
#include "CFActivatorCallBackInterface.h"
#include "CFActivatorEventHandlerCallBack.h"
#include "cfstarterobserver.h"
#include "cfstateobserver.h"
#include "CFActionCacheElement.h"

class CCFActivatorEngineActionPluginManager;
class MCFActionOwner;
class CCFActionIndication;

/**
 *  Activator engine main class.
 *
 *  @lib -
 *  @since S60 4.0
 */
NONSHARABLE_CLASS( CCFActivator ): public CBase,
public MCFActionHandler,
public MCFActivatorCallBackInterface,
public MCFActivatorEventHandlerCallBack,
public MCFStarterObserver,
public MCFStateObserver
    {
public: // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CCFActivator* NewL();

    /**
     * Destructor.
     */
    IMPORT_C virtual ~CCFActivator();

public: // New methods

    /**
     * Defines a new action.
     * @param aActionIdentifier Id of the action.
     * @param aActionCache The currently defined actions.
     * @return CCFActionCacheElement*.
     */
    static CCFActionCacheElement* ActionCacheElementExists(
        const TDesC& aActionIdentifier,
        RActionCacheArray& aActionCache );

private: // Functions from base classes

    // From MCFActionHandler
    void FireActionL( CCFScriptEvent* aEvent );

    // From MCFActionHandler
    void FireActionL( const CCFContextSourceCommand& aCommand );

    // From MCFActionHandler
    TInt GetActionSecurityPolicy( const TDesC& aActionIdentifier,
        TSecurityPolicy& aPolicy );

    // From MCFActivatorCallBackInterface
    void DefineActionL( const TDesC& aActionIdentifier,
        const TSecurityPolicy& aSecurityPolicy,
        const TUid& aOwnerUid,
        MCFActionOwner* aOwner );

    // From MCFActivatorCallBackInterface
    void SubscribeActionL(
        CCFActionSubscription* aActionSubscription,
        MCFActionSubscriptionListener* aListener,
        RThread& aClientThread );

    // From MCFActivatorCallBackInterface
    void UnsubscribeAction(
        CCFActionSubscription& aActionSubscription,
        MCFActionSubscriptionListener& aListener );

    // From MCFActivatorCallBackInterface
    void DeregisterActions( MCFActionOwner* aOwner );

    // From MCFActivatorCallBackInterface
    void RemoveSubscriptions( MCFActionSubscriptionListener& aListener );

    // From MCFStarterObserver
    void InitializePhaseL( CCFPhaseBase::TCFPhaseId aPhaseId );

    // from MCFStateObserver
    void UpdatePlugInsL();

        // From MCFStarterObserver
        void SetEventHandler( MCFStarterEventHandler& aEventHandler );
        
private:

    CCFActivator();
    void ConstructL();

private: // New methods

    // Parses action identifier from the script event
    TPtrC ActionIdentifier( const CCFScriptEvent& aEvent ) const;

    // Creates action indication
    CCFActionIndication* CreateActionIndicationL(
        const TDesC& aActionIdentifier,
        const CCFScriptEvent& aEvent ) const;

    // Check if the current listener has already subscribed
    // for the action.
    TBool AlreadySubscribed( CCFActionCacheElement* aElement,
        MCFActionSubscriptionListener* aListener ) const;

    // Trigger internal action
    void TriggerInternalActionL( const CCFScriptEvent& aEvent );

    // Trigger external action
    void TriggerExternalActionL( const CCFScriptEvent& aEvent,
        const RActionSubscriptionArray& aSubscriptions );

    // Returns ETrue when id is reserved, otherwise EFalse.
    TBool IsReservedActionId( const TDesC& aActionId );

private: // Data

    // Own:
    CCFActivatorEngineActionPluginManager* iActionPluginManager;

    // Own: Defined actions
    RActionCacheArray iActionCache;
    };

#endif
