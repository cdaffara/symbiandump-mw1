/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionSubscription class declaration.
*
*/


#ifndef C_CFACTIONSUBSCRIPTION_H
#define C_CFACTIONSUBSCRIPTION_H

#include <e32base.h>

/**
* Subscription to get information of actions.
* When a client process sets an action subscription it
* basically means that all the subscribed actions are
* executed in the callers process. Action indication
* is received through MCFListener interface when an action
* assocaitade in a rule has been evaluated to true and is
* needed to be executed.
*
* Remember to clearly define what kind of capabilities
* are needed when actions are executed from your process.
*
* @since S60 5.0
* @lib cfservices.lib
*/
NONSHARABLE_CLASS( CCFActionSubscription ): public CBase
    {
    public: // Construction & destruction
 
        // Two phased constrcutors
        IMPORT_C static CCFActionSubscription* NewL();
        IMPORT_C static CCFActionSubscription* NewLC();

    public: // Public API
    
        /**
        * Sets action identifier to subscription.
        * This is the same identifier which is used in rules.
        * For example:
        * 'Phone'
        * 'Musicplayer'
        * 'Musicplayer.Catalogs'
        * 'Mediaplayer.Filelist.Status'
        *
        * @since S60 5.0
        * @param aIdentifier: Identifier of the action
        * @return None
        */
        virtual void SetActionIdentifierL( const TDesC& aIdentifier ) = 0;

        /**
        * Return action identifier.
        *
        * @since S60 5.0
        * @param None
        * @return const TDesC&
        */
        virtual const TDesC& ActionIdentifier() const = 0;
    };

#endif
