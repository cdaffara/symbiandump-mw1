/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


#ifndef M_CFACTIONSUBSCRIPTIONLISTENER_H
#define M_CFACTIONSUBSCRIPTIONLISTENER_H

class CCFActionIndication;

/**
* Context Subscription listener.
* @internal use only
*/
class MCFActionSubscriptionListener
    {
    public:
    
        /**
        * Indicates that action is needed to be performed.
        * Clients needs first to subscribed to an action before
        * action indications can be received.
        * Ownership of aIndication is transeferred to client.
        * 
        * @since S60 4.0
        * @param aIndication: Action with parameters.
        * @return None
        */
        virtual void ActionIndicationL( CCFActionIndication* aIndication ) = 0;
        
    protected:
    
        // Deny destruction through this class
        virtual ~MCFActionSubscriptionListener(){};
    };

#endif//M_CFACTIONSUBSCRIPTIONLISTENER_H
