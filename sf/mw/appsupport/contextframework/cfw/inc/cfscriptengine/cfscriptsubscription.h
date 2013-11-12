/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFScriptSubscription class declaration.
*
*/



#ifndef C_CFSCRIPTSUBSCRIPTION_H
#define C_CFSCRIPTSUBSCRIPTION_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MCFContextInterface;
class MCFContextSubscriptionListener;
class MCFScriptListener;
class CCFContextSubscriptionImpl;
class CCFContextObject;

// CLASS DECLARATION

/**
*  Script subscription class provides a centralized context subscription for
*  operations on Context Framework Scripts. Script subscription stores all
*  operation subscriptions for a context and manages one real context
*  subscription on Context Framework.
*
*  @lib CFScriptEngine
*  @since S60 v5.0
*/
NONSHARABLE_CLASS( CCFScriptSubscription ): public CBase
    {
public: // Constructors and destructor

    /**
    * Creates a CCFScriptSubscription instance.
    * @param aCF is a reference to context interface.
    * @param aCFListener is a reference to context subscription listener.
    * @param aListener is a pointer to script listener, i.e. context operation.
    * @return a pointer to the created instance of CCFScriptSubscription.
    */
    static CCFScriptSubscription* NewL( MCFContextInterface& aCF,
            MCFContextSubscriptionListener& aCFListener,
            MCFScriptListener* aListener );

    /**
    * Creates a CCFScriptSubscription instance.
    * @param aCF is a reference to context interface.
    * @param aCFListener is a reference to context subscription listener.
    * @param aListener is a pointer to script listener, i.e. context operation.
    * @return a pointer to the created instance of CCFScriptSubscription.
    */
    static CCFScriptSubscription* NewLC( MCFContextInterface& aCF,
            MCFContextSubscriptionListener& aCFListener,
            MCFScriptListener* aListener );

    /**
    * Destructor.
    */
    virtual ~CCFScriptSubscription();

public: // New functions

    /**
    * Adds a script listener to this context subscription. Notifies the listener
    * about the current value of subscribed context.
    * @param aListener is a pointer to script listener, i.e. context operation.
    * @return None.
    */
    void AddListenerL( MCFScriptListener* aListener );

    /**
    * Removes a script listener from this context subscription. Removing the
    * last listener also removes the context subscription.
    * @param aListener is a pointer to script listener, i.e. context operation.
    * @return ETrue if there are no more listeners and the instance could be
    *   deleted, EFalse otherwise.
    */
    TBool RemoveListener( MCFScriptListener* aListener );

    /**
    * Checks whether the context subscription is a match with a script listener.
    * @param aListener is a pointer to script listener, i.e. context operation.
    * @return ETrue if the subscription is a match with the listener, EFalse
    *   otherwise.
    */
    TBool Match( const MCFScriptListener& aListener ) const;

    /**
    * Checks whether the context subscription is a match with a context.
    * @param aContext is a reference to a context.
    * @return ETrue if the subscription is a match with the context, EFalse
    *   otherwise.
    */
    TBool Match( const CCFContextObject& aContext ) const;

    /**
    * Notifies listeners about a context, i.e. evaluates context operations with
    * the context.
    * @param aContext is a reference to the context to be notified.
    * @param aContextLevelDelay is storage for context level delay requirement
    *   from the context operation evaluation.
    * @return ETrue if any listener executed actual evaluation, EFalse
    *   otherwise.
    */
    TBool NotifyListeners( const CCFContextObject& aContext,
            TInt& aContextLevelDelay ) const;

protected:

    /**
    * C++ default constructor.
    */
    CCFScriptSubscription( MCFContextInterface& aCF,
            MCFContextSubscriptionListener& aCFListener );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( MCFScriptListener* aListener );

private: // New functions

    /**
    * Subscribes context requested by a listener and notifies the listener about
    * the current value of the context.
    * @param aListener is a pointer to script listener, i.e. context operation.
    * @return KErrNone if successful, otherwise any system wide error code.
    */
    TInt DoSubscribeL( MCFScriptListener* aListener );

    /**
    * Notifies a listener about the current value of a context it is interested.
    * @param aListener is a pointer to script listener, i.e. context operation.
    * @return None.
    */
    void NotifyListenerOfCurrentContextValueL(
            MCFScriptListener* aListener ) const;

private: // Data

    // Context interface for managing subscriptions.
    MCFContextInterface& iCF;

    // Context subscription listener interface for managing subscriptions.
    MCFContextSubscriptionListener& iCFListener;

    // Flag telling whether subscription has been made (ownership transferred).
    TBool iSubscribed;

    // Context subscription for script listeners; when subscription has been
    // activated (SubscribeContext() called) the ownership has been transferred.
    CCFContextSubscriptionImpl* iSubscription;

    // Script listeners for the context subscription. Not owned.
    RPointerArray< MCFScriptListener > iListeners;
    };


#endif // C_CFSCRIPTSUBSCRIPTION_H
