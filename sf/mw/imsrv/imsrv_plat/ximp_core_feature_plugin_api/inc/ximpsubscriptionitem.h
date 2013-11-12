/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Subscription item interface
*
*/

#ifndef MXIMPPSCSUBSCRIPTIONITEM_H
#define MXIMPPSCSUBSCRIPTIONITEM_H

class MXIMPPscContext;
class CXIMPDataSubscriptionStateImp;

/**
 * Context interface.
 *
 * @since S60 v3.2
 */
class MXIMPSubscriptionItem
    {
    
public: // Interface

    /**
     * Synthesise own presence information to given context.
     * Presence information is filtered with context pif.
     * @param aContext. Target context for presence information.
     */    
    virtual void SynthesiseSubscriptionEventToL( MXIMPPscContext* aContext, TBool aForceEvent ) = 0;
    
    /**
     * Synthesise presence information to all subscribed contexts.
     */
    virtual void SynthesiseSubscriptionEventToAllCtxsL() = 0;
            
    /**
     * Activate subscriber that is added with AddSubscriberL method of subscriptionItem
     * @param aContext. Target context.
     */
    virtual void ActivateSubscriberL( MXIMPPscContext* aContext ) = 0;

    /**
     * Set new subscriptionstate normally invoked by adaptation.
     * @param aNewState New subscription state
     */
    virtual void SetDataSubscriptionStateL( CXIMPDataSubscriptionStateImp& aNewState ) = 0;
        
protected: // Destruction
    
    /**
     * Object cannot be deleted through this interface
     */
    virtual ~MXIMPSubscriptionItem(){}
    };

/* ======================================================================== */



#endif // MXIMPPSCSUBSCRIPTIONITEM_H
