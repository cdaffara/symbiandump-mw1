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
* Description:  MCFContextSubscriptionListener interface declaration.
*
*/


#ifndef M_CFCONTEXTSUBSCRIPTIONLISTENER_H
#define M_CFCONTEXTSUBSCRIPTIONLISTENER_H

// CONSTANTS
const TInt KCFSubscriptionListenerInterfaceUidValue = 0x20016BC0;
const TUid KCFSubscriptionListenerInterfaceUid = {KCFSubscriptionListenerInterfaceUidValue};

// FORWARD DECLARATIONS
class CCFContextIndication;

/**
* Context Subscription listener.
* All context subscribers must implement this interface.
* When the subscribed context is changed context indication will
* be received through this interface.
* 
* Notice that this interface must be returned also from the Extension() method!
*
* @lib -
* @since S60 5.0
*/
class MCFContextSubscriptionListener
    {
    public:
    
        /**
        * Indicates that context has changed.
        * Clients needs first to subscribed to a context before
        * context indications can be received.
        * 
        * @since S60 5.0
        * @param aIndication: Context changed
        * @return None
        */
        virtual void ContextIndicationL( CCFContextIndication* aIndication ) = 0;
        
        /**
        * Requests client side thread handle.
        * Ownership is trasferred to the caller. Do not put aThread
        * into clean up stack or close it.
        * 
        * @since S60 5.0
        * @param aThread Client thread handle.
        * @return KErrNone if no errors.
        */
        virtual TInt Client( RThread& aThread ) const = 0;
        
        /**
        * Called when error occurs while adding subscriptions.
        *
        * @since S60 5.0
        * @param aError Error that occured.
        * @param aSource Context source related to the error.
        * @param aType Context type related to the error.
        * @return None
        */
        virtual void HandleContextSubscriptionError( TInt aError,
            const TDesC& aSource,
            const TDesC& aType ) = 0;
        
    protected:
    
        // Deny destruction through this class
        virtual ~MCFContextSubscriptionListener(){};
    };

#endif
