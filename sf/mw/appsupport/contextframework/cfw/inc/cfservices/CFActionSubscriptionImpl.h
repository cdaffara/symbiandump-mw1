/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef C_CFACTIONSUBSCRIPTIONIMPL_H
#define C_CFACTIONSUBSCRIPTIONIMPL_H

#include <e32cmn.h>

#include "cfactionsubscription.h"

class MCFActionSubscriptionListener;
class RWriteStream;
class RReadStream;

/**
 * Action subscription implementation.
 */
NONSHARABLE_CLASS( CCFActionSubscriptionImpl ): public CCFActionSubscription
    {
public: // Construction & destruction

    // Two phased constructors
    IMPORT_C static CCFActionSubscriptionImpl* NewL();
    IMPORT_C static CCFActionSubscriptionImpl* NewLC();

    // Destructor
    ~CCFActionSubscriptionImpl();

public: // From base classes

    // From CCFActionSubscription
    void SetActionIdentifierL( const TDesC& aIdentifier );

    // From CCFActionSubscription
    const TDesC& ActionIdentifier() const;

public: // New methods

    /**
     * Externalizes object.
     *
     * @since S60 4.0
     * @param aStream Stream where to write.
     * @return None
     */
    IMPORT_C void ExternalizeL( RWriteStream& aStream );

    /**
     * Internalizes object.
     * 
     * @since S60 4.0
     * @param aStream where to read.
     * @return None
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

    /**
     * Sets listener associated for the subscription.
     * 
     * @since S60 4.0
     * @param aListener Listener.
     * @return None
     */
    IMPORT_C void SetListener( MCFActionSubscriptionListener* aListener );

    /**
     * Returns listener associated with the subscription.
     * 
     * @since S60 4.0
     * @param None
     * @return MCFActionSubscriptionListener&
     */
    IMPORT_C MCFActionSubscriptionListener& Listener() const;

    /**
     * Returns object size.
     *
     * @since S60 4.0
     * @param None
     * @return TInt
     */
    IMPORT_C TInt Size() const;

private:

    void ConstructL();
    CCFActionSubscriptionImpl();

private:

    // Own: Identifier
    HBufC* iIdentifier;

    // Ref: Listener
    MCFActionSubscriptionListener* iListener;
    };

// Action subscription array type definition
typedef RPointerArray<CCFActionSubscriptionImpl> RActionSubscriptionArray;

#endif // C_CFACTIONSUBSCRIPTIONIMPL_H

